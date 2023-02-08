/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:59:53 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/08 20:40:01 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include "get_next_line.h"
#include <fcntl.h>

int		is_floor_ceiling_color(char *gnl_buf)
{
	if (!ft_strncmp(gnl_buf, "F ", 2))
		return (1);
	else if (!ft_strncmp(gnl_buf, "C ", 2))
		return (2);
	else
		return (0);
}

int	get_rgb_val(int rgb_val[3])
{
	int	res;

	res = rgb_val[2];
	res += (rgb_val[1] * 16 * 16);
	res += (rgb_val[0] * 16 * 16 * 16 * 16);
	return (res);
}

void	get_map_slots(t_info *info, char *gnl_buf, int map_fd)
{
	int	i;

	info->width = ft_strlen(gnl_buf);
	info->height = 1;
	while (1)
	{
		free(gnl_buf);
		gnl_buf = get_next_line(map_fd);
		if (!gnl_buf)
			break ;
		if (ft_strlen(gnl_buf) > (size_t)info->width)
			info->width = ft_strlen(gnl_buf);
		info->height++;
	}
	info->map = (char **)malloc(sizeof(char *) * info->height);
	if (!info->map)
		parse_err(MALLOC_FAIL);
	i = 0;
	while (i < info->height)
	{
		info->map[i++] = (char *)malloc(sizeof(char) * info->width);
		if (!info->map[i - 1])
			parse_err(MALLOC_FAIL);
	}
}

int		is_map(char *gnl_buf)
{
	int		i;
	char	c;

	i = 0;
	while (gnl_buf[i])
	{
		c = gnl_buf[i++];
		if (c != '0' && c != '1' && c != ' ' && c != 'N' && c != 'S' && c != 'W' && c != 'E')
			return (1);
	}
	return (0);
}
