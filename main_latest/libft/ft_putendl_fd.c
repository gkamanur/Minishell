/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:55:17 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:41:47 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// void ft_putchar_fd(char c, int fd);

void ft_putendl_fd(char *s, int fd)
{
    ft_putstr_fd(s, fd);
    write (fd, "\n", 1); 
}
