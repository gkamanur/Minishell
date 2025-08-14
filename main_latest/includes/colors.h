/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:33:21 by gkamanur          #+#    #+#             */
/*   Updated: 2025/04/15 10:39:08 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

// ANSI Escape Codes for Colors
# define RESET "\x1B[0m"

// Standard Rainbow Colors
# define RED "\x1B[31m"
# define ORANGE "\x1B[38;5;214m"
# define YELLOW "\x1B[33m"
# define GREEN "\x1B[32m"
# define BLUE "\x1B[34m"
# define INDIGO "\x1B[38;5;54m"
# define VIOLET "\x1B[35m"

// Light Shades
# define LIGHT_RED "\x1B[91m"
# define LIGHT_ORANGE "\x1B[38;5;216m"
# define LIGHT_YELLOW "\x1B[93m"
# define LIGHT_GREEN "\x1B[92m"
# define LIGHT_BLUE "\x1B[94m"
# define LIGHT_INDIGO "\x1B[38;5;105m"
# define LIGHT_VIOLET "\x1B[95m"

// Dark Shades
# define DARK_RED "\x1B[38;5;88m"
# define DARK_ORANGE "\x1B[38;5;130m"
# define DARK_YELLOW "\x1B[38;5;136m"
# define DARK_GREEN "\x1B[38;5;22m"
# define DARK_BLUE "\x1B[38;5;17m"
# define DARK_INDIGO "\x1B[38;5;18m"
# define DARK_VIOLET "\x1B[38;5;53m"

// Filled (Background Colors)
# define FILLED_RED "\x1B[41m"
# define FILLED_ORANGE "\x1B[48;5;214m"
# define FILLED_YELLOW "\x1B[43m"
# define FILLED_GREEN "\x1B[42m"
# define FILLED_BLUE "\x1B[44m"
# define FILLED_INDIGO "\x1B[48;5;54m"
# define FILLED_VIOLET "\x1B[45m"

// Light Filled Backgrounds
# define FILLED_LIGHT_RED "\x1B[101m"
# define FILLED_LIGHT_ORANGE "\x1B[48;5;216m"
# define FILLED_LIGHT_YELLOW "\x1B[103m"
# define FILLED_LIGHT_GREEN "\x1B[102m"
# define FILLED_LIGHT_BLUE "\x1B[104m"
# define FILLED_LIGHT_INDIGO "\x1B[48;5;105m"
# define FILLED_LIGHT_VIOLET "\x1B[105m"

// Dark Filled Backgrounds
# define FILLED_DARK_RED "\x1B[48;5;88m"
# define FILLED_DARK_ORANGE "\x1B[48;5;130m"
# define FILLED_DARK_YELLOW "\x1B[48;5;136m"
# define FILLED_DARK_GREEN "\x1B[48;5;22m"
# define FILLED_DARK_BLUE "\x1B[48;5;17m"
# define FILLED_DARK_INDIGO "\x1B[48;5;18m"
# define FILLED_DARK_VIOLET "\x1B[48;5;53m"

#endif // RAINBOW_COLORS_H
