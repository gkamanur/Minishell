/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 15:47:30 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h> // For debugging

int builtin_unset(t_tokens *tokens, t_env **env_list)
{
    t_tokens *current_token;
    t_env    *current_env;
    t_env    *prev_env;
    t_env *temp_env = *env_list;

    // Debug: Confirm function is called
    printf("builtin_unset called\n");

    // Check for valid token list and environment list
    if (!tokens || !env_list)
    {
        fprintf(stderr, "builtin_unset: Invalid tokens or env_list\n");
        return 1;
    }

    // Debug: Print token list
    printf("Tokens:\n");
    t_tokens *temp_token = tokens;
    while (temp_token)
    {
        printf("  Token: %s\n", temp_token->token ? temp_token->token : "NULL");
        temp_token = temp_token->next;
    }

    // Start from the second token (skip "unset")
    current_token = tokens->next;

    while (current_token)
    {
        // Validate token content
        if (!current_token->token || !*current_token->token)
        {
            fprintf(stderr, "builtin_unset: Skipping invalid token\n");
            current_token = current_token->next;
            continue;
        }

        // Debug: Print environment list before unset
        printf("Environment before unsetting '%s':\n", current_token->token);
        
        while (temp_env)
        {
            printf("  %s=%s\n", temp_env->key, temp_env->value);
            temp_env = temp_env->next;
        }

        current_env = *env_list;
        prev_env = NULL;

        // Search for the environment variable
        while (current_env)
        {
            // Debug: Print comparison
            printf("Comparing: token='%s' with env_key='%s'\n",
                   current_token->token, current_env->key);
            if (current_env->key && strcmp(current_env->key, current_token->token) == 0)
            {
                // Found the variable to unset
                if (prev_env)
                    prev_env->next = current_env->next;
                else
                    *env_list = current_env->next;

                // Free the node's contents
                if (current_env->key)
                    free(current_env->key);
                if (current_env->value)
                    free(current_env->value);
                free(current_env);
                printf("Unset '%s' successfully\n", current_token->token);
                break;
            }
            prev_env = current_env;
            current_env = current_env->next;
        }
        current_token = current_token->next;
    }

    // Debug: Print environment list after unset
    printf("Environment after unset:\n");
    temp_env = *env_list;
    while (temp_env)
    {
        printf("  %s=%s\n", temp_env->key, temp_env->value);
        temp_env = temp_env->next;
    }

    return 0; // Success
}
