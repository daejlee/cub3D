/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:44:39 by hkong             #+#    #+#             */
/*   Updated: 2023/02/08 20:57:11 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_line_info	*set_line_info(t_info *info, int y_pixel)
{
	t_line_info	*line;
	double		y_rate;

	line = (t_line_info *)malloc(sizeof(t_line_info));
	if (!line)
		parse_err(MALLOC_FAIL);
	y_rate = 2 * y_pixel / (double)SCREEN_WIDTH - 1;
	line->y = y_pixel;
	line->ray.x = info->dir.x + info->plane.x * y_rate;
	line->ray.y = info->dir.y + info->plane.y * y_rate;
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

t_info	*init_info(void)
{
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		parse_err(MALLOC_FAIL);
	info->ceil = -1;
	info->floor = -1;
	info->dir.x = 0;
	info->dir.y = 0;
	info->map = NULL;
	info->mlx.ptr = NULL;
	info->mlx.win_ptr = NULL;
	info->plane.x = 0;
	info->plane.y = 0;
	info->pos.x = 0;
	info->pos.y = 0;
	info->screen.ptr = NULL;
	info->wall_img[0].ptr = NULL;
	info->wall_img[1].ptr = NULL;
	info->wall_img[2].ptr = NULL;
	info->wall_img[3].ptr = NULL;
	info->width = 0;
	info->height = 0;
	return (info);
}