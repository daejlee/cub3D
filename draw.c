/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:16:14 by hkong             #+#    #+#             */
/*   Updated: 2023/02/08 16:26:24 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	draw_map(t_info *info)
{
	t_line_info	*line;
	int			x_pixel;
	int			side;

	draw_default_screen(info);
	x_pixel = -1;
	while (++x_pixel < SCREEN_WIDTH)
	{
		line = set_line_info(info, x_pixel);
		side = dda_algorithm(line);
		draw_line(side, line);
	}
	mlx_put_image_to_window(info->mlx.ptr, info->mlx.win_ptr, \
														info->screen.ptr, 0, 0);
	return (0);
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

int	draw_line(int side, t_line_info *line)
{
	if (side == 0)
		line->wall.dist = (line->ray_len.x - line->delta.x);
	else
		line->wall.dist = (line->ray_len.y - line->delta.y);
	line->wall.height = (int)(SCREEN_HEIGHT / line->wall.dist);
	if (line->wall.height < 0)
		line->wall.height = 2147483647;
	line->wall.dir = get_wall_dir(line, side);
	line->wall.tex.x = get_texture_x(line, side);
	line->wall.start = get_wall_start(line);
	line->wall.end = get_wall_end(line);
	draw_wall(line, side);
	return (0);
}

int	draw_wall(t_line_info *line, int side)
{
	t_ivector		pixel;
	double			accurate_tex_y;
	unsigned int	color;

	line->wall.tex_step = (double)TEXTURE_SIZE / line->wall.height;
	accurate_tex_y = 0;
	if (SCREEN_HEIGHT / 2 - line->wall.height / 2 < 0)
		accurate_tex_y = (line->wall.height / 2 - SCREEN_HEIGHT / 2) \
														* line->wall.tex_step;
	pixel.x = line->x;
	pixel.y = line->wall.start;
	while (pixel.y <= line->wall.end)
	{
		line->wall.tex.y = (int)accurate_tex_y;
		if (line->wall.tex.y >= TEXTURE_SIZE)
			line->wall.tex.y -= TEXTURE_SIZE;
		accurate_tex_y += line->wall.tex_step;
		color = get_pixel(line->info->wall[line->wall.dir], \
									line->wall.tex.y, line->wall.tex.x);
		if (side == 1)
			color = (color >> 1) & 8355711;
		set_pixel(line->info->screen, pixel.y, line->x, color);
		pixel.y++;
	}
	return (0);
}