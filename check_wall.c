/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:13:45 by hkong             #+#    #+#             */
/*   Updated: 2023/02/08 16:14:02 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
		if (line->info->map[line->ray_pos.x][line->ray_pos.y] == '1')
			hit_wall = 1;
	}
	return (side);
}

int	step_dir(double ray)
{
	if (ray < 0)
		return (-1);
	return (1);
}
