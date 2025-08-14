/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:48:11 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:52:08 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// size_t	ft_strlen(const char *str);

char	my_fun1(unsigned int i, char c)
{
	if (i % 2 == 0)
		if (c >= 65 && c <= 90)
			return (c + 32);
	if (i % 2 != 0)
		if (c >= 97 && c <= 122)
			return (c - 32);
	return (c);
}

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	int		i;

	str = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	i = 0;
	while (s[i] != 0)
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = 0;
	return (str);
}
