/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 16:47:26 by hkong             #+#    #+#             */
/*   Updated: 2023/02/08 16:45:41 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include <mlx.h>

# define SCREEN_WIDTH 1000
# define SCREEN_HEIGHT 600
# define MAP_WIDTH 24
# define MAP_HEIGHT 24
# define TEXTURE_SIZE 32

# define BUTTON_W 13
# define BUTTON_A 0
# define BUTTON_S 1
# define BUTTON_D 2
# define BUTTON_LEFT 123
# define BUTTON_RIGHT 124
# define BUTTON_ESC 53
# define ON_KEYDOWN_EVENT 2
# define ON_DESTROY_EVENT 17

# define ROTATE_SPEED 0.05
# define MOVE_SPEED 0.05

enum parse_err_code
{
	MALLOC_FAIL = 0,
	CORRUPTED_MAP,
	CORRUPTED_TEXTURE,
	NOT_ENOUGH_ELEM,
	DUPLICATED_ELEM,
	INVALID_ELEM,
	INVAILD_RGB_VAL,
	MAP_NOT_SURROUNDED_BY_WALL
};

enum wall_dir
{
	NORTH = 0,
	SOUTH,
	EAST,
	WEST,
};

typedef struct s_mlx
{
	void	*ptr;
	void	*win_ptr;
}	t_mlx;

typedef struct s_img
{
	void	*ptr;
	char	*data;
	int		bpp;
	int		line_size;
	int		endian;
}	t_img;

typedef struct s_dvector
{
	double	x;
	double	y;
}	t_dvector;

typedef struct s_ivector
{
	int	x;
	int	y;
}	t_ivector;

typedef struct s_info
{
	t_mlx		mlx;
	t_dvector	pos;
	t_dvector	dir;
	t_dvector	plane;
	t_img		screen;
	t_img		wall[4];
	char		**map;
	int			width;
	int			height;
	int			floor; // -1로 초기화 되어야 함
	int			ceil; // "
}	t_info;

typedef struct s_wall_info
{
	double			dist;
	int				height;
	int				start;
	int				end;
	double			tex_step;
	enum wall_dir	dir;
	t_ivector		tex;
}	t_wall_info;

typedef struct s_line_info
{
	int			x;
	t_dvector	ray;
	t_dvector	delta;
	t_ivector	ray_pos;
	t_dvector	ray_len;
	t_wall_info	wall;
	t_info		*info;
}	t_line_info;

extern const char worldMap[MAP_WIDTH][MAP_HEIGHT];

/**
 * draw.c
 */

int				draw_map(t_info *info);
void			draw_default_screen(t_info *info);
int				draw_line(int side, t_line_info *line);
int				draw_wall(t_line_info *line, int side);

/**
 * draw_utils.c
 */

int				get_texture_x(t_line_info *line, int side);
enum wall_dir	get_wall_dir(t_line_info *line, int side);
int				get_wall_start(t_line_info *line);
int				get_wall_end(t_line_info *line);

/**
 * check_wall.c
 */

int				dda_algorithm(t_line_info *line);
int				step_dir(double ray);

/**
 * pixel.c
 */

void			set_pixel(t_img image, int x, int y, unsigned int color);
unsigned int	get_pixel(t_img image, int x, int y);

/**
 * init.c
 */

t_line_info		*set_line_info(t_info *info, int x_pixel);
void			init_info(t_info *info);


/**
 * control.c
 */
int				on_keydown(int keycode, t_info *info);
int				on_destroy(void);
void			rotate(t_info *info, int dir);
void			move(t_info *info, double dir_x, double dir_y);

/**
 * parse.c
 */
void			parse(t_info *info, char *map_name);

/**
 * parse_utils_1.c
 */
void			free_arr(char **arr);
void			*set_image(t_info *info, t_img *image, char *filename);
int				is_invalid_rgb_val(int rgb_val[3]);
void			parse_err(int err_code);
int				is_cardinal_texture(char *gnl_buf);

/**
 * parse_utils_2.c
 */
int				is_floor_ceiling_color(char *gnl_buf);
int				get_rgb_val(int rgb_val[3]);
void			get_map_slots(t_info *info, char *gnl_buf, int map_fd);
int				is_map(char *gnl_buf);

#endif