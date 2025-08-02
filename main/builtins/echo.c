/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 14:13:40 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_echo(t_tokens *tokens)
{
    char    **args;
    int     no_newline = 0;
    int     i = 1;

    // The function receives a single pointer to the head of the list
    args = tokens_to_args(tokens);
    if (!args)
        return (1);

    if (args[1] && strcmp(args[1], "-n") == 0)
    {
        no_newline = 1;
        i++;
    }

    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    if (!no_newline)
        printf("\n");

    free(args); // Don't free the strings, just the array itself
    return (0);
}