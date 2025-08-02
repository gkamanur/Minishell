/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 14:41:54 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **tokens_to_args(t_tokens *tokens_head)
{
    t_tokens *current = tokens_head;
    int count = 0;
    char **args;
    int i = 0;

    if (!tokens_head)
        return (NULL);

    // Count the number of tokens
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    // Allocate memory for the argument array
    args = malloc((count + 1) * sizeof(char *));
    if (!args)
        return (NULL);

    // Populate the array with the token strings
    current = tokens_head;
    while (current != NULL)
    {
        args[i] = current->token;
        current = current->next;
        i++;
    }
    args[count] = NULL; // NULL-terminate the array
    return (args);
}


int is_builtin(t_tokens **tokens) {
    // Check for NULL pointers to prevent crashes.
    if (!tokens || !*tokens || !(*tokens)->token) return 0;
    
    char *cmd_name = (*tokens)->token;
    // Debug: printf("Checking builtin: %s\n", cmd_name);
    return (strcmp(cmd_name, "echo") == 0 || strcmp(cmd_name, "cd") == 0 ||
            strcmp(cmd_name, "pwd") == 0 || strcmp(cmd_name, "export") == 0 ||
            strcmp(cmd_name, "unset") == 0 || strcmp(cmd_name, "env") == 0 ||
            strcmp(cmd_name, "exit") == 0);
}
// --- `builtin_unset`, `builtin_env`, `builtin_exit` and `is_builtin` are fine ---

// --- Corrected `execute_builtin` Function ---
int execute_builtin(t_tokens *tokens, t_env **env_list, int last_status)
{
    if (!tokens || !tokens->token)
        return (last_status);

    char *cmd_name = tokens->token;

    if (strcmp(cmd_name, "echo") == 0)
        return (builtin_echo(tokens));
    else if (strcmp(cmd_name, "cd") == 0)
        return (builtin_cd(tokens, env_list));
    else if (strcmp(cmd_name, "pwd") == 0)
        return (builtin_pwd());
    else if (strcmp(cmd_name, "export") == 0)
        return (builtin_export(tokens, env_list));
    else if (strcmp(cmd_name, "unset") == 0)
        return (builtin_unset(tokens, env_list));
    else if (strcmp(cmd_name, "env") == 0)
        return (builtin_env(tokens, env_list));
    else if (strcmp(cmd_name, "exit") == 0)
    {
        // Exit builtin should handle arguments
        // Here, we just call exit with the last status
        builtin_exit(last_status);
        return (0);
    }
    return (last_status);
}