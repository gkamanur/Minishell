/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:26:28 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 16:19:49 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t wait (int *status)
{
    pid_t result;
    if (!status)
    {
        putstr_clr("wait : arg requires\n", 2, RED);
        return (-1);
    }
    result = wait(status);
    if (result == -1)
        putstr_clr("wait : Failed\n", 1, RED);
    return (result);
}

pid_t	Waitpid(pid_t pid, int *status, int options)
{
	pid_t	result;

	if (!status)
		return (-1);
	result = waitpid(pid, status, options);
	if (result == -1)
		putstr_clr("waitpid : Failed\n", 1, RED);
	return (result);
}

char *Readline(void)
{
    char *line = readline(BLUE"🧐🧐$MINISHELL$> "RESET);
    if (!line)
    {
        dbzSpinnerLoading();
        free(line);
	    exit(EX_OK);
    }
    else
        add_history(line);
    return line;
}

void dbzSpinnerLoading() 
{
    const char *charging[] = {
        "[            ]",
        "[=           ]",
        "[==          ]",
        "[===         ]",
        "[====        ]",
        "[=====       ]",
        "[======      ]",
        "[=======     ]",
        "[========    ]",
        "[=========   ]",
        "[==========  ]",
        "[=========== ]",
        "[===========💥]",
    };
    const int frames = sizeof(charging) / sizeof(charging[0]);

    putstr_clr("bye bye going off...", 1, RED);
    for (int i = 0; i < frames; i++) 
    {
        putstr_fd("\r",1);
        printf(YELLOW " %s "RESET, charging[i]);
        fflush(stdout);
        usleep(111337);  
    }
	putstr_clr("\n✅ EXIT ✅\n", 1, YELLOW);
}

void exitLoading() 
{
    const char *charging[] = {
        "[            ]",
        "[=           ]",
        "[==          ]",
        "[===         ]",
        "[====        ]",
        "[=====       ]",
        "[======      ]",
        "[=======     ]",
        "[========    ]",
        "[=========   ]",
        "[==========  ]",
        "[=========== ]",
        "[===========💥]",
    };
    const int frames = sizeof(charging) / sizeof(charging[0]);

	printf(RED"Shutting down...\n"RESET);

    // Loop through the "charging" animation for 3 seconds
    for (int i = 0; i < frames; i++) {
        printf("\r" YELLOW "%s" RESET, charging[i]);
        fflush(stdout);  // Force update the console
        usleep(421337);  
    }
	printf(GREEN"\n✅ EXIT ✅\n"RESET);
	exit(EX_OK);
}

void printbanner(void)
{
putstr_fd(
    GREEN
"                                                                   \n"
"███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n"
"████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n"
"██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n"
"██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n"
"██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n"
"╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n"RESET, 1);
                                                                  

}


                                   