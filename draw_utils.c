/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:14:53 by hkong             #+#    #+#             */
/*   Updated: 2023/02/08 16:26:55 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_texture_x(t_line_info *line, int side)
{
	double	wall_x;
	int		texture_x;

	if (side == 0)
		wall_x = line->info->pos.y + line->wall.dist * line->ray.y;
	else
		wall_x = line->info->pos.x + line->wall.dist * line->ray.x;
	wall_x -= floor(wall_x);
	texture_x = (int)(wall_x * (double)TEXTURE_SIZE);
	if ((side == 0 && line->ray.x > 0) || (side == 1 && line->ray.y < 0))
		texture_x = TEXTURE_SIZE - texture_x -1;
	return (texture_x);
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

int	get_wall_start(t_line_info *line)
{
	int	start;

	start = -line->wall.height / 2 + SCREEN_HEIGHT / 2;
	if (start < 0)
		start = 0;
	return (start);
}

int	get_wall_end(t_line_info *line)
{
	int	end;

	end = line->wall.height / 2 + SCREEN_HEIGHT / 2;
	if (end >= SCREEN_HEIGHT)
		end = SCREEN_HEIGHT - 1;
	return (end);
}
