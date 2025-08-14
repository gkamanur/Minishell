/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:00:50 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/14 16:44:55 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



int	parse_input(t_data *data)
{
	if (data->input_raw == NULL)
		builtin_exit(data, NULL);
	else if(ft_strcmp(data->input_raw, "\0") == 0) 
		return (0);
	else if(input_isspace(data->input_raw) == TRUE) 
		return (TRUE);
    add_history(data->input_raw);
    if (make_tokens(data,data->input_raw) == FALSE)
		return (FALSE);
	if (data->token->type == TOKEN_END)
		return (FALSE);
	if (if_var(&data->token) == FALSE)
		return(FALSE);
	print_tokens(data->token);
	return (1);
}