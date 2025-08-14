/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:47:58 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/14 13:50:11 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static const char *get_token_type_name(int type)
{
	switch (type)
	{
		case TOKEN_SPACE: return "SPACES";
		case TOKEN_WORD: return "WORD";
		case TOKEN_VAR: return "VAR";
		case TOKEN_PIPE: return "PIPE";
		case TOKEN_INPUT: return "INPUT";
		case TOKEN_OUTPUT: return "TRUNC";
		case TOKEN_HEREDOC: return "HEREDOC";
		case TOKEN_APPEND: return "APPEND";
		case TOKEN_END: return "END";
		default: return "UNKNOWN";
	}
}

// Function to print a single token's details
static void print_single_token(t_tokens *token, int index)
{
	if (!token)
	{
		printf("Token %d: NULL\n", index);
		return;
	}
	printf("Token %d:\n", index);
	printf("  str: %s\n", token->str ? token->str : "(null)");
	printf("  str_backup: %s\n", token->str_bkp ? token->str_bkp : "(null)");
	printf("  var_exists: %s\n", token->is_var ? "true" : "false");
	printf("  type: %s (%d)\n", get_token_type_name(token->type), token->type);
	printf("  status: %d\n", token->status);
	printf("  join: %s\n", token->join ? "true" : "false");
	printf("  prev: %p\n", (void *)token->prev);
	printf("  next: %p\n", (void *)token->next);
}

// Function to print all tokens in the linked list
void print_tokens(t_tokens *head)
{
	t_tokens *current = head;
	int index = 0;

	if (!current)
	{
		printf("No tokens in the list.\n");
		return;
	}
	printf("=== Token List ===\n");
	while (current)
	{
		print_single_token(current, index);
		current = current->next;
		index++;
	}
	printf("=================\n");
}