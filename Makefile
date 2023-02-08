# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/27 15:24:38 by daejlee           #+#    #+#              #
#    Updated: 2023/02/07 21:56:06 by daejlee          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= cub3D
CFLAGS 		= -g -Wall -Wextra -Werror
CC 			= cc
RM			= rm -f

INCLUDE 	= -I./include -I$(MLX_DIR) -I./libft_garage/libft -I./libft_garage/gnl
LIBRARY 	= -L$(LIBFT_DIR) -lft -lmlx

SRC 		= main.c control.c parse.c parse_utils_1.c parse_utils_2.c

OBJ 		= $(SRC:.c=.o)

LIBFT 		= ./libft_garage/libft.a
LIBFT_DIR	= ./libft_garage/

MLX_DIR		= ./minilibx_mms_20200219/
MLX_FLAGS	=  -framework OpenGL -framework Appkit

all : $(NAME)

m1 :
	arch -x86_64 make

$(NAME) : $(OBJ)
	$(MAKE) all -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) -g -o $(NAME) $(OBJ) $(LIBRARY) $(MLX_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean :
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) $(OBJ)

fclean : clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all
