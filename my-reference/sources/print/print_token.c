
#include "minishell.h"

static const char *get_token_type_name(int type)
{
	switch (type)
	{
		case SPACES: return "SPACES";
		case WORD: return "WORD";
		case VAR: return "VAR";
		case PIPE: return "PIPE";
		case INPUT: return "INPUT";
		case TRUNC: return "TRUNC";
		case HEREDOC: return "HEREDOC";
		case APPEND: return "APPEND";
		case END: return "END";
		default: return "UNKNOWN";
	}
}

// Function to print a single token's details
static void print_single_token(t_token *token, int index)
{
	if (!token)
	{
		printf("Token %d: NULL\n", index);
		return;
	}
	printf("Token %d:\n", index);
	printf("  str: %s\n", token->str ? token->str : "(null)");
	printf("  str_backup: %s\n", token->str_backup ? token->str_backup : "(null)");
	printf("  var_exists: %s\n", token->var_exists ? "true" : "false");
	printf("  type: %s (%d)\n", get_token_type_name(token->type), token->type);
	printf("  status: %d\n", token->status);
	printf("  join: %s\n", token->join ? "true" : "false");
	printf("  prev: %p\n", (void *)token->prev);
	printf("  next: %p\n", (void *)token->next);
}

// Function to print all tokens in the linked list
void print_tokens(t_token *head)
{
	t_token *current = head;
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