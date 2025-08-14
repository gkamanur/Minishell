/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:26:22 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:45:48 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// size_t	ft_strlen(const char *str);

// char	*ft_strchr(const char *str, int search_str);

// size_t	ft_strlcpy(char *dst, const char *src, size_t size);

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trim;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while ((s1[start] != 0) && ft_strchr(set, s1[start]))
		start++;
	while ((end > start) && ft_strchr(set, s1[end - 1]))
		end--;
	trim = ((char *)malloc(((end - start) + 1) * sizeof(char)));
	if (!trim)
		return (NULL);
	ft_strlcpy(trim, s1 + start, end - start + 1);
	trim[end - start + 1] = 0;
	return (trim);
}
