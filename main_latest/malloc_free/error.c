/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:04:01 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/12 17:39:53 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int add_detailquotes(char *cmd)
{
    if (ft_strncmp(cmd, "export", 7) == 0 || ft_strncmp(cmd, "unset", 6) == 0)
        return (TRUE);
    return (FALSE);
}

int error_cmd(char *cmd, char *detail, char *err_msg, int num)
{
    char *msg;
    int det_quotes;
    
    det_quotes = add_detailquotes(cmd);
    msg = ft_strdup("Minishell: ");
    if (cmd != NULL)
    {
        msg = ft_strjoin(msg, cmd);
        msg = ft_strjoin(msg, ": ");
    }
    if (detail)
    {
        if (det_quotes)
            msg = ft_strjoin(msg, "`");
        msg = ft_strjoin(msg, detail);
        if (det_quotes)
            msg = ft_strjoin(msg, "'");
        msg = ft_strjoin(msg, ": ");
    }
    msg = ft_strjoin(msg, err_msg);
    putstr_clr(msg, 2, RED);
    free_ptr(msg);
    return(num);
}
void error_msg(const char *msg, const char *func_name)
{
	if (!msg || !func_name)
		return;
	putstr_clr("minishell: ", 2, RED);
	putstr_clr((char *)func_name, 2, YELLOW);
	putstr_clr(": ", 2, RED);
	putstr_clr((char *)msg, 2, RED);
	putstr_clr("\n", 2, RED);
}

int use_mssg(int ret)
{
    putstr_clr("run ./minishell", 2, YELLOW);
    return (ret);
}