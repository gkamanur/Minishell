/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:43:16 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/14 10:31:58 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int fd_restore(t_file *file)
{
    int ret;
    
    ret = TRUE;
    if (!file)
        return (ret);
    if (file->bkp_stdin != -1)
    {
        if (dup2(file->bkp_stdin, STDIN_FILENO) == -1)
            ret = FALSE;
        close(file->bkp_stdin);
        file->bkp_stdin = -1;
    }
    if (file->bkp_stdout != -1)
    {
        if (dup2(file->bkp_stdout, STDOUT_FILENO) == -1)
            ret = FALSE;
        close(file->bkp_stdout);
        file->bkp_stdout = -1;
    }
    return (ret);
}

int fd_redirect(t_file *file)
{
    int ret;
    
    ret = TRUE;
    if(!file)
        return (ret);
        
    file->bkp_stdin = dup(STDERR_FILENO);
    if (file->bkp_stdin == -1)
        ret = error_cmd("dup", "STDIN BKP", strerror(errno), FALSE);
    
    file->bkp_stdout = dup (STDOUT_FILENO);
    if (file->bkp_stdout == -1)
        ret = error_cmd("dup", "STDOUT BKP", strerror(errno), FALSE);
    
    if (file->fd_in != -1)
        if (dup2(file->fd_in, STDIN_FILENO) == -1)
            ret = error_cmd("dup2", file->infile, strerror(errno), FALSE);
    if (file->fd_out != -1)
        if (dup2(file->fd_out, STDIN_FILENO) == -1)
            ret = error_cmd("dup2", file->outfile, strerror(errno), FALSE);
    return (ret);
}

int check_files(t_file *file)
{
    if (!file || (!file->infile && !file->outfile))
        return (TRUE);
    if ((file->infile && file->fd_in == -1) || (file->outfile && file->fd_out == -1))
        return (FALSE);
    return (TRUE);
}

void	close_pipe_files(t_cmd *cmds, t_cmd *skip_cmd)
{
	while (cmds)
	{
		if (cmds != skip_cmd && cmds->pipe_fd)
		{
			close(cmds->pipe_fd[0]);
			close(cmds->pipe_fd[1]);
		}
		cmds = cmds->next;
	}
}

void close_files(t_cmd *cmd, int bkp)
{
    if (cmd->files)
    {
        if (cmd->files->fd_in != -1)
            close (cmd->files->fd_in);
        if (cmd->files->fd_out != -1)
            close (cmd->files->fd_out);
        if (bkp)
            fd_restore(cmd->files);
    }
    close_pipe_files(cmd, NULL);
}
