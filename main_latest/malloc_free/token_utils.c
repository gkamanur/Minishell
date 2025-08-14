/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:34:17 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/12 13:36:28 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void delete_token(t_tokens *lst, void (*del)(void *))
{
    if (!lst || !del)
        return;

    if (lst->str)
        del(lst->str);
    if (lst->str_bkp)
        del(lst->str_bkp);

    if (lst->prev)
        lst->prev->next = lst->next;
    if (lst->next)
        lst->next->prev = lst->prev;

    free_ptr(lst);
}

void	clear_token(t_tokens **lst, void (*del)(void *))
{
	t_tokens	*tmp;

	tmp = NULL;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		delete_token(*lst, del);
		*lst = tmp;
	}
}

void	add_token(t_tokens **alst, t_tokens *new_node)
{
	t_tokens	*start;

	start = *alst;
	if (start == NULL)
	{
		*alst = new_node;
		return ;
	}
	if (alst && *alst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
}

t_tokens	*new_token(char *str, char *str_backup, int type, int status)
{
	t_tokens	*new_node;

	new_node = malloc(sizeof(t_tokens) * 1);
	if (!(new_node))
		return (NULL);
	new_node->str = str;
	new_node->str_bkp = str_backup;
	new_node->is_var = false;
	new_node->type = type;
	new_node->status = status;
	new_node->join = false;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}
