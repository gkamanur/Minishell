/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:06:59 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/15 16:33:34 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_status_token(t_tokens **lst, char c)
{
	if (c == '\'' && (*lst)->status == DEFAULT)
		(*lst)->status = SQ;
	else if (c == '\'' && (*lst)->status == SQ)
		(*lst)->status = DEFAULT;
	else if (c == '"' && (*lst)->status == DEFAULT)
		(*lst)->status= DQ;
	else if (c == '"' && (*lst)->status== DQ)
		(*lst)->status = DEFAULT;
}
int next_char(char c)
{
    if (c == '$' || c == ' ' || c == '\0')
        return (1);
    return (0);
}

int exp_var(t_tokens *lst)
{
    int i;

    i = 0;
    while (lst->str[i])
    {
        check_status_token(&lst,lst->str[i]);
        if (lst->str[i] == '$' && !next_char(lst->str[i]) && )
    }
    
}
int expand(t_data *data, t_tokens **lst)
{
    t_tokens *curr;
    int i;
    
    curr = *lst;
    while (curr)
    {
        if (curr->type == TOKEN_VAR)
        {
            exp_var(curr);
        }
    }
}