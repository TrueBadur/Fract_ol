# q
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/29 15:05:00 by ehugh-be          #+#    #+#              #
#    Updated: 2018/10/29 17:52:30 by ehugh-be         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRC = $(wildcard ft*.c) get_next_line.c 
OBJ=$(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(SRC) $(NAME)

$(NAME): $(OBJ)
	@ar r $(NAME) $(OBJ)

main: $(NAME) main.c 
	@$(CC) $(CFLAGS) main.c -o main $(NAME)

%.o: %.c $(HDR)
#	echo "\x1B[31m"
#	norminette $<
#	echo "\x1B[37m"
	@$(CC) $(CFLAGS) -c $< -o $@

fclean: clean
	@/bin/rm -f $(NAME) 

clean:
	@/bin/rm -f $(OBJ)
	@/bin/rm -f libft.h.gch

re: fclean all

