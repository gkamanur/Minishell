/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:22:47 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/13 13:48:12 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_array(char **av)
{
    int i;
    i = 0;

    if (!av)
        return ;
    while (av[i])
    {
        if (av[i])
        {
            free_ptr(av[i]);
            av[i] = NULL;
        }
        i++;
    }
    free (av);
    av = NULL;
}

void free_data(t_data *data, int clear_history)
{
    if (!data)
        return;

    free_ptr(data->input_raw);
    data->input_raw = NULL;

    if (data->token)
        clear_token(&data->token, free_ptr);
    if (data->cmd)
        lstclear_cmd(&data->cmd, free_ptr);

    if (clear_history)
    {
        free_ptr(data->pwd);
        free_ptr(data->owd);
        free_array(data->env);
        rl_clear_history();
    }
}

void free_ptr(void *p)
{
    if (p)
    {
        free(p);
        p = NULL;
    }
}

void free_file(t_file *file)
{
    if (!file)
        return;

    fd_restore(file);

    if (file->heredoc_delim)
        unlink(file->infile);

    free_ptr(file->heredoc_delim);
    free_ptr(file->infile);
    free_ptr(file->outfile);

    free_ptr(file);
}

void free_token(t_tokens **lst, void (*del)(void *))
{
    t_tokens *tmp;

    if (!lst || !del)
        return;

    while (*lst)
    {
        tmp = (*lst)->next;
        delete_token(*lst, del);
        *lst = tmp;
    }
}

