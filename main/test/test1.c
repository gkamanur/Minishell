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
# define DEFAULT_SOURCE
#include <signal.h>

extern char **environ;

volatile sig_atomic_t g_signal_received = 0;

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_cmd {
    char **args;
    char *input_file;
    char *output_file;
    int append;
    struct s_cmd *next;
} t_cmd;

t_env *g_env_list = NULL;
int g_last_exit_status = 0;

void signal_handler(int sig) {
    g_signal_received = sig;
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

void init_env(char **envp) {
    for (int i = 0; envp[i]; i++) {
        char *entry = strdup(envp[i]);
        char *eq = strchr(entry, '=');
        if (eq) {
            *eq = '\0';
            t_env *node = create_env_node(entry, eq + 1);
            node->next = g_env_list;
            g_env_list = node;
        }
        free(entry);
    }
}

char *get_env_value(const char *key) {
    t_env *current = g_env_list;
    while (current) {
        if (strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return "";
}

void free_env(void) {
    t_env *current = g_env_list;
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

void free_cmd(t_cmd *cmd) {
    while (cmd) {
        t_cmd *tmp = cmd;
        cmd = cmd->next;
        free_args(tmp->args);
        free(tmp->input_file);
        free(tmp->output_file);
        free(tmp);
    }
}

char *find_executable(char *cmd) {
    if (strchr(cmd, '/'))
        return access(cmd, X_OK) == 0 ? strdup(cmd) : NULL;

    char *path = get_env_value("PATH");
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

char *parse_quotes(char *line, int *i, char quote) {
    char *result = malloc(1024);
    int j = 0;
    (*i)++;
    while (line[*i] && line[*i] != quote) {
        if (quote == '"' && line[*i] == '$' && line[*i + 1]) {
            (*i)++;
            if (line[*i] == '?') {
                char status[16];
                sprintf(status, "%d", g_last_exit_status);
                strcpy(&result[j], status);
                j += strlen(status);
                (*i)++;
            } else {
                char key[64] = {0};
                int k = 0;
                while (line[*i] && (isalnum(line[*i]) || line[*i] == '_'))
                    key[k++] = line[(*i)++];
                char *value = get_env_value(key);
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

t_cmd *parse_command(char *line) {
    t_cmd *head = NULL, *current = NULL;
    char **args = malloc(sizeof(char *) * 64);
    int arg_count = 0;
    int i = 0;

    while (line[i]) {
        while (line[i] == ' ') i++;

        if (!line[i] || line[i] == '|') {
            if (arg_count > 0) {
                t_cmd *cmd = malloc(sizeof(t_cmd));
                cmd->args = malloc(sizeof(char *) * (arg_count + 1));
                for (int j = 0; j < arg_count; j++)
                    cmd->args[j] = args[j];
                cmd->args[arg_count] = NULL;
                cmd->input_file = NULL;
                cmd->output_file = NULL;
                cmd->append = 0;
                cmd->next = NULL;
                if (!head) head = cmd;
                else current->next = cmd;
                current = cmd;
                arg_count = 0;
            }
            if (line[i] == '|') i++;
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
                current->args = malloc(sizeof(char *));
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

        if (line[i] == '\'' || line[i] == '"') {
            args[arg_count++] = parse_quotes(line, &i, line[i]);
        } else {
            char buffer[256];
            int j = 0;
            while (line[i] && line[i] != ' ' && line[i] != '|' && line[i] != '<' && line[i] != '>')
                buffer[j++] = line[i++];
            buffer[j] = '\0';
            args[arg_count++] = strdup(buffer);
        }
    }

    if (arg_count > 0) {
        t_cmd *cmd = malloc(sizeof(t_cmd));
        cmd->args = malloc(sizeof(char *) * (arg_count + 1));
        for (int j = 0; j < arg_count; j++)
            cmd->args[j] = args[j];
        cmd->args[arg_count] = NULL;
        cmd->input_file = NULL;
        cmd->output_file = NULL;
        cmd->append = 0;
        cmd->next = NULL;
        if (!head) head = cmd;
        else current->next = cmd;
    }

    free(args);
    return head;
}

void builtin_echo(char **args) {
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
    g_last_exit_status = 0;
}

void builtin_cd(char **args) {
    char *path = args[1] ? args[1] : get_env_value("HOME");
    if (chdir(path) != 0)
        perror("cd");
    else
        g_last_exit_status = 0;
}

void builtin_pwd(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
        perror("pwd");
    g_last_exit_status = 0;
}

void builtin_export(char **args) {
    if (!args[1]) {
        for (t_env *cur = g_env_list; cur; cur = cur->next)
            printf("export %s=%s\n", cur->key, cur->value);
    } else {
        char *eq = strchr(args[1], '=');
        if (eq) {
            *eq = '\0';
            t_env *node = create_env_node(args[1], eq + 1);
            node->next = g_env_list;
            g_env_list = node;
        }
    }
    g_last_exit_status = 0;
}

void builtin_unset(char **args) {
    for (int i = 1; args[i]; i++) {
        t_env *cur = g_env_list, *prev = NULL;
        while (cur && strcmp(cur->key, args[i]) != 0) {
            prev = cur;
            cur = cur->next;
        }
        if (cur) {
            if (prev) prev->next = cur->next;
            else g_env_list = cur->next;
            free(cur->key);
            free(cur->value);
            free(cur);
        }
    }
    g_last_exit_status = 0;
}

void builtin_env(void) {
    for (t_env *cur = g_env_list; cur; cur = cur->next)
        printf("%s=%s\n", cur->key, cur->value);
    g_last_exit_status = 0;
}

void builtin_exit(void) {
    exit(g_last_exit_status);
}

int is_builtin(char *cmd) {
    return (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0 ||
            strcmp(cmd, "pwd") == 0 || strcmp(cmd, "export") == 0 ||
            strcmp(cmd, "unset") == 0 || strcmp(cmd, "env") == 0 ||
            strcmp(cmd, "exit") == 0);
}

void execute_builtin(char **args) {
    if (!args[0]) return;
    if (strcmp(args[0], "echo") == 0) builtin_echo(args);
    else if (strcmp(args[0], "cd") == 0) builtin_cd(args);
    else if (strcmp(args[0], "pwd") == 0) builtin_pwd();
    else if (strcmp(args[0], "export") == 0) builtin_export(args);
    else if (strcmp(args[0], "unset") == 0) builtin_unset(args);
    else if (strcmp(args[0], "env") == 0) builtin_env();
    else if (strcmp(args[0], "exit") == 0) builtin_exit();
}

void execute_pipeline(t_cmd *cmd) {
    int in_fd = STDIN_FILENO;
    int pipe_fd[2];
    pid_t pid;
    t_cmd *current = cmd;

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

            if (is_builtin(current->args[0])) {
                execute_builtin(current->args);
                exit(g_last_exit_status);
            }

            char *path = find_executable(current->args[0]);
            if (path) {
                execve(path, current->args, environ);
                perror("execve");
                free(path);
            }
            fprintf(stderr, "minishell: %s: command not found\n", current->args[0]);
            exit(127);
        }

        if (in_fd != STDIN_FILENO) close(in_fd);
        if (current->next) {
            close(pipe_fd[1]);
            in_fd = pipe_fd[0];
        }

        current = current->next;
    }

    while (wait(NULL) > 0);
}

int main(int argc, char **argv, char **envp) {
    char *line;
    init_signals();
    init_env(envp);

    while (1) {
        line = readline("minishell$ ");
        if (!line) {
            printf("exit\n");
            break;
        }
        if (*line) {
            add_history(line);
            t_cmd *cmd = parse_command(line);
            if (cmd && !cmd->next && is_builtin(cmd->args[0]))
                execute_builtin(cmd->args);
            else if (cmd)
                execute_pipeline(cmd);
            free_cmd(cmd);
        }
        free(line);
    }

    free_env();
    rl_clear_history();
    return g_last_exit_status;
}