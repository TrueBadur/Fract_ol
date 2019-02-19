# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/29 15:05:00 by ehugh-be          #+#    #+#              #
#    Updated: 2019/01/17 17:37:34 by bparker          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fract_ol
SRC = $(wildcard *.c) 
OBJ=$(SRC:.c=.o)
CC = clang
#CFLAGS = -Wall -Wextra -Werror
LIB = libft/libft.a
LIBOCL = libft/libocl.a
LIBDIR = libft/
LIBSPATH = -I libft/ -I /usr/local/include/
HDR = fractol.h
LINK = -lmlx -framework OpenGL -framework AppKit -L /usr/local/lib/
OCL = -framework OpenCL

all: lib $(OBJ) $(NAME)

$(NAME): $(LIB) $(OBJ)
	$(CC) $(CFLAGS) $(LIBSPATH) -o $(NAME) $(LINK) $(LIB) $(OCL) $(OBJ) $(LIBOCL)

lib:  
	make -C $(LIBDIR)

%.o: %.c $(HDR) $(LIB)
	$(CC) $(CFLAGS) -c $< -o $@

fclean: clean
	/bin/rm -f $(NAME) 
	make -C $(LIBDIR) fclean

clean: cleanlib
	/bin/rm -f $(OBJ)

cleanlib: 
	make -C $(LIBDIR) clean

re: fclean all

