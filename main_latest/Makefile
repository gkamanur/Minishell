# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/29 14:01:48 by gkamanur          #+#    #+#              #
#    Updated: 2025/08/14 16:21:31 by gkamanur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = cc -fsanitize=address

FLAGS =  -Werror -Wall -Wextra -g

RM = rm -rf

# include source.list

LIBFT_DIR       := libft
BUILTINS_DIR 	:= builtins
ENV_DIR      	:= environment
HELPER_DIR   	:= helper
MALLOC_FREE_DIR := malloc_free
PRINT_DIR     	:= print
MAIN_DIR     	:= main

# Auto-discover all .c files
SRC := \
	$(wildcard $(LIBFT_DIR)/*.c) \
	$(wildcard $(HELPER_DIR)/*.c) \
	$(wildcard $(ENV_DIR)/*.c) \
	$(wildcard $(MALLOC_FREE_DIR)/*.c) \
	$(wildcard $(BUILTINS_DIR)/*.c) \
	$(wildcard $(PRINT_DIR)/*.c) \
	$(wildcard $(MAIN_DIR)/*.c)
	  
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