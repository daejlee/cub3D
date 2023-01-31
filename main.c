#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <mlx.h>

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
# define MAP_WIDTH 24
# define MAP_HEIGHT 24

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
	double		old_time;
}	t_info;

typedef struct s_line_info
{
	double		x;
	double		height;
	t_dvector	ray;
	t_dvector	delta;
	t_ivector	ray_pos;
	t_dvector	ray_len;
	t_info		*info;
}	t_line_info;

char worldMap[MAP_WIDTH][MAP_HEIGHT]=
{
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','1','1','1','1','1','0','0','0','0','1','0','1','0','1','0','0','0','1'},
  {'1','0','0','0','0','0','1','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','1','0','0','0','1','0','0','0','0','1','0','0','0','1','0','0','0','1'},
  {'1','0','0','0','0','0','1','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','1','1','0','1','1','0','0','0','0','1','0','1','0','1','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','N','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','1','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','1','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','0','0','0','1','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','1','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','1','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

int	on_destroy(void)
{
	exit(0);
	return (0);
}

void	rotate(t_info *info, int dir)
{
	double	old_dir_x;
	double	old_plane_x;
	double	speed;

	speed = ROTATE_SPEED * dir;
	old_dir_x = info->dir.x;
	info->dir.x = info->dir.x * cos(speed) - info->dir.y * sin(speed);
	info->dir.y = old_dir_x * sin(speed) + info->dir.y * cos(speed);
	old_plane_x = info->plane.x;
	info->plane.x = info->plane.x * cos(speed) - info->plane.y * sin(speed);
	info->plane.y = old_plane_x * sin(speed) + info->plane.y * cos(speed);
}

int	on_keydown(int keycode, t_info *info)
{
	if (keycode == BUTTON_ESC)
		exit(0);
	else if (keycode == BUTTON_LEFT)
		rotate(info, 1);
	else if (keycode == BUTTON_RIGHT)
		rotate(info, -1);
	return (0);
}

void	draw_default_screen(t_info *info)
{
	t_ivector	index;
	int			floor_color;
	int			ceil_color;
	int			color;

	if (info->screen.ptr)
		mlx_destroy_image(info->mlx.ptr, info->screen.ptr);
	info->screen.ptr = mlx_new_image(info->mlx.ptr, SCREEN_WIDTH, SCREEN_HEIGHT);
	info->screen.data = mlx_get_data_addr(info->screen.ptr, &(info->screen.bpp), &(info->screen.line_size), &(info->screen.endian));
	ceil_color = 0x0099CCFF;
	floor_color = 0x00808080;
	index.x = -1;
	while (++index.x < SCREEN_HEIGHT)
	{
		index.y = -1;
		while (++index.y < SCREEN_WIDTH)
		{
			if (index.x < SCREEN_HEIGHT / 2)
				color = ceil_color;
			else
				color = floor_color;
			*(unsigned int *)(info->screen.data + index.x * info->screen.line_size + index.y * (info->screen.bpp / 8)) = color;
		}
	}
}

t_line_info	*set_line_info(t_info *info, int x_pixel)
{
	t_line_info *line;

	line = (t_line_info *)malloc(sizeof(t_line_info));
	if (!line)
		return (NULL);
	line->x = 2 * x_pixel / (double)SCREEN_WIDTH - 1;
	line->ray.x = info->dir.x + info->plane.x * line->x;
	line->ray.y = info->dir.y + info->plane.y * line->x;
	line->ray_pos.x = (int)info->pos.x;
	line->ray_pos.y = (int)info->pos.y;
	line->delta.x = fabs(1 / line->ray.x);
    line->delta.y = fabs(1 / line->ray.y);
	if (line->ray.x < 0)
		line->ray_len.x = (info->pos.x - line->ray_pos.x) * line->delta.x;
	else
		line->ray_len.x = (line->ray_pos.x + 1.0 - info->pos.x) * line->delta.x;
	if (line->ray.y < 0)
		line->ray_len.y = (info->pos.y - line->ray_pos.y) * line->delta.y;
	else
		line->ray_len.y = (line->ray_pos.y + 1.0 - info->pos.y) * line->delta.y;
	line->info = info;
	return (line);
}

int	dda_algorithm(t_line_info *line)
{
	int	hit_wall;
	int	side;

	hit_wall = 0;
	while (hit_wall == 0)
	{
		if (line->ray_len.x < line->ray_len.y)
		{
			line->ray_len.x += line->delta.x;
			if (line->ray.x < 0)
				line->ray_pos.x += -1;
			else
				line->ray_pos.x += 1;
			side = 0;
		}
		else
		{
			line->ray_len.y += line->delta.y;
			if (line->ray.y < 0)
				line->ray_pos.y += -1;
			else
				line->ray_pos.y += 1;
			side = 1;
		}
		if (worldMap[line->ray_pos.x][line->ray_pos.y] > '0')
			hit_wall = 1;
	}
	return (side);
}

int	draw_line(int side, int x_pixel, t_line_info *line)
{
	double	wall_dist;
	int		wall_height;
	int		start;
	int		end;
	int		color;

	if (side == 0)
		wall_dist = (line->ray_len.x - line->delta.x);
	else
		wall_dist = (line->ray_len.y - line->delta.y);
	wall_height = (int)(SCREEN_HEIGHT / wall_dist);
	start = -wall_height / 2 + SCREEN_HEIGHT / 2;
	if (start < 0)
		start = 0;
	end = wall_height / 2 + SCREEN_HEIGHT / 2;
	if (end >= SCREEN_HEIGHT)
		end = SCREEN_HEIGHT - 1;
	if (side == 0)
		color = 0x0000FF00;
	else
		color = 0x000000FF;
	for (int pixel = start; pixel <= end; pixel++)
		*(unsigned int *)(line->info->screen.data + pixel * line->info->screen.line_size + x_pixel * (line->info->screen.bpp / 8)) = color;
	return (0);
}

int	draw_map(t_info *info)
{
	t_line_info *line;
	int	x_pixel;
	int	side;

	draw_default_screen(info);
	x_pixel = -1;
	while (++x_pixel < SCREEN_WIDTH)
	{
		line = set_line_info(info, x_pixel);
		side = dda_algorithm(line);
		draw_line(side, x_pixel, line);
	}
	mlx_put_image_to_window(info->mlx.ptr, info->mlx.win_ptr, info->screen.ptr, 0, 0);
	return (0);
}

int	main()
{
	void	*mlx;
	void	*mlx_win;
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	/* 플레이어 위치. 맵에서 읽어서 저장해야 함 */
	info->pos.x = 12.5;
	info->pos.y = 12.5;
	/* 플레이어 방향. 역시 맵에서 읽어서 저장해야 함 */
	info->dir.x = -1;
	info->dir.y = 0;
	/* 카메라 평면 벡터. 플레이어 방향에 맞춰 변해야 함 */
	info->plane.x = 0;
	info->plane.y = 0.66;
	/* time of previous frame */
	info->old_time = 0;

	/* mlx 초기화 및 hook */
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
	info->mlx.ptr = mlx;
	info->mlx.win_ptr = mlx_win;
	info->screen.ptr = NULL;
	mlx_hook(mlx_win, ON_DESTROY_EVENT, 0, on_destroy, NULL);
	mlx_hook(mlx_win, ON_KEYDOWN_EVENT, 0, on_keydown, info);
	mlx_loop_hook(mlx, draw_map, info);
	mlx_loop(mlx);
	return (0);
}