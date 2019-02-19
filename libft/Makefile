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
OCL_NAME = libocl.a
SRC = $(wildcard ft*.c) get_next_line.c
OBJ = $(SRC:.c=.o)
OCL_SRC = ocl.c
OCL_OBJ = ocl.o
CC = clang
#CFLAGS = -Wall -Wextra -Werror

all: $(SRC) $(NAME) ocl

ocl: $(OCL_OBJ)
	ar r $(OCL_NAME) $(OCL_OBJ)

$(NAME): $(OBJ)
	ar r $(NAME) $(OBJ)

#main: $(NAME) main.c
#	@$(CC) $(CFLAGS) main.c -o main $(NAME)

%.o: %.c $(HDR)
#	echo "\x1B[31m"
#	norminette $<
#	echo "\x1B[37m"
	$(CC) $(CFLAGS) -c $< -o $@

fclean: clean
	@/bin/rm -f $(NAME) $(OCL_NAME)

clean:
	@/bin/rm -f $(OBJ) $(OCL_OBJ)
	@/bin/rm -f libft.h.gch

re: fclean all

