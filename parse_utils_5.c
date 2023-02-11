/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 18:32:49 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/11 15:37:28 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_map_border(t_info *info, int x)
{
	int	y;

	y = -1;
	while (++y < info->width)
	{
		if (info->map[x][y] == ' ')
			dfs(info, x, y);
		else if (info->map[x][y] != '1')
			err(INVALID_MAP);
	}
}

static void	check_map_inside(t_info *info, int x)
{
	if (info->map[x][0] == ' ')
		dfs(info, x, 0);
	else if (info->map[x][0] != '1')
		err(INVALID_MAP);
	if (info->map[x][info->width - 1] == ' ')
		dfs(info, x, info->width - 1);
	else if (info->map[x][info->width - 1] != '1')
		err(INVALID_MAP);
}

void	examine_map(t_info *info)
{
	int	x;
	int	i;

	x = -1;
	while (++x < info->height)
	{
		if (x == 0 || x == info->height - 1)
			check_map_border(info, x);
		else
			check_map_inside(info, x);
	}
	i = -1;
	while (++i < info->height)
	{
		if (ft_strchr(info->map[i], ' '))
			err(INVALID_MAP);
	}
}

void	check_map_format(char *map_name)
{
	unsigned int	len;

	len = ft_strlen(map_name);
	if (len < 4)
		err(INVALID_MAP_FORMAT);
	if (map_name[len - 1] != 'b' || map_name[len - 2] != 'u'
		|| map_name[len - 3] != 'c' || map_name[len - 4] != '.')
		err(INVALID_MAP_FORMAT);
}
