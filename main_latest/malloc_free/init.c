/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:38:32 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/13 16:49:08 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_env(t_data *data, char **env)
{
	int		i;

	data->env = ft_calloc(env_count(env) + 1, sizeof * data->env);
	if (!data->env)
		return (false);
	i = 0;
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
			return (false);
		i++;
	}
	return (true);
}

int	init_wds(t_data *data)
{
	char	buff[PATH_MAX];
	char	*wd;

	wd = getcwd(buff, PATH_MAX);
	data->pwd = ft_strdup(wd);
	if (!data->pwd)
		return (false);
	if (get_varindex_from_env(data->env, "OLDPWD") != -1)
	{
		data->owd = ft_strdup(get_varvalue_from_env(data->env,
					"OLDPWD"));
		if (!data->owd)
			return (false);
	}
	else
	{
		data->owd = ft_strdup(wd);
		if (!data->owd)
			return (false);
	}
	return (true);
}

bool	init_data(t_data *data, char **env)
{
	if (!init_env(data, env))
	{
		error_cmd("Fatal", NULL, "Could not initialize environment", 1);
		return (false);
	}
	if (!init_wds(data))
	{
		error_cmd("Fatal", NULL, "Could not initialize working directories",
			1);
		return (false);
	}
	data->token = NULL;
	data->input_raw = NULL;
	data->cmd = NULL;
	data->pid = -1;
	g_last_exit_code = 0;
	return (true);
}

/* init_io:
*	Initializes a structure with default values to contain
*	infiles and outfiles information for a command.
*/
void	init_file(t_cmd *cmd)
{
	if (!cmd->files)
	{
		cmd->files = malloc(sizeof * cmd->files);
		if (!cmd->files)
			return ;
		cmd->files->infile = NULL;
		cmd->files->outfile = NULL;
		cmd->files->heredoc_delim = NULL;
		cmd->files->heredoc_quotes = false;
		cmd->files->fd_in = -1;
		cmd->files->fd_out = -1;
		cmd->files->bkp_stdin= -1;
		cmd->files->bkp_stdout = -1;
	}
}