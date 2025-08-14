/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/14 10:32:49 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void builtin_exit(int status)
// {
//     dbzSpinnerLoading();
//     exit(status);
// }

#include <limits.h>
#include <stdbool.h>
#include <ctype.h>

static bool check_out_of_range(int sign, unsigned long long num, bool *error)
{
    if ((sign > 0 && num > LONG_MAX) ||
        (sign < 0 && num > -(unsigned long)LONG_MIN))
    {
        *error = true;
        return true;
    }
    return false;
}

static long ft_atoi_long(char *str, bool *error)
{
    unsigned long long num = 0;
    int sign = 1;
    int i = 0;
    
    while (ft_isspace(str[i]))
        i++;

    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-') sign = -1;
        i++;
    }

    while (ft_isdigit(str[i]))
    {
        num = num * 10 + (str[i] - '0');
        if (check_out_of_range(sign, num, error))
            break;
        i++;
    }

    return (long)(num * sign);
}

static bool is_numeric_string(const char *str)
{
    while (ft_isspace(*str))
        str++;

    if (*str == '+' || *str == '-')
        str++;

    if (!ft_isdigit(*str))
        return false;

    while (*str)
    {
        if (!ft_isdigit(*str) && !ft_isspace(*str))
            return false;
        str++;
    }

    return true;
}

static int get_exit_code(char *arg, bool *error)
{
    if (!arg)
        return g_last_exit_code;

    if (!is_numeric_string(arg))
    {
        *error = true;
        return 0;
    }

    long value = ft_atoi_long(arg, error);
    return (int)(value % 256);
}

static bool is_quiet_mode(t_data *data)
{
    t_cmd *cmd = data->cmd;
    return (cmd && (cmd->next || cmd->prev));
}

void	exit_shell(t_data *data, int exno)
{
	if (data)
	{
		if (data->cmd && data->cmd->files)
			close_files(data->cmd, true);
		free_data(data, true);
	}
	exit(exno);
}

int builtin_exit(t_data *data, char **args)
{
    bool error = false;
    int exit_code;

    if (!is_quiet_mode(data) && data->valid)
        ft_putendl_fd("exit", 2);

    if (!args[1])
        exit_code = g_last_exit_code;
    else
    {
        exit_code = get_exit_code(args[1], &error);

        if (error)
            exit_code = error_cmd("exit", args[1], "numeric argument required", 2);
        else if (args[2])
            return error_cmd("exit", NULL, "too many arguments", 1);
    }

    exit_shell(data, exit_code);
    return 2;
}

