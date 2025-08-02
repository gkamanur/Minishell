#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

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

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

// Signal handler for SIGINT
static void handle_sigint(int sig) {
    (void)sig;
    printf("\n");
    // Re-display prompt (simplified)
    printf("$ ");
    fflush(stdout);
}

// Initialize signal handlers
void init_signals(void) {
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT (Ctrl+\)
}

// Count nodes in t_tokens linked list
int lst_size(t_tokens *tokens) {
    int count = 0;
    while (tokens) {
        count++;
        tokens = tokens->next;
    }
    return count;
}

// Free t_tokens linked list
void free_tokens(t_tokens *tokens) {
    while (tokens) {
        t_tokens *tmp = tokens;
        tokens = tokens->next;
        free(tmp->token);
        free(tmp);
    }
}

// Simplified input parsing (tokenizes words and operators)
t_tokens *input_parse(char *input) {
    if (!input) return NULL;
    t_tokens *head = NULL, *current = NULL;
    char *ptr = input, *start;
    int token_num = 0;

    while (*ptr) {
        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        if (!*ptr) break;
        start = ptr;

        t_token_type type = TOKEN_WORD;
        if (*ptr == '|' || *ptr == '>' || *ptr == '<' || (*ptr == '>' && *(ptr+1) == '>')) {
            type = TOKEN_OPERATOR;
            ptr++;
            if (*ptr == '>') ptr++; // Handle >>
        } else {
            // Simplified: treat everything else as a word
            while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '|' && *ptr != '>' && *ptr != '<') ptr++;
        }

        // Create token
        t_tokens *node = malloc(sizeof(t_tokens));
        if (!node) {
            free_tokens(head);
            return NULL;
        }
        node->token = strndup(start, ptr - start);
        node->token_num = token_num++;
        node->type = type;
        node->next = NULL;

        if (!head) head = node;
        else current->next = node;
        current = node;
    }
    return head;
}

// Read input line (replaces Readline)
char *read_line(void) {
    char *line = NULL;
    size_t len = 0;
    printf("$ ");
    if (getline(&line, &len, stdin) == -1) {
        free(line);
        return NULL; // EOF or error
    }
    // Remove trailing newline
    if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
    return line;
}

char **tokens_to_args(t_tokens *tokens) {
    int count = lst_size(tokens);
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args) return NULL;

    t_tokens *current = tokens;
    for (int i = 0; i < count; i++) {
        args[i] = current->token;
        current = current->next;
    }
    args[count] = NULL; // NULL-terminate
    return args;
}

int builtin_echo(t_cmd *cmd, t_env **env_list) {
    (void)env_list;
    char **args = tokens_to_args(*cmd->args);
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
    char **args = tokens_to_args(*cmd->args);
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
    char **args = tokens_to_args(*cmd->args);
    if (!args) return 1;
    if (!args[1]) {
        for (t_env *cur = *env_list; cur; cur = cur->next)
            printf("export %s=%s\n", cur->key, cur->value);
    } else {
        char *eq = strchr(args[1], '=');
        if (eq) {
            *eq = '\0';
            t_env *node = create_env_node(args[1], eq + 1);
            if (!node) {
                free(args);
                return 1;
            }
            node->next = *env_list;
            *env_list = node;
        }
    }
    free(args);
    return 0;
}

int builtin_unset(t_cmd *cmd, t_env **env_list) {
    char **args = tokens_to_args(*cmd->args);
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
    if (!cmd || !cmd->args || !*cmd->args) return 0;
    char *cmd_name = (*cmd->args)->token;
    return (strcmp(cmd_name, "echo") == 0 || strcmp(cmd_name, "cd") == 0 ||
            strcmp(cmd_name, "pwd") == 0 || strcmp(cmd_name, "export") == 0 ||
            strcmp(cmd_name, "unset") == 0 || strcmp(cmd_name, "env") == 0 ||
            strcmp(cmd_name, "exit") == 0);
}

int execute_builtin(t_cmd *cmd, t_env **env_list, int last_status) {
    if (!cmd || !cmd->args || !*cmd->args) return last_status;
    char *cmd_name = (*cmd->args)->token;
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

t_env *create_env_node(const char *key, const char *value) {
    t_env *node = malloc(sizeof(t_env));
    if (!node) return NULL;
    node->key = strdup(key);
    node->value = strdup(value);
    if (!node->key || !node->value) {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }
    node->next = NULL;
    return node;
}

t_env *init_env(char **envp) {
    t_env *env_list = NULL;
    for (int i = 0; envp[i]; i++) {
        char *entry = strdup(envp[i]);
        if (!entry) continue;
        char *eq = strchr(entry, '=');
        if (eq) {
            *eq = '\0';
            t_env *node = create_env_node(entry, eq + 1);
            if (node) {
                node->next = env_list;
                env_list = node;
            }
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
    while (env_list) {
        t_env *tmp = env_list;
        env_list = env_list->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

t_cmd *token2cmd(t_tokens **tokens) {
    if (!tokens || !*tokens) return NULL;
    t_cmd *head = NULL, *current = NULL;
    t_tokens *tok = *tokens;
    int arg_count = 0;
    t_tokens **args = NULL;

    while (tok) {
        if (tok->type == TOKEN_OPERATOR && strcmp(tok->token, "|") == 0) {
            // Create command if there are arguments
            if (arg_count > 0) {
                t_cmd *cmd = malloc(sizeof(t_cmd));
                if (!cmd) {
                    free(args);
                    free_cmd(head);
                    return NULL;
                }
                cmd->args = malloc((arg_count + 1) * sizeof(t_tokens *));
                if (!cmd->args) {
                    free(cmd);
                    free(args);
                    free_cmd(head);
                    return NULL;
                }
                for (int i = 0; i < arg_count; i++) cmd->args[i] = args[i];
                cmd->args[arg_count] = NULL;
                cmd->input_file = NULL;
                cmd->output_file = NULL;
                cmd->append = 0;
                cmd->next = NULL;
                if (!head) head = cmd;
                else current->next = cmd;
                current = cmd;
                free(args);
                args = NULL;
                arg_count = 0;
            }
            tok = tok->next;
            continue;
        } else if (tok->type == TOKEN_OPERATOR && (strcmp(tok->token, ">") == 0 || strcmp(tok->token, ">>") == 0)) {
            if (tok->next && tok->next->type == TOKEN_WORD) {
                if (arg_count > 0) {
                    t_cmd *cmd = malloc(sizeof(t_cmd));
                    if (!cmd) {
                        free(args);
                        free_cmd(head);
                        return NULL;
                    }
                    cmd->args = malloc((arg_count + 1) * sizeof(t_tokens *));
                    if (!cmd->args) {
                        free(cmd);
                        free(args);
                        free_cmd(head);
                        return NULL;
                    }
                    for (int i = 0; i < arg_count; i++) cmd->args[i] = args[i];
                    cmd->args[arg_count] = NULL;
                    cmd->output_file = strdup(tok->next->token);
                    cmd->append = strcmp(tok->token, ">>") == 0;
                    cmd->input_file = NULL;
                    cmd->next = NULL;
                    if (!head) head = cmd;
                    else current->next = cmd;
                    current = cmd;
                    free(args);
                    args = NULL;
                    arg_count = 0;
                    tok = tok->next->next; // Skip operator and filename
                    continue;
                }
            }
        } else if (tok->type == TOKEN_OPERATOR && strcmp(tok->token, "<") == 0) {
            if (tok->next && tok->next->type == TOKEN_WORD) {
                if (arg_count > 0) {
                    t_cmd *cmd = malloc(sizeof(t_cmd));
                    if (!cmd) {
                        free(args);
                        free_cmd(head);
                        return NULL;
                    }
                    cmd->args = malloc((arg_count + 1) * sizeof(t_tokens *));
                    if (!cmd->args) {
                        free(cmd);
                        free(args);
                        free_cmd(head);
                        return NULL;
                    }
                    for (int i = 0; i < arg_count; i++) cmd->args[i] = args[i];
                    cmd->args[arg_count] = NULL;
                    cmd->input_file = strdup(tok->next->token);
                    cmd->output_file = NULL;
                    cmd->append = 0;
                    cmd->next = NULL;
                    if (!head) head = cmd;
                    else current->next = cmd;
                    current = cmd;
                    free(args);
                    args = NULL;
                    arg_count = 0;
                    tok = tok->next->next; // Skip operator and filename
                    continue;
                }
            }
        }

        // Add token to args
        args = realloc(args, (arg_count + 1) * sizeof(t_tokens *));
        if (!args) {
            free_cmd(head);
            return NULL;
        }
        args[arg_count++] = tok;
        tok = tok->next;
    }

    // Create final command if there are arguments
    if (arg_count > 0) {
        t_cmd *cmd = malloc(sizeof(t_cmd));
        if (!cmd) {
            free(args);
            free_cmd(head);
            return NULL;
        }
        cmd->args = malloc((arg_count + 1) * sizeof(t_tokens *));
        if (!cmd->args) {
            free(cmd);
            free(args);
            free_cmd(head);
            return NULL;
        }
        for (int i = 0; i < arg_count; i++) cmd->args[i] = args[i];
        cmd->args[arg_count] = NULL;
        cmd->input_file = NULL;
        cmd->output_file = NULL;
        cmd->append = 0;
        cmd->next = NULL;
        if (!head) head = cmd;
        else current->next = cmd;
        free(args);
    } else {
        free(args);
    }

    return head;
}

void free_cmd(t_cmd *cmd) {
    while (cmd) {
        t_cmd *tmp = cmd;
        cmd = cmd->next;
        free(tmp->args); // Note: tokens are freed separately
        free(tmp->input_file);
        free(tmp->output_file);
        free(tmp);
    }
}

int main(int ac, char **av, char **envp) {
    ((void)ac);
    ((void)av);
    char *inp_line;
    t_env *env_list = init_env(envp);
    int status = 0;
    t_tokens *args;
    t_cmd *cmd;

    init_signals();
    printf("Shell started\n");

    while (1) {
        inp_line = read_line();
        if (!inp_line) break; // Exit on EOF (Ctrl+D)
        if (strlen(inp_line) == 0) {
            free(inp_line);
            continue;
        }
        args = input_parse(inp_line);
        cmd = token2cmd(&args);
        if (cmd && !cmd->next && is_builtin(cmd))
            status = execute_builtin(cmd, &env_list, status);
        free_cmd(cmd);
        free_tokens(args);
        free(inp_line);
    }

    free_env(env_list);
    printf("Shell exited\n");
    return status;
}