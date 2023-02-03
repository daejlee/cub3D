/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:06:27 by hkong             #+#    #+#             */
/*   Updated: 2023/02/03 22:13:03 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

const char worldMap[MAP_WIDTH][MAP_HEIGHT]=
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
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
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

void	set_pixel(t_img image, int x, int y, unsigned int color)
{
	*(unsigned int *)(image.data + \
						x * image.line_size + y * (image.bpp / 8)) = color;
}

unsigned int	get_pixel(t_img image, int x, int y)
{
	unsigned int	color;

	color = *(unsigned int *)(image.data \
								+ x * image.line_size + y * (image.bpp / 8));
	return (color);
}

void	draw_default_screen(t_info *info)
{
	t_ivector	index;
	int			color;

	if (info->screen.ptr)
		mlx_destroy_image(info->mlx.ptr, info->screen.ptr);
	info->screen.ptr = mlx_new_image(info->mlx.ptr, \
										SCREEN_WIDTH, SCREEN_HEIGHT);
	info->screen.data = mlx_get_data_addr(info->screen.ptr, \
		&(info->screen.bpp), &(info->screen.line_size), &(info->screen.endian));
	index.x = -1;
	while (++index.x < SCREEN_HEIGHT)
	{
		index.y = -1;
		while (++index.y < SCREEN_WIDTH)
		{
			if (index.x < SCREEN_HEIGHT / 2)
				color = info->ceil;
			else
				color = info->floor;
			set_pixel(info->screen, index.x, index.y, color);
		}
	}
}

t_line_info	*set_line_info(t_info *info, int x_pixel)
{
	t_line_info	*line;

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

int	step_dir(double ray)
{
	if (ray < 0)
		return (-1);
	return (1);
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
			line->ray_pos.x += step_dir(line->ray.x);
			side = 0;
		}
		else
		{
			line->ray_len.y += line->delta.y;
			line->ray_pos.y += step_dir(line->ray.y);
			side = 1;
		}
		if (worldMap[line->ray_pos.x][line->ray_pos.y] == '1')
			hit_wall = 1;
	}
	return (side);
}

enum wall_dir	get_wall_dir(t_line_info *line, int side)
{
	if (side == 0)
	{
		if (line->ray_pos.x >= line->info->pos.x)
			return (NORTH);
		else
			return (SOUTH);
	}
	if (line->ray_pos.y >= line->info->pos.y)
		return (EAST);
	return (WEST);
}

int	get_texture_x(t_line_info *line, int side, double wall_dist)
{
	double	wall_x;
	int		texture_x;

	if (side == 0)
		wall_x = line->info->pos.y + wall_dist * line->ray.y;
	else
		wall_x = line->info->pos.x + wall_dist * line->ray.x;
	wall_x -= floor(wall_x);
	texture_x = (int)(wall_x * (double)TEXTURE_SIZE);
	if ((side == 0 && line->ray.x > 0) || (side == 1 && line->ray.y < 0))
		texture_x = TEXTURE_SIZE - texture_x -1;
	return (texture_x);
}

int	get_wall_start(int wall_height)
{
	int	start;

	start = -wall_height / 2 + SCREEN_HEIGHT / 2;
	if (start < 0)
		start = 0;
	return (start);
}

int	get_wall_end(int wall_height)
{
	int	end;

	end = wall_height / 2 + SCREEN_HEIGHT / 2;
	if (end >= SCREEN_HEIGHT)
		end = SCREEN_HEIGHT - 1;
	return (end);
}

int	draw_wall(t_info *info, int wall_height, int texture_x, enum wall_dir wall_dir)
{
	t_ivector		pixel;
	double			accurate_tex_y;
	double			texture_step;
	unsigned int	color;
	int				texture_y;

	texture_step = (double)TEXTURE_SIZE / wall_height;
	accurate_tex_y = 0;
	if (SCREEN_HEIGHT / 2 - wall_height / 2 < 0)
		accurate_tex_y = (-SCREEN_HEIGHT / 2 + wall_height / 2) * texture_step;
	pixel.y = get_wall_start(wall_height);
	while (pixel.y <= get_wall_end(wall_height))
	{
		texture_y = (int)accurate_tex_y;
		if (texture_y >= TEXTURE_SIZE)
			texture_y -= TEXTURE_SIZE;
		accurate_tex_y += texture_step;
		color = get_pixel(info->wall[wall_dir], texture_y, texture_x);
		if(side == 1) color = (color >> 1) & 8355711;
		set_pixel(info->screen, pixel.y, pixel.x, color);
		pixel.y++;
	}
}

int	draw_line(int side, int x_pixel, t_line_info *line)
{
	double	wall_dist;
	int		wall_height;
	int		start;
	int		end;
	int		wall_dir;
	int		texX;

	if (side == 0)
		wall_dist = (line->ray_len.x - line->delta.x);
	else
		wall_dist = (line->ray_len.y - line->delta.y);
	wall_height = (int)(SCREEN_HEIGHT / wall_dist);
	wall_dir = get_wall_dir(line, side);
	texX = get_texture_x(line, side, wall_dist);
	double step = 1.0 * TEXTURE_SIZE / wall_height;
	double texPos = 0;
	if (SCREEN_HEIGHT/2 - wall_height/2 < 0)
		texPos = (-SCREEN_HEIGHT/2 + wall_height/2) * step;
	// double texPos = (start - SCREEN_HEIGHT / 2 - wall_height) * step;
	start = -wall_height / 2 + SCREEN_HEIGHT / 2;
	if (start < 0)
		start = 0;
	end = wall_height / 2 + SCREEN_HEIGHT / 2;
	if (end >= SCREEN_HEIGHT)
		end = SCREEN_HEIGHT - 1;
	for (int pixel = start; pixel <= end; pixel++)
	{
		int texY = (int)texPos;
		if (texY >= TEXTURE_SIZE)
			texY -= TEXTURE_SIZE;
		texPos += step;
		unsigned int color = get_pixel(line->info->wall[wall_dir], texY, texX);
		if(side == 1) color = (color >> 1) & 8355711;
		set_pixel(line->info->screen, pixel, x_pixel, color);
	}
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

void	set_image(t_info *info, t_img *image, char *filename)
{
	int	width;
	int	height;

	//todo: file 존재/형식 올바른지 확인
	image->ptr = mlx_xpm_file_to_image(info->mlx.ptr, filename, &width, &height);
	image->data = mlx_get_data_addr(image->ptr, &(image->bpp), &(image->line_size), &(image->endian));
}

int main(void)
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
	info->ceil = 0x0099CCFF;
	info->floor = 0x00808080;

	/* mlx 초기화 및 hook */
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
	info->mlx.ptr = mlx;
	info->mlx.win_ptr = mlx_win;
	info->screen.ptr = NULL;
	set_image(info, info->wall + NORTH, "source/new1.xpm");
	set_image(info, info->wall + SOUTH, "source/new2.xpm");
	set_image(info, info->wall + EAST, "source/new3.xpm");
	set_image(info, info->wall + WEST, "source/new4.xpm");
	mlx_hook(mlx_win, ON_DESTROY_EVENT, 0, on_destroy, NULL);
	mlx_hook(mlx_win, ON_KEYDOWN_EVENT, 0, on_keydown, info);
	mlx_loop_hook(mlx, draw_map, info);
	mlx_loop(mlx);
	return (0);
}
