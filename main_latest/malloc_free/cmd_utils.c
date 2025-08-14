/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:38:52 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/12 19:26:13 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_cmd(t_cmd **cmd)
{
    t_cmd *curr;

    curr = *cmd;
    curr->cmd = NULL;
    curr->path = NULL;
    curr->args = NULL;
    curr->pipe_out = 0;
    curr->pipe_fd = 0;
    curr->prev = NULL;
    curr->next = NULL;
}

t_cmd *new_cmd(int val)
{
    t_cmd *new;
    
    new = Malloc (sizeof(t_cmd));
    ft_memset(new, 0, sizeof(t_cmd));
    new->pipe_out = val;
    init_cmd(&new);
    return (new);
}

void add_cmd(t_cmd **lst, t_cmd *new_node)
{
    t_cmd *start;
    
    start = *lst;
    if (!start)
    {
        *lst = new_node;
        return ;
    }
    if (lst && *lst && new_node)
    {
        while (start->next)
            start = start->next;
        start->next = new_node;
        new_node->prev = start;
    }
}

t_cmd *last_cmd(t_cmd *cmd)
{
    while(cmd->next)
        cmd = cmd->next;
    return (cmd);
}

void del_node_cmd(t_cmd *lst, void (*del)(void *))
{
    if (lst->cmd)
        (*del)(lst->cmd);
    if (lst->args)
        free_array(lst->args);
    if (lst->pipe_fd)
        (*del)(lst->pipe_fd);
    if (lst->files)
        free_file(lst->files);
    (*del)(lst);
}

void lstclear_cmd(t_cmd **lst, void (*del)(void *))
{
    t_cmd *curr;
    
    curr = NULL;
    while (*lst)
    {
        curr = (*lst)->next;
        del_node_cmd(*lst, del);
        *lst = curr;
    } 
}