/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 14:12:36 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_export(t_tokens *tokens, t_env **env_list)
{
    char    **args;
    char    *eq;
    t_env   *node;
    int     i = 1;

    args = tokens_to_args(tokens);
    if (!args)
        return (1);
    
    if (!args[1])
    {
        // No arguments: print the environment
        for (t_env *cur = *env_list; cur; cur = cur->next)
            printf("export %s=\"%s\"\n", cur->key, cur->value);
    }
    else
    {
        // Arguments provided: add/update variables
        while (args[i])
        {
            eq = strchr(args[i], '=');
            if (eq)
            {
                // Temporarily split the string
                *eq = '\0';
                
                // Check if the variable already exists and update it
                t_env *cur = *env_list;
                while (cur)
                {
                    if (strcmp(cur->key, args[i]) == 0)
                    {
                        free(cur->value);
                        cur->value = strdup(eq + 1);
                        *eq = '='; // Restore the string
                        break;
                    }
                    cur = cur->next;
                }

                // If not found, create a new node
                if (!cur)
                {
                    node = create_env_node(strdup(args[i]), strdup(eq + 1));
                    if (!node)
                    {
                        free(args);
                        *eq = '='; // Restore the string before returning
                        return (1);
                    }
                    node->next = *env_list;
                    *env_list = node;
                }
                *eq = '='; // Restore the string
            }
            else
            {
                // Handle cases like 'export MYVAR' without a value
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
                // Depending on the desired behavior, you might return 1 here or continue
            }
            i++;
        }
    }
    free(args);
    return (0);
}