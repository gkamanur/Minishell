/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:41:08 by gkamanur          #+#    #+#             */
/*   Updated: 2025/07/31 18:09:21 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_check(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r' || c == '\a')
		return (1);
	else if (c == '\'')
		return (2);
	else if (c == '"')
		return (3);
	else if (c == '$')
		return (4);
	else if (c == '|' || c == '>' || c == '<' || c == '&')
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
