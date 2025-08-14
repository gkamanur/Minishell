/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:52:07 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/13 16:47:57 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **Realloc_env(t_data *data, int size)
{
    char **new;
    int i;
    new = ft_calloc(size + 1, sizeof (*new));
    if (!new)
        return (NULL);
    i = 0;
    while (data->env[i] && i < size)
    {
        new[i] = ft_strdup(data->env[i]);
        free_ptr(data->env[i]);
        i++;
    }
    free(data->env);
    return (new);
}

int delenvvarr(t_data *data, int idx) 
{
    char **env = data->env;
    int count;

    if (idx >= env_count(env))
        return false;

    free_ptr(env[idx]);
    count = idx;
    while (env[count + 1]) {
        env[count] = ft_strdup(env[count + 1]);
        free_ptr(env[count + 1]);
        count++;
    }
    data->env = Realloc_env(data, count);
    return (data->env != NULL);
}

int set_envar(t_data *data, char *key, char *val)
{
    int ind;
    char *str;
    
    ind =  get_varindex_from_env(data->env, key);
    if (!val)
        val="";
    str = ft_strjoin("=", val);
    if (!str)
        return (0);
    if (ind != -1 && data->env[ind])
    {
        free_ptr(data->env[ind]);
        data->env[ind] = ft_strjoin(key, str);
    }
    else
    {
        ind = env_count(data->env);
        data->env = Realloc_env(data, ind + 1);
        if (!data->env)
            return (0);
        data->env[ind] = ft_strjoin(key, str);
    }
    free_ptr(str);
    return(1);
}
