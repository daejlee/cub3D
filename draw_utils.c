/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:14:53 by hkong             #+#    #+#             */
/*   Updated: 2023/02/11 15:42:48 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_texture_y(t_line_info *line, int side)
{
	double	wall_y;
	int		texture_y;

	if (side == 0)
		wall_y = line->info->pos.y + line->wall.dist * line->ray.y;
	else
		wall_y = line->info->pos.x + line->wall.dist * line->ray.x;
	wall_y -= floor(wall_y);
	texture_y = (int)(wall_y * (double)TEXTURE_SIZE);
	if ((side == 0 && line->ray.x > 0) || (side == 1 && line->ray.y < 0))
		texture_y = TEXTURE_SIZE - texture_y -1;
	return (texture_y);
}

enum e_wall_dir	get_wall_dir(t_line_info *line, int side)
{
	if (side == 0)
	{
		if (line->ray_pos.x >= line->info->pos.x)
			return (SOUTH);
		else
			return (NORTH);
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
