/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:55:14 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/02 16:20:01 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ps -eo pid,comm | grep '[m]inishell' | awk '{print $1}' | xargs kill     */
#ifndef MINISHELL_H
# define MINISHELL_H
# define _DEFAULT_SOURCE
# include "colors.h"
# include <limits.h>
# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sysexits.h>
# include <signal.h>
#define SPAACE "\a\t\n\v\f\r "
# define BUFSIZEE 1024

typedef enum {
    TOKEN_WORD,      // Commands, arguments, filenames 
    TOKEN_SQUOTED,   //single Quoted strings
    TOKEN_DQUOTED,   //double Quoted strings
    TOKEN_OPERATOR,  // Operators like |, >, <, >>, &&
    TOKEN_VARIABLE   // Variables like $HOME
} t_token_type;

typedef struct s_tokens 
{
    char *token;
    int token_num;
    t_token_type type;
    struct s_tokens *next;
} t_tokens;

typedef struct s_parse
{
	t_tokens	**head;
	char		*buffer;
	int			token_num;
	size_t		i;
}				t_parse;

typedef struct s_cmd {
    char **args;
    char *input_file;
    char *output_file;
    int append;
    struct s_cmd *next;
} t_cmd;

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;



void	putstr_fd(char *str, int fd);
void	putstr_clr(char *str, int fd, char *clr);
int	is_operator(const char *str);
int	is_special_char(int c);
int	lst_size(t_tokens *lst);
void *Malloc (size_t size);
void *Realloc (void *ptr, size_t size);
pid_t wait (int *status);
pid_t	Waitpid(pid_t pid, int *status, int options);
void printbanner(void);
void dbzSpinnerLoading(void);
void exitLoading();
char *Readline(void);
void init_signals(void);


size_t	ft_strlen(const char *str);
int	ft_strcmp(const char *s1, const char *s2);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strdup(char *src);
char* ft_strtok(char* input_string, const char* delimiter);


int	ft_check(char c);
void add_token(t_tokens **head, char *token, int token_num, t_token_type type);
void skip_whitespace(char *input, size_t len, t_parse *ctx);
t_tokens	*input_parse(char *input);
void free_tokens(t_tokens *tokens);
void free_cmd(t_cmd *cmd);

t_env	*create_env_node(const char *key, const char *value);
t_env	*init_env(char **envp);
char	*get_env_value(const char *key, t_env *env_list);
void	free_env(t_env *env_list);
char **tokens_to_args(t_tokens *tokens_head);

/* builtins list*/
int builtin_cd(t_tokens *tokens, t_env **env_list);
int builtin_echo(t_tokens *tokens);
// int builtin_env(t_env *env_list);
int builtin_env(t_tokens *tokens, t_env **env_list);
void builtin_exit(int status);
int builtin_export(t_tokens *tokens, t_env **env_list);
int builtin_pwd(void);
int builtin_unset(t_tokens *tokens, t_env **env_list);



int is_builtin(t_tokens **tokens);
int execute_builtin(t_tokens *tokens, t_env **env_list, int last_status);



#endif