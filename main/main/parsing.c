/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:03:24 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/01 12:32:14 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



void add_token(t_tokens **head, char *token, int token_num, t_token_type type)
{
    t_tokens *new_node = Malloc(sizeof(t_tokens));
    new_node->token = ft_strdup(token);
    new_node->token_num = token_num;
    new_node->type = type;
    new_node->next = NULL;

    if (*head == NULL)
        *head = new_node;
    else
    {
        t_tokens *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}

void skip_whitespace(char *input, size_t len, t_parse *ctx)
{
	while (ctx->i < len && (ft_check(input[ctx->i]) == 1))
		ctx->i++;
}

int handle_quote(char *input, size_t len, t_parse *ctx)
{
	char	quote;
	size_t	buf_pos;
	int flag;

	buf_pos = 0;
	flag = 0;
	quote = input[ctx->i++];
	while (ctx->i < len && input[ctx->i] != quote)
		ctx->buffer[buf_pos++] = input[ctx->i++];
	if (ctx->i < len && input[ctx->i] == quote)
	{
		ctx->i++;
		ctx->buffer[buf_pos] = '\0';
		return (1);
	}
	return (0);
}

static void handle_squoted_string(char *input, size_t len, t_parse *ctx)
{
	if (handle_quote(input, len, ctx))
		add_token(ctx->head, ctx->buffer, ctx->token_num++, TOKEN_SQUOTED);
	else
		putstr_clr("incomplete single quotes", 2, RED);
}

static void handle_dquoted_string(char *input, size_t len, t_parse *ctx)
{
	if (handle_quote(input, len, ctx))
		add_token(ctx->head, ctx->buffer, ctx->token_num++, TOKEN_DQUOTED);
	else
		putstr_clr("incomplete double quotes", 2, RED);
}



static void	handle_variable(char *input, size_t len, t_parse *ctx)
{
	size_t	buf_pos;

	buf_pos = 0;
	ctx->buffer[buf_pos++] = input[ctx->i++];
	while (ctx->i < len && (ft_check(input[ctx->i]) == 5 || input[ctx->i] == '_'))
		ctx->buffer[buf_pos++] = input[ctx->i++];
	ctx->buffer[buf_pos] = '\0';
	add_token(ctx->head, ctx->buffer, ctx->token_num++, TOKEN_VARIABLE);
}

static void	handle_operator_or_word(char *input, size_t len, t_parse *ctx)
{
	size_t	buf_pos;
	size_t	buffer_size;

	buf_pos = 0;
	buffer_size = BUFSIZEE;
	if (ctx->i + 1 < len && (ft_check(input[ctx->i])  == 5)
		&& input[ctx->i] == input[ctx->i + 1])
	{
		ctx->buffer[buf_pos++] = input[ctx->i++];
		ctx->buffer[buf_pos++] = input[ctx->i++];
	}
	else if (is_operator(&input[ctx->i]))
	{
		ctx->buffer[buf_pos++] = input[ctx->i++];
	}
	else
	{
		while (ctx->i < len && !is_special_char(input[ctx->i]) && !is_operator(&input[ctx->i]))
		{
			if (buf_pos + 1 >= buffer_size)
			{
				buffer_size *= 2;
				ctx->buffer = Realloc(ctx->buffer, buffer_size);
			}
			ctx->buffer[buf_pos++] = input[ctx->i++];
		}
	}
	ctx->buffer[buf_pos] = '\0';
	if (buf_pos > 0)
		add_token(ctx->head, ctx->buffer, ctx->token_num++,
			is_operator(ctx->buffer) ? TOKEN_OPERATOR : TOKEN_WORD);
}


t_tokens	*input_parse(char *input)
{
	t_parse	ctx;
	t_tokens	*head;
	char		*buffer;
	size_t		len;

	if (!input)
		return (NULL);
	len = strlen(input);
	buffer = Malloc(BUFSIZEE);
	head = NULL;
	ctx.head = &head;
	ctx.buffer = buffer;
	ctx.i = 0;
	ctx.token_num = 0;
	while (ctx.i < len)
	{
		skip_whitespace(input, len, &ctx);
		if (ctx.i >= len)
			break ;
		if (ft_check(input[ctx.i]) == 2)
			handle_squoted_string(input, len, &ctx);
		else if (ft_check(input[ctx.i]) == 3)
			handle_dquoted_string(input, len, &ctx);
		else if (ft_check(input[ctx.i]) == 4)
			handle_variable(input, len, &ctx);
		else
			handle_operator_or_word(input, len, &ctx);
	}
	free(buffer);
	return (head);
}

void free_tokens(t_tokens *head)
{
    t_tokens *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->token);
        free(temp);
    }
}