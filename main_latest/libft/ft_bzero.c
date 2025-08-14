/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:43:19 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:27:59 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void	*ft_memset(void *s, int c, size_t n);

void	ft_bzero(void *dst, size_t n)
{
	ft_memset(dst, 0, n);
}
