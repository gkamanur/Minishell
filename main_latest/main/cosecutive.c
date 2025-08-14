/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cosecutive.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:07:27 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/13 16:49:51 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* this pair of functions is checking for syntax errors caused by consecutive operators in your token list 
(things like ||, &&, >>>, |<, etc.).*/

// 1. PIPE followed by PIPE (e.g., "| |")
// 2. Two operators in a row (type > PIPE means operator like <, >, <<, >>)
// 3. END token right after an operator (unfinished command)



int check_consec(t_tokens *node)
{
    if (node->prev)
    {
        if (node->type == TOKEN_PIPE && node->prev->type == TOKEN_PIPE)
            return (TRUE);
        if (node->type > TOKEN_PIPE && node->prev->type == TOKEN_PIPE)
            return (TRUE);
        if (node->type == TOKEN_END && node->prev->type >= TOKEN_PIPE)
            return (TRUE);
    }
    return (FALSE);
}

int if_consec(t_tokens **lst)
{
    t_tokens *tmp;
    
    tmp = *lst;
    while (tmp)
    {
        if (check_consec(tmp) == TRUE)
        {
            if (tmp->type == TOKEN_END && tmp->prev && tmp->prev->type > TOKEN_PIPE)
                putstr_clr("SYNTAX ERROR: consecutive1\n",2, RED);
            else if (tmp->type == TOKEN_END && tmp->prev)
            {
                putstr_clr("SYNTAX ERROR: consecutive2",2, RED);
                putstr_clr(tmp->prev->str, 2, RED);
                write (2, "\n", 1);
            }
            else
            {
                putstr_clr("SYNTAX ERROR: consecutive3",2, RED);
                putstr_clr(tmp->str, 2, RED);
                write (2, "\n", 1);
            }
        }
        tmp = tmp->next;
    }
    return (TRUE);
}