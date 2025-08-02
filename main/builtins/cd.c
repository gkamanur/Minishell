/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 14:12:09 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_cd(t_tokens *tokens, t_env **env_list)
{
    char    **args;
    char    *path;
    int     status;

    args = tokens_to_args(tokens); // Malloc happens here
    if (!args)
        return (1);
    path = args[1] ? args[1] : get_env_value("HOME", *env_list);
    status = 0;
    if (chdir(path) != 0)
    {
        perror("cd");
        status = 1;
    }
    free(args); // The args array is freed here
    // The code then continues, and this is where the use-after-free likely happens
    return (status);
}