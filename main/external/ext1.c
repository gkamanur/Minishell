/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:53:14 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 15:53:29 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

// Helper to convert env_list to char ** for execve
char **env_list_to_array(t_env *env_list)
{
    int count = 0;
    t_env *current = env_list;
    while (current)
    {
        if (current->key && current->value)
            count++;
        current = current->next;
    }

    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;

    current = env_list;
    int i = 0;
    while (current)
    {
        if (current->key && current->value)
        {
            envp[i] = malloc(strlen(current->key) + strlen(current->value) + 2);
            sprintf(envp[i], "%s=%s", current->key, current->value);
            i++;
        }
        current = current->next;
    }
    envp[i] = NULL;
    return envp;
}

// Helper to free envp array
void free_envp(char **envp)
{
    for (int i = 0; envp[i]; i++)
        free(envp[i]);
    free(envp);
}

// Helper to convert tokens to char ** for execve
char **tokens_to_array(t_tokens *tokens)
{
    int count = 0;
    t_tokens *current = tokens;
    while (current)
    {
        count++;
        current = current->next;
    }

    char **args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;

    current = tokens;
    int i = 0;
    while (current)
    {
        args[i] = current->token; // Assume token is already allocated
        i++;
        current = current->next;
    }
    args[i] = NULL;
    return args;
}

int execute_external(t_tokens *tokens, t_env *env_list)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    if (pid == 0) // Child process
    {
        char **args = tokens_to_array(tokens);
        char **envp = env_list_to_array(env_list);
        char *path;

        // Find full path for command (e.g., /bin/ls)
        path = find_command_path(tokens->token, env_list); // Implement this
        if (!path)
        {
            fprintf(stderr, "minishell: %s: command not found\n", tokens->token);
            free(args);
            free_envp(envp);
            exit(127);
        }

        execve(path, args, envp);
        perror("execve"); // Only reached if execve fails
        free(path);
        free(args);
        free_envp(envp);
        exit(1);
    }
    else // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        return 1;
    }
}

// Helper to find command in PATH (implement in helper/fileX.c)
char *find_command_path(const char *cmd, t_env *env_list)
{
    char *path = NULL;
    t_env *current = env_list;
    while (current)
    {
        if (strcmp(current->key, "PATH") == 0)
        {
            path = current->value;
            break;
        }
        current = current->next;
    }
    if (!path)
        return NULL;

    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    char full_path[1024];

    while (dir)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return strdup(full_path);
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}