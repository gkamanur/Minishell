/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 14:38:11 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:51:32 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// size_t	ft_strlcpy(char *dst, const char *src, size_t size);

int	ft_word(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s != c)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

int	ft_wordlen(const char *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
	{
		len++;
	}
	return (len);
}

void	free_all(char **result, size_t i)
{
	while (i > 0)
		free(result[--i]);
	free(result);
	return ;
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	char	**result;

	result = (char **)malloc((ft_word(s, c) + 1) * sizeof(char *));
	if (!s || !c || !result)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s != c)
		{
			result[i] = (char *)malloc((ft_wordlen(s, c) + 1) * sizeof(char));
			if (!result[i])
				free_all(result, i);
			ft_strlcpy(result[i++], s,ft_wordlen(s, c) + 1);
			s +=ft_wordlen(s, c);
		}
	}
	result[i] = NULL;
	return (result);
}
