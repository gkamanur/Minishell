/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:52:22 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:33:44 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// size_t	ft_strlen(char *str);

char	*ft_strdup(char *src)
{
	size_t	i;
	size_t	j;
	char	*copy;

	i = ft_strlen(src);
	copy = (char *)malloc((i + 1) * sizeof(char));
	if (copy == NULL)
		return (NULL);
	j = 0;
	while (j <= i)
	{
		copy[j] = src[j];
		j++;
	}
	copy[i] = '\0';
	return (copy);
}

// int	main(void)
// {
// 	char orig[] = "abcdef";
// 	char *dup = ft_strdup(orig);
// 	if (dup != NULL)
// 	{
// 		printf("copied : %s", dup);
// 		free(dup);
// 	}
// 	return (0);
// }