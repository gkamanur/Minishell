/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:17:25 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/14 10:34:22 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_status(int status, char *str, int i)
{
	if (str[i] == '\'' && status == DEFAULT)
		status = SQ;
	else if (str[i] == '\'' && status == SQ)
		status = DEFAULT;
	else if (str[i] == '"' && status == DEFAULT)
		status = DQ;
	else if (str[i] == '"' && status == DQ)
		status = DEFAULT;
	return (status);
}
// int	input_isseparator(char *str, int i)
// {
// 	if (str[i] == '\0')
// 		return (TOKEN_END);
// 	if ((ft_check(str[i]) == 3) && (ft_check(str[i + 1]) == 3))
// 		return (TOKEN_HEREDOC);
// 	else if ((ft_check(str[i]) == 2) && (ft_check(str[i + 1]) == 2))
// 		return (TOKEN_APPEND);
// 	if (ft_check(str[i]) == 1)
// 		return (TOKEN_SPACE);
// 	else if (ft_check(str[i]) == 5)
// 		return (TOKEN_PIPE);
// 	else if (ft_check(str[i]) == 3)
// 		return (TOKEN_INPUT);
// 	else if (ft_check(str[i]) == 2)
// 		return (TOKEN_OUTPUT);
// 	return (0);
// }
int	if_word(t_tokens **token, char *str, int ind, int start)
{
	int		i;
	char	*word;

	i = 0;
	word = malloc(sizeof(char) * (ind - start + 1));
	while (start < ind)
	{
		word[i] = str[start];
		start++;
		i++;
	}
	word[i] = 0;
	add_token(token, new_token(word, ft_strdup(word), TOKEN_WORD, DEFAULT));
	return (0);
}
int	if_separator(t_tokens **lst, char *str, int type, int ind)
{
	int		i;
	char	*sep;

	i = 0;
	if (type == TOKEN_APPEND || type == TOKEN_HEREDOC)
	{
		sep = malloc(sizeof(char) * 3);
		while (i < 2)
			sep[i++] = str[ind++];
		sep[i] = 0;
		add_token(lst, new_token(sep, NULL, type, DEFAULT));
	}
	else
	{
		sep = malloc(sizeof(char) * 2);
		while (i < 1)
			sep[i++] = str[ind++];
		sep[i] = 0;
		add_token(lst, new_token(sep, NULL, type, DEFAULT));
	}
	return (0);
}

int	word_or_sep(int *i, char *str, int start, t_data *data)
{
	int	type;

	type = input_isseparator(str, (*i));
	if (type)
	{
		if ((*i) != 0 && (input_isseparator(str, ((*i) - 1)) == 0))
			if_word(&data->token, str, (*i), start);
		if (type == TOKEN_APPEND || type == TOKEN_HEREDOC || type == TOKEN_INPUT
			|| type == TOKEN_PIPE || type == TOKEN_OUTPUT || type == TOKEN_END)
		{
			if_separator(&data->token, str, (*i), type);
			if (type == TOKEN_APPEND || type == TOKEN_HEREDOC)
				(*i)++;
		}
		start = (*i) + 1;
	}
	return (start);
}

int	make_tokens(t_data *data, char *str)
{
	int i = 0;
	int start;
	int end;
	int status;

	start = 0;
	end = ft_strlen(str);
	status = DEFAULT;
	while (i <= end)
	{
		status = check_status(status, str, i);
		if (status == DEFAULT)
			start = word_or_sep(&i, str, start, data);
	}
	if (status != DEFAULT)
	{
		if (status == SQ)
			putstr_clr ("incomplete single quotes", 2, RED);
		else if(status == DQ) 
            putstr_clr ("incomplete double quotes", 2, RED);
		else 
            putstr_clr("SYNTAX ERROR", 2, RED);
            
        return(FALSE);
	}
    return (TRUE);
}