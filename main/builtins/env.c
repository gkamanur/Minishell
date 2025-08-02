/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 14:40:33 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int builtin_env(t_env *env_list)
// {
//     t_env *current_env;

//     if (!env_list)
//         return 1;

//     current_env = env_list;
//     while (current_env)
//     {
//         // Only print if the value exists.
//         // A variable can exist without a value, which env does not print.
//         if (current_env->value)
//             printf("%s=%s\n", current_env->key, current_env->value);
//         current_env = current_env->next;
//     }
//     return 0;
// }

int builtin_env(t_tokens *tokens, t_env **env_list)
{
    // Skip the "env" command token
    t_tokens *current_token = tokens->next;
    
    // Check for variable assignments
    while (current_token && strchr(current_token->token, '='))
    {
        // This is a simplified logic. You'll need to parse the key and value.
        // For example, using strdup and strchr
        //char *assignment = current_token->token;
        // ... (create temporary env node or modify current env)
        
        current_token = current_token->next;
    }
    
    // Check if there is a command to execute
    if (current_token)
    {
        // Execute the command with the modified environment
        // e.g., execve(...)
    }
    else
    {
        // No command, just print the current environment
        for (t_env *cur = *env_list; cur; cur = cur->next)
        {
            if (cur->value)
                printf("%s=%s\n", cur->key, cur->value);
        }
    }
    return 0;
}