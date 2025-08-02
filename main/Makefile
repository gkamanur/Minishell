# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/29 14:01:48 by gkamanur          #+#    #+#              #
#    Updated: 2025/08/02 14:26:51 by gkamanur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = cc -fsanitize=address

FLAGS =  -Werror -Wall -Wextra

RM = rm -rf

 #SRC = builtins/built_utils.c\
# 	  builtins/built_utils.c\
# 	  builtins/environ_var.c\
# 	  helper/check.c\
# 	  helper/file1.c\
# 	  helper/file2.c\
# 	  helper/file3.c\
# 	  main/signal.c\
# 	  main/parsing.c\
# 	  main/main.c

include source.list
	  
OBJS = $(SRC:.c=.o)

HDR = includes/minishell.h\
	  includes/colors.h
	  
all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -lreadline -o $(NAME)
	
%.o: %.c $(HDR)
	$(CC) $(FLAGS) -c $< -o $@

clean : 
	$(RM) $(OBJS)
	
fclean : clean
		$(RM) $(NAME)
		
re : fclean all
	$(RM) $(OBJS)