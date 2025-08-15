/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:41:08 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/15 14:47:17 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_check(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r' || c == '\a')
		return (1);
	else if (c == '>')
		return (2);
	else if (c == '<')
		return (3);
	else if (c == '$')
		return (4);
	else if (c == '|')
		return (5);
	else if ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (6);
	return (0);
}

int	is_operator(const char *str)
{
	const char	*operators[] = {
		"|", ">", "<", ">>", "<<", "&&", "||", NULL
	};
	int			i;

	i = 0;
	while (operators[i])
	{
		if (strcmp(str, operators[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	is_special_char(int c)
{
	const char	specials[] = {' ', '\t', '\n', '\v', '\f',
							'\r', '\a', '"', '\'', '$', '\0'};
	int			i;

	i = 0;
	while (specials[i])
	{
		if (c == specials[i])
			return (1);
		i++;
	}
	return (0);
}

int input_isspace(char *inp)
{
	int i;
	i = 0;
	while (inp[i] != 0)
	{
		if (!(ft_isspace(inp[i])))
			return (FALSE);
		i++;
	}
	return(TRUE);
}

// if (ft_check(str[i]) == 1)
// int input_isseparator(char *str, int i)
// {
//     if (str[i] == '\0')
//         return (TOKEN_END);
//     if ((ft_check(str[i]) == 3) && (ft_check(str[i + 1]) == 3))
//         return TOKEN_HEREDOC;
//     else if ((ft_check(str[i]) == 2) && (ft_check(str[i + 1]) == 2))
//         return (TOKEN_APPEND);
// 	if (((str[i] > 8 && str[i] < 14) || str[i] == 32))
//         return (TOKEN_SPACE);
//     else if (ft_check(str[i]) == 5)
//         return (TOKEN_PIPE);
//     else if (ft_check(str[i]) == 3)
//         return (TOKEN_INPUT);
//     else if (ft_check(str[i]) == 2)
//         return (TOKEN_OUTPUT);

//     return 0;
// }

int input_isseparator(char *str, int i)
{
	if (((str[i] > 8 && str[i] < 14) || str[i] == 32))
		return (TOKEN_SPACE);
	else if (str[i] == '|')
		return (TOKEN_PIPE);
	else if (str[i] == '<' && str[i + 1] == '<')
		return (TOKEN_HEREDOC);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (TOKEN_APPEND);
	else if (str[i] == '<')
		return (TOKEN_INPUT);
	else if (str[i] == '>')
		return (TOKEN_OUTPUT);
	else if (str[i] == '\0')
		return (TOKEN_END);
	else
		return (0);
}
void check_var(t_tokens **lst)
{
	int i;
	
	i = 0;
	while ((*lst)->str[i])
	{
		if (ft_check((*lst)->str[i]) == 4)
		{
			if ((*lst)->prev && (*lst)->prev->type == TOKEN_HEREDOC)
				break;
			(*lst)->type = TOKEN_VAR;
			return;
		}
		i++;
	}
}

int if_var(t_tokens **lst)
{
	t_tokens *temp;
	temp = *lst;
	if (temp->type == TOKEN_PIPE)
	{
		putstr_clr("SYNTAX ERROR : if_var", 2, RED);
		return(FALSE);
	}
	while (temp)
	{
		check_var(&temp);
		if (if_consec(&temp) == FALSE)
			return (FALSE);
		temp = temp->next;
	}
	return (TRUE);
}