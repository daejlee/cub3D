# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/27 15:24:38 by daejlee           #+#    #+#              #
#    Updated: 2023/01/27 20:33:45 by hkong            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= cub3D
CFLAGS 		= -g -Wall -Wextra -Werror
CC 			= cc
SRC 		= main.c
OBJ 		= $(SRC:.c=.o)
LIBFT 		= ./libft_garage/libft.a
LIBFT_DIR	= ./libft_garage/
MLX_DIR		= ./minilibx_mms_20200219/
MLX_FLAGS	=  -framework OpenGL -framework Appkit

INCLUDE = -I./include -I$(MLX_DIR)
LIBRARY = -L$(LIBFT_DIR) -lft -lmlx

all : $(NAME)

$(NAME) : $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -g -o $(NAME) $(OBJ) $(LIBRARY) $(MLX_FLAGS)

$(OBJ) : $(SRC)
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC)

$(LIBFT) :
	cd $(LIBFT_DIR); $(MAKE)

clean :
	rm -f $(OBJ)
	make -C libft_garage/ clean

fclean :
	rm -f $(OBJ) $(NAME)
	make -C libft_garage/ fclean

re :
	$(MAKE) fclean
	$(MAKE) all
