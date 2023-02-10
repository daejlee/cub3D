/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 18:32:15 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/10 18:45:49 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	set_spawning_point(char c, t_info *info, int x, int y)
{
	info->pos.x = x + 0.5;
	info->pos.y = y + 0.5;
	if (c == 'N')
	{
		info->dir.x = -1;
		info->plane.y = 0.66;
	}
	else if (c == 'S')
	{
		info->dir.x = 1;
		info->plane.y = -0.66;
	}
	else if (c == 'W')
	{
		info->dir.y = -1;
		info->plane.x = -0.66;
	}
	else
	{
		info->dir.y = 1;
		info->plane.x = 0.66;
	}
	return ('0');
}

static void	paste_map(t_info *info, char *gnl_buf, int *spawn_flag, int i)
{
	int		k;
	char	c;

	k = 0;
	while (k < info->width)
	{
		if ((unsigned int)k < ft_strlen(gnl_buf))
			c = gnl_buf[k];
		else
			c = ' ';
		if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		{
			if (*spawn_flag)
				err(INVALID_MAP);
			info->map[i][k] = set_spawning_point(c, info, i, k);
			*spawn_flag = 1;
		}
		else
			info->map[i][k] = c;
		k++;
	}
}

void	get_map(int map_fd, t_info *info)
{
	char	*gnl_buf;
	int		i;
	int		spawn_flag;

	gnl_buf = get_next_line(map_fd);
	gnl_buf = remove_tailing_nl(gnl_buf);
	while (is_map(gnl_buf))
	{
		free(gnl_buf);
		gnl_buf = get_next_line(map_fd);
		gnl_buf = remove_tailing_nl(gnl_buf);
	}
	i = -1;
	spawn_flag = 0;
	while (++i < info->height)
	{
		gnl_buf = remove_tailing_nl(gnl_buf);
		if (is_map(gnl_buf))
			err(INVALID_ELEM);
		paste_map(info, gnl_buf, &spawn_flag, i);
		free(gnl_buf);
		gnl_buf = get_next_line(map_fd);
	}
	if (!spawn_flag)
		err(INVALID_MAP);
}
