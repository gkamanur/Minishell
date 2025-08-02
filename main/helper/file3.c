/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:54:12 by gkamanur          #+#    #+#             */
/*   Updated: 2025/07/31 15:47:41 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const unsigned char	*s;
	unsigned char		*d;
	size_t				i;

	s = (const unsigned char *)src;
	d = (unsigned char *)dest;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

int	check(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	if (c >= '0' && c <= '9')
		return (2);
	return (0);
}

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

char	*ft_strchr(const char *str, int search_str)
{
	while ((*str != '\0') && (*str != (char)search_str))
	{
		str++;
	}
	if (*str == (char)search_str)
		return ((char *)str);
	return (NULL);
}

char* ft_strtok(char* input_string, const char* delimiter)
{
    static char* next_token = NULL;

    // If input is NULL
    if (input_string == NULL) {
        input_string = next_token;
    }
    if (input_string == NULL) {
        return NULL;
    }

    while (*input_string && ft_strchr(delimiter, *input_string)) {
        input_string++;
    }

    if (*input_string == '\0') {
        next_token = NULL;
        return NULL;
    }

    char* token = input_string;

    while (*input_string && !ft_strchr(delimiter, *input_string)) {
        input_string++;
    }

    if (*input_string) {
        *input_string = '\0';
        next_token = input_string + 1;
    } else {
        next_token = NULL;
    }

    return token; 
}

int	lst_size(t_tokens *lst)
{
	int count = 0;
    if (!lst) return 0;
    
    t_tokens *current = lst;
    while (current) {
        count++;
        current = current->next;
        if (count > 1000) break; // Prevent infinite loops
    }
    return count;
}