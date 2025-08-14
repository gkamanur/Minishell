/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:09:22 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/14 16:58:04 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_last_exit_code = 0;

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    (void)envp;
	t_data data;
	
    // init_signals();
    putstr_clr("started", 1, YELLOW);
    printbanner();
    ft_memset(&data, 0, sizeof(t_data));
    if (!init_data(&data, envp))
        exit_shell(NULL, EXIT_FAILURE);
    while (1)
    {
        
        data.input_raw = readline(INPUT);
        set_signals_interactive();
        if (parse_input(&data))
            g_last_exit_code = 0;
        else
            g_last_exit_code = 1;
        
        free_data(&data, FALSE);
    }

	exit_shell(&data, g_last_exit_code);
    return (0);
}