/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:51:30 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:33:44 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// void ft_putchar_fd(char c, int fd);

void ft_putstr_fd(char *s, int fd)
{
    int i;
    
    i = 0;
    while (s[i])
    {
        ft_putchar_fd(s[i++], fd);
    }
} 
