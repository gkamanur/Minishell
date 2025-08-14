/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:20:54 by gkamanur          #+#    #+#             */
/*   Updated: 2024/11/20 13:33:44 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr = (const unsigned char *)s;
	unsigned char		target;
	size_t				i;

	target = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == target)
		{
			return ((void *)(ptr + i));
		}
		i++;
	}
	return (NULL);
}

// #include <stdio.h>
// int main() {
//     char str[] = "Hello, World!";
//     char search = 'W';
//     // Use my_memchr to search for the first occurrence of 'W'
//     char *result = my_memchr(str, search, sizeof(str));
//     if (result != NULL) {
//         printf("Character '%c' found at position: %ld\n", 
//                   search, result - str);
//     } else {
//         printf("Character '%c' not found in the string.\n", search);
//     }
//     return 0;
// }
