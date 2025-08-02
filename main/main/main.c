/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:09:22 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 15:59:35 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	try_execve_in_path(const char *file, char *const argv[], char *const envp[])
{
	char	*path_env;
	char	*paths;
	char	*dir;
	char	fullpath[1024];
	int		k;
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (-1);
	paths = strdup(path_env);
	dir = strtok(paths, ":");
	k = 0;
	while (dir)
	{
		snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, file);
		printf("full path %i: %s\n", ++k, fullpath);
		i = execve(fullpath, argv, envp);
		// If execve fails, continue to next path
		printf("execve : %i\n", i);
		dir = strtok(NULL, ":");
	}
	free(paths);
	return (-1); // execve failed in all paths
}

void	print_args(t_tokens *args)
{
	t_tokens	*current;
	const char	*type_str;

	current = args;
	while (current && current->token)
	{
		switch (current->type)
		{
		case TOKEN_WORD:
			type_str = "WORD";
			break ;
		case TOKEN_SQUOTED:
			type_str = "SQUOTED";
			break ;
		case TOKEN_DQUOTED:
			type_str = "DQUOTED";
			break ;
		case TOKEN_OPERATOR:
			type_str = "OPERATOR";
			break ;
		case TOKEN_VARIABLE:
			type_str = "VARIABLE";
			break ;
		default:
			type_str = "UNKNOWN";
			break ;
		}
		printf("%i : %s  {%s}\n", current->token_num, current->token, type_str);
		current = current->next;
	}
}

int main(int ac, char **av, char **envp)
{
    char        *inp_line;
    t_env       *env_list;
    int         status = 0; // Initialize status
    t_tokens    *args;

    (void)ac;
    (void)av;
    env_list = init_env(envp);
    init_signals();
    putstr_clr("started", 1, YELLOW);
    printbanner();

    while (1)
    {
        inp_line = Readline();
        if (!inp_line) // Handle EOF (Ctrl+D)
            break;
        if (strlen(inp_line) == 0)
        {
            free(inp_line);
            continue;
        }

        args = input_parse(inp_line);
        print_args(args);

        if (args && is_builtin(&args))
            status = execute_builtin(args, &env_list, status);

        free_tokens(args);
        free(inp_line);
    }

    free_env(env_list);
	rl_clear_history();
    return (0);
}
