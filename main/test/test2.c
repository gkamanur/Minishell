#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <termios.h>
#include <ctype.h>
#include <signal.h>

extern char **environ;

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef enum {
    TOKEN_WORD,      // Commands, arguments, filenames 
    TOKEN_SQUOTED,   // Single quoted strings
    TOKEN_DQUOTED,   // Double quoted strings
    TOKEN_OPERATOR,  // Operators like |, >, <, >>, &&
    TOKEN_VARIABLE   // Variables like $HOME
} t_token_type;

typedef struct s_tokens {
    char *token;
    int token_num;
    t_token_type type;
    struct s_tokens *next;
} t_tokens;

typedef struct s_cmd {
    t_tokens **args;
    char *input_file;
    char *output_file;
    int append;
    struct s_cmd *next;
} t_cmd;

void signal_handler(int sig) {
    static volatile sig_atomic_t signal_received = 0;
    signal_received = sig;
    if (sig == SIGINT) {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void init_signals(void) {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}

t_env *create_env_node(const char *key, const char *value) {
    t_env *node = malloc(sizeof(t_env));
    if (!node) return NULL;
    node->key = strdup(key);
    node->value = strdup(value);
    node->next = NULL;
    return node;
}

t_env *init_env(char **envp) {
    t_env *env_list = NULL;
    for (int i = 0; envp[i]; i++) {
        char *entry = strdup(envp[i]);
        char *eq = strchr(entry, '=');
        if (eq) {
            *eq = '\0';
            t_env *node = create_env_node(entry, eq + 1);
            node->next = env_list;
            env_list = node;
        }
        free(entry);
    }
    return env_list;
}

char *get_env_value(const char *key, t_env *env_list) {
    t_env *current = env_list;
    while (current) {
        if (strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return "";
}

void free_env(t_env *env_list) {
    t_env *current = env_list;
    while (current) {
        t_env *tmp = current;
        current = current->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

void free_args(char **args) {
    if (!args) return;
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}

void free_tokens(t_tokens **tokens) {
    if (!tokens) return;
    for (int i = 0; tokens[i]; i++) {
        free(tokens[i]->token);
        free(tokens[i]);
    }
    free(tokens);
}

void free_cmd(t_cmd *cmd) {
    while (cmd) {
        t_cmd *tmp = cmd;
        cmd = cmd->next;
        free_tokens(tmp->args);
        free(tmp->input_file);
        free(tmp->output_file);
        free(tmp);
    }
}

char **tokens_to_args(t_tokens **tokens) {
    if (!tokens) return NULL;
    int count = 0;
    for (int i = 0; tokens[i]; i++) count++;
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args) return NULL;
    for (int i = 0; i < count; i++) args[i] = tokens[i]->token;
    args[count] = NULL;
    return args;
}

char *find_executable(const char *cmd, t_env *env_list) {
    if (strchr(cmd, '/'))
        return access(cmd, X_OK) == 0 ? strdup(cmd) : NULL;

    char *path = get_env_value("PATH", env_list);
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");

    while (dir) {
        char *full_path = malloc(strlen(dir) + strlen(cmd) + 2);
        sprintf(full_path, "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

char *parse_quotes(char *line, int *i, char quote, t_env *env_list, int last_status, t_token_type *type) {
    char *result = malloc(1024);
    int j = 0;
    (*i)++;
    while (line[*i] && line[*i] != quote) {
        if (quote == '"' && line[*i] == '$' && line[*i + 1]) {
            (*i)++;
            *type = TOKEN_VARIABLE;
            if (line[*i] == '?') {
                char status[16];
                sprintf(status, "%d", last_status);
                strcpy(&result[j], status);
                j += strlen(status);
                (*i)++;
            } else {
                char key[64] = {0};
                int k = 0;
                while (line[*i] && (isalnum(line[*i]) || line[*i] == '_'))
                    key[k++] = line[(*i)++];
                char *value = get_env_value(key, env_list);
                strcpy(&result[j], value);
                j += strlen(value);
            }
        } else {
            result[j++] = line[(*i)++];
        }
    }
    if (line[*i] == quote) (*i)++;
    result[j] = '\0';
    return result;
}

t_cmd *parse_command(char *line, t_env *env_list, int last_status) {
    t_cmd *head = NULL, *current = NULL;
    t_tokens **tokens = malloc(sizeof(t_tokens *) * 64);
    int token_count = 0;
    int i = 0;
    static int token_num = 0;

    while (line[i]) {
        while (line[i] == ' ') i++;

        if (!line[i] || line[i] == '|') {
            if (token_count > 0) {
                t_cmd *cmd = malloc(sizeof(t_cmd));
                cmd->args = malloc(sizeof(t_tokens *) * (token_count + 1));
                for (int j = 0; j < token_count; j++)
                    cmd->args[j] = tokens[j];
                cmd->args[token_count] = NULL;
                cmd->input_file = NULL;
                cmd->output_file = NULL;
                cmd->append = 0;
                cmd->next = NULL;
                if (!head) head = cmd;
                else current->next = cmd;
                current = cmd;
                token_count = 0;
            }
            if (line[i] == '|') {
                t_tokens *op = malloc(sizeof(t_tokens));
                op->token = strdup("|");
                op->token_num = token_num++;
                op->type = TOKEN_OPERATOR;
                op->next = NULL;
                tokens[token_count++] = op;
            }
            i++;
            continue;
        }

        if (line[i] == '<' || line[i] == '>') {
            int append = (line[i] == '>' && line[i + 1] == '>');
            if (append) i++;
            i++;
            while (line[i] == ' ') i++;
            char file[256];
            int j = 0;
            while (line[i] && line[i] != ' ' && line[i] != '|' && line[i] != '<' && line[i] != '>')
                file[j++] = line[i++];
            file[j] = '\0';
            if (!current) {
                current = malloc(sizeof(t_cmd));
                current->args = malloc(sizeof(t_tokens *));
                current->args[0] = NULL;
                current->input_file = NULL;
                current->output_file = NULL;
                current->append = 0;
                current->next = NULL;
                head = current;
            }
            if (line[i - j - (append ? 2 : 1)] == '<')
                current->input_file = strdup(file);
            else {
                current->output_file = strdup(file);
                current->append = append;
            }
            continue;
        }

        t_tokens *token = malloc(sizeof(t_tokens));
        token->token_num = token_num++;
        token->type = TOKEN_WORD;
        if (line[i] == '\'' || line[i] == '"') {
            token->type = (line[i] == '\'') ? TOKEN_SQUOTED : TOKEN_DQUOTED;
            token->token = parse_quotes(line, &i, line[i], env_list, last_status, &token->type);
        } else {
            char buffer[256];
            int j = 0;
            while (line[i] && line[i] != ' ' && line[i] != '|' && line[i] != '<' && line[i] != '>')
                buffer[j++] = line[i++];
            buffer[j] = '\0';
            if (buffer[0] == '$' && buffer[1]) token->type = TOKEN_VARIABLE;
            token->token = strdup(buffer);
        }
        token->next = NULL;
        tokens[token_count++] = token;
    }

    if (token_count > 0) {
        t_cmd *cmd = malloc(sizeof(t_cmd));
        cmd->args = malloc(sizeof(t_tokens *) * (token_count + 1));
        for (int j = 0; j < token_count; j++)
            cmd->args[j] = tokens[j];
        cmd->args[token_count] = NULL;
        cmd->input_file = NULL;
        cmd->output_file = NULL;
        cmd->append = 0;
        cmd->next = NULL;
        if (!head) head = cmd;
        else current->next = cmd;
    }

    free(tokens);
    return head;
}

int builtin_echo(t_cmd *cmd, t_env **env_list) {
    char **args = tokens_to_args(cmd->args);
    if (!args) return 1;
    int no_newline = 0, i = 1;
    if (args[1] && strcmp(args[1], "-n") == 0) {
        no_newline = 1;
        i++;
    }
    while (args[i]) {
        printf("%s", args[i]);
        if (args[i + 1]) printf(" ");
        i++;
    }
    if (!no_newline) printf("\n");
    free(args);
    return 0;
}

int builtin_cd(t_cmd *cmd, t_env **env_list) {
    char **args = tokens_to_args(cmd->args);
    if (!args) return 1;
    char *path = args[1] ? args[1] : get_env_value("HOME", *env_list);
    int status = 0;
    if (chdir(path) != 0) {
        perror("cd");
        status = 1;
    }
    free(args);
    return status;
}

int builtin_pwd(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))) {
        printf("%s\n", cwd);
        return 0;
    }
    perror("pwd");
    return 1;
}

int builtin_export(t_cmd *cmd, t_env **env_list) {
    char **args = tokens_to_args(cmd->args);
    if (!args) return 1;
    if (!args[1]) {
        for (t_env *cur = *env_list; cur; cur = cur->next)
            printf("export %s=%s\n", cur->key, cur->value);
    } else {
        char *eq = strchr(args[1], '=');
        if (eq) {
            *eq = '\0';
            t_env *node = create_env_node(args[1], eq + 1);
            node->next = *env_list;
            *env_list = node;
        }
    }
    free(args);
    return 0;
}

int builtin_unset(t_cmd *cmd, t_env **env_list) {
    char **args = tokens_to_args(cmd->args);
    if (!args) return 1;
    for (int i = 1; args[i]; i++) {
        t_env *cur = *env_list, *prev = NULL;
        while (cur && strcmp(cur->key, args[i]) != 0) {
            prev = cur;
            cur = cur->next;
        }
        if (cur) {
            if (prev) prev->next = cur->next;
            else *env_list = cur->next;
            free(cur->key);
            free(cur->value);
            free(cur);
        }
    }
    free(args);
    return 0;
}

int builtin_env(t_env *env_list) {
    for (t_env *cur = env_list; cur; cur = cur->next)
        printf("%s=%s\n", cur->key, cur->value);
    return 0;
}

void builtin_exit(int status) {
    exit(status);
}

int is_builtin(t_cmd *cmd) {
    if (!cmd || !cmd->args || !cmd->args[0]) return 0;
    char *cmd_name = cmd->args[0]->token;
    return (strcmp(cmd_name, "echo") == 0 || strcmp(cmd_name, "cd") == 0 ||
            strcmp(cmd_name, "pwd") == 0 || strcmp(cmd_name, "export") == 0 ||
            strcmp(cmd_name, "unset") == 0 || strcmp(cmd_name, "env") == 0 ||
            strcmp(cmd_name, "exit") == 0);
}

int execute_builtin(t_cmd *cmd, t_env **env_list, int last_status) {
    if (!cmd || !cmd->args || !cmd->args[0]) return last_status;
    char *cmd_name = cmd->args[0]->token;
    if (strcmp(cmd_name, "echo") == 0) return builtin_echo(cmd, env_list);
    else if (strcmp(cmd_name, "cd") == 0) return builtin_cd(cmd, env_list);
    else if (strcmp(cmd_name, "pwd") == 0) return builtin_pwd();
    else if (strcmp(cmd_name, "export") == 0) return builtin_export(cmd, env_list);
    else if (strcmp(cmd_name, "unset") == 0) return builtin_unset(cmd, env_list);
    else if (strcmp(cmd_name, "env") == 0) return builtin_env(*env_list);
    else if (strcmp(cmd_name, "exit") == 0) {
        builtin_exit(last_status);
        return 0; // Unreachable
    }
    return last_status;
}

int execute_pipeline(t_cmd *cmd, t_env **env_list, int last_status) {
    int in_fd = STDIN_FILENO;
    int pipe_fd[2];
    pid_t pid;
    t_cmd *current = cmd;
    int status = last_status;

    while (current) {
        if (current->next)
            pipe(pipe_fd);

        pid = fork();
        if (pid == 0) {
            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (current->next) {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            if (current->input_file) {
                int fd = open(current->input_file, O_RDONLY);
                if (fd < 0) { perror(current->input_file); exit(1); }
                dup2(fd, STDIN_FILENO); close(fd);
            }
            if (current->output_file) {
                int fd = open(current->output_file, O_WRONLY | O_CREAT |
                              (current->append ? O_APPEND : O_TRUNC), 0644);
                if (fd < 0) { perror(current->output_file); exit(1); }
                dup2(fd, STDOUT_FILENO); close(fd);
            }

            if (is_builtin(current)) {
                int exit_status = execute_builtin(current, env_list, status);
                exit(exit_status);
            }

            char *path = find_executable(current->args[0]->token, *env_list);
            char **args = tokens_to_args(current->args);
            if (path) {
                execve(path, args, environ);
                perror("execve");
                free(path);
            }
            free_args(args);
            fprintf(stderr, "minishell: %s: command not found\n", current->args[0]->token);
            exit(127);
        }

        if (in_fd != STDIN_FILENO) close(in_fd);
        if (current->next) {
            close(pipe_fd[1]);
            in_fd = pipe_fd[0];
        }

        current = current->next;
    }

    int wstatus;
    while (wait(&wstatus) > 0) {
        if (WIFEXITED(wstatus))
            status = WEXITSTATUS(wstatus);
    }
    return status;
}

int main(int argc, char **argv, char **envp) {
    char *line;
    t_env *env_list = init_env(envp);
    int last_status = 0;

    init_signals();
    while (1) {
        line = readline("minishell$ ");
        if (!line) {
            printf("exit\n");
            break;
        }
        if (*line) {
            add_history(line);
            t_cmd *cmd = parse_command(line, env_list, last_status);
            if (cmd && !cmd->next && is_builtin(cmd))
                last_status = execute_builtin(cmd, &env_list, last_status);
            else if (cmd)
                last_status = execute_pipeline(cmd, &env_list, last_status);
            free_cmd(cmd);
        }
        free(line);
    }

    free_env(env_list);
    rl_clear_history();
    return last_status;
}