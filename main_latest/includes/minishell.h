/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:55:14 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/15 14:46:27 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ps -eo pid,comm | grep '[m]inishell' | awk '{print $1}' | xargs kill     */
#ifndef MINISHELL_H
# define MINISHELL_H
# define _DEFAULT_SOURCE
# include "colors.h"
# include "../libft/libft.h"
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# define INPUT RED"Minishell >$$"RESET
# define TRUE 1
# define FALSE 0

extern int 	g_last_exit_code;

typedef struct s_tokens 
{
    char *str;
    char *str_bkp;
    int token_num;
    int is_var;
    int type;
    int join;
    int status;
    struct s_tokens *next;
    struct s_tokens *prev;
} t_tokens;

typedef struct s_file
{
    char *infile;
    char *outfile;
    int fd_in;
    int fd_out;
    char *heredoc_delim;
    int *heredoc_quotes;
    int bkp_stdin;
    int bkp_stdout;
} t_file;

typedef struct s_parse
{
	t_tokens	**head;
	char		*buffer;
	int			token_num;
	size_t		i;
}				t_parse;

typedef struct s_cmd {
    char *cmd;
    char *path;
    char **args;
    int pipe_out;
    int *pipe_fd;
    int append;
    t_file *files;
    struct s_cmd *next;
    struct s_cmd *prev;
} t_cmd;

typedef struct s_data 
{
    int valid;
    t_tokens *token;
    char *input_raw;
    char **env;
    char *pwd;
    char *owd;
    t_cmd *cmd;
    pid_t pid;
} t_data;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

enum e_token
{
    TOKEN_SPACE = 1,
    TOKEN_WORD,
    TOKEN_VAR, 
    TOKEN_PIPE, 
    TOKEN_INPUT, 
    TOKEN_OUTPUT,
    TOKEN_APPEND, 
    TOKEN_HEREDOC,   
    TOKEN_AMPER,  
    TOKEN_END   
};

enum e_status
{
    DEFAULT,
    SQ,
    DQ
};
//Signals
void signal_handler(int sig);
void init_signals(void);
void	set_signals_interactive(void);

//initialization
int	init_env(t_data *data, char **env);
int	init_wds(t_data *data);
bool	init_data(t_data *data, char **env);
void	init_file(t_cmd *cmd);

//free 
void free_array(char **av);
void free_data(t_data *data, int clear_history);
void free_ptr(void *p);
void free_file(t_file *file);
void free_token(t_tokens **lst, void (*del)(void *));
//token_utils
void delete_token(t_tokens *lst, void (*del)(void *));
void	clear_token(t_tokens **lst, void (*del)(void *));
void	add_token(t_tokens **alst, t_tokens *new_node);
t_tokens	*new_token(char *str, char *str_backup, int type, int status);
//cmd_utils
void init_cmd(t_cmd **cmd);
t_cmd *new_cmd(int val);
void add_cmd(t_cmd **lst, t_cmd *new_node);
t_cmd *last_cmd(t_cmd *cmd);
void del_node_cmd(t_cmd *lst, void (*del)(void *));
void lstclear_cmd(t_cmd **lst, void (*del)(void *));
//fd
int fd_restore(t_file *file);
int fd_redirect(t_file *file);
int check_files(t_file *file);
void	close_pipe_files(t_cmd *cmds, t_cmd *skip_cmd);
void close_files(t_cmd *cmd, int bkp);

//file1
void	putstr_fd(char *str, int fd);
void	putstr_clr(char *str, int fd, char *clr);
void *Malloc (size_t size);
void *Realloc (void *ptr, size_t size);
void Chdir(const char *path);
//file2
pid_t wait (int *status);
pid_t	Waitpid(pid_t pid, int *status, int options);
char *Readline(const char *str);
void dbzSpinnerLoading();
void exitLoading();
void printbanner(void);
//print
void print_tokens(t_tokens *head);
//check
int	ft_check(char c);
int	is_operator(const char *str);
int	is_special_char(int c);
int input_isspace(char *inp);
int input_isseparator(char *str, int i);
void check_var(t_tokens **lst);
int if_var(t_tokens **lst);

//consecutive check
int check_consec(t_tokens *node);
int if_consec(t_tokens **lst);

// environment
int env_count(char **av);
int get_varindex_from_env(char **env, char *var);
char *get_varvalue_from_env(char **env, char *var);
int env_var_valid(char *str);

char **Realloc_env(t_data *data, int size);
int delenvvarr(t_data *data, int idx);
int set_envar(t_data *data, char *key, char *val);

//error
int add_detailquotes(char *cmd);
int error_cmd(char *cmd, char *detail, char *err_msg, int num);
void error_msg(const char *msg, const char *func_name);
int use_mssg(int ret);

//tokenization
int	check_status(int status, char *str, int i);
int	input_isseparator(char *str, int i);
int	if_word(t_tokens **token, char *str, int ind, int start);
int	if_separator(t_tokens **lst, char *str, int type, int ind);
int	word_or_sep(int *i, char *str, int start, t_data *data);
int	make_tokens(t_data *data, char *str);

//Builtins
void	exit_shell(t_data *data, int exno);
int builtin_exit(t_data *data, char **args);

//parse
int	parse_input(t_data *data);
#endif