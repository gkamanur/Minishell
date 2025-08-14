/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:03:51 by gkamanur          #+#    #+#             */
/*   Updated: 2025/07/29 15:25:55 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	putstr_fd(char *str, int fd)
{
	while (*str)
		write(fd, str++, 1);
}

void	putstr_clr(char *str, int fd, char *clr)
{
	putstr_fd(clr, fd);
	putstr_fd(str, fd);
	putstr_fd(RESET "\n", fd);
}

void *Malloc (size_t size)
{
    void *ptr;
    if (size == 0)
    {
        return(NULL);
    }
    ptr = malloc (size);
    if (!ptr)
    {
        putstr_clr("Malloc failed", 2, RED);
        exit(EXIT_FAILURE);
    }
    return (ptr);
}

void *Realloc (void *ptr, size_t size)
{
    void *new_ptr;
    new_ptr = realloc (ptr, size);
    
    if (!new_ptr && size != 0)
    {
        putstr_clr("Realloc failed", 2, RED);
        exit(EXIT_FAILURE);
    }
    return (new_ptr);
}

void Chdir(const char *path)
{
    if (!path)
        putstr_clr("cd: give the path",2, RED);
    else if (chdir(path) == -1)
       putstr_clr("cd FAILED",2, RED);
    return ;
}
