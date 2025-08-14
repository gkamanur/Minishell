/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:33:19 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:52:15 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	my_fun(unsigned int i, char *c)
{
	if (i % 2 == 0)
		if (*c >= 65 && *c <= 90)
			*c += 32;
	if (i % 2 != 0)
		if (*c >= 97 && *c <= 122)
			*c -= 32;
}

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i] != 0)
	{
		f(i, &s[i]);
		i++;
	}
}
