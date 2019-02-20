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
OCL_NAME = libocl.a
SRC = $(wildcard ft*.c) get_next_line.c
OBJ = $(SRC:.c=.o)
OCL_SRC = $(wildcard ft_ocl/*.c)
OCL_OBJ = $(OCL_SRC:.c=.o)
CC = clang
#CFLAGS = -Wall -Wextra -Werror

.c.a:
	@$(CC) $(CFLAGS) -c $< -o $*.o
	@ar r $@ $*.o
	@rm $*.o

all: libft.a($(OBJ)) libocl.a($(OCL_OBJ))

fclean: clean
	@/bin/rm -f $(NAME) $(OCL_NAME)

clean:
	@/bin/rm -f $(OBJ) $(OCL_OBJ)
	@/bin/rm -f libft.h.gch

re: fclean all

