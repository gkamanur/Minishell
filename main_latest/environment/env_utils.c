/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:00:21 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/13 16:41:37 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int env_count(char **av)
{
    int i;
    i = 0;
    while (av && av[i])
        i++;
    return (i);
}

int get_varindex_from_env(char **env, char *var)
{
    int i;
    char *str;
    
    str = ft_strjoin(var, "=");
    if (!str)
        return (-1);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(str, env[i], ft_strlen(str)) == 0)
        {
            free_ptr(str);
            return (i);
        }
        i++;
    }
    free_ptr(str);
    return (-1);
}

char *get_varvalue_from_env(char **env, char *var)
{
    int i;
    char *str;
    
    str = ft_strjoin(var, "=");
    if (!str)
        return (NULL);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(str, env[i], ft_strlen(str)) == 0)
        {
            free_ptr(str);
            return (ft_strchr(env[i], '=') + 1);
        }
        i++;
    }
    free_ptr(str);
    return (NULL);
}

int env_var_valid(char *str)
{
    int i;

    i = 0;
    if (ft_isalpha(str[i]) == 0 && str[i] != '_')
        return (0);
    i++;
    while (str[i] && str[i] != '=')
    {
        if (ft_isalnum(str[i]) == 0 && str[i] != '_')
            return(0);
        i++;
    }
    return (1);
}