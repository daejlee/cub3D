/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:58:32 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/07 16:55:33 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include "get_next_line.h"
#include <fcntl.h>

void	get_cardinal_texture(t_info *info, char *gnl_buf, int *task_cnt_adr)
{
	int		code;
	char	**temp_arr;

	code = is_cardinal_texture(gnl_buf) - 1;
	temp_arr = ft_split(gnl_buf, ' ');
	if (!temp_arr)
		parse_err(MALLOC_FAIL);
	if (!set_image(info, info->wall + code, temp_arr[1]))
		parse_err(CORRUPTED_TEXTURE);
	free_arr(temp_arr);
	*task_cnt_adr--;
}

void	get_floor_ceiling_color(t_info *info, char *gnl_buf, int *task_cnt_adr)
{
	int		code;
	char	**temp;
	char	**color_arr;
	int		rgb_val[3];

	code = is_floor_ceiling_color(gnl_buf);
	temp = ft_split(gnl_buf, ' ');
	color_arr = ft_split(temp[1], ',');
	if (!temp || !color_arr)
		parse_err(MALLOC_FAIL);
	rgb_val[0] = ft_atoi(color_arr[0]);
	rgb_val[1] = ft_atoi(color_arr[1]);
	rgb_val[2] = ft_atoi(color_arr[2]);
	if (!is_invalid_rgb_val(rgb_val))
		parse_err(INVAILD_RGB_VAL);
	if (code == 1)
		info->floor = get_rgb_val(rgb_val);
	else
		info->ceil = get_rgb_val(rgb_val);
	free_arr(temp);
	free_arr(color_arr);
	*task_cnt_adr--;
}

void	get_info_until_map(int map_fd, t_info *info)
{
	char	*gnl_buf;
	int		task_cnt;

	gnl_buf = NULL;
	task_cnt = 6;
	while (1)
	{
		gnl_buf = get_next_line(map_fd);
		if (!gnl_buf)
			parse_err(NOT_ENOUGH_ELEM);
		else if (is_cardinal_texture(gnl_buf))
			get_cardinal_texture(info, gnl_buf, &task_cnt);
		else if (is_floor_ceiling_color(gnl_buf))
			get_floor_ceiling_color(info, gnl_buf, &task_cnt);
		else if (!is_map(gnl_buf))
		{
			if (task_cnt)
				parse_err(NOT_ENOUGH_ELEM);
			get_map_slots(info, gnl_buf, map_fd);
			break ;
		}
		else if (gnl_buf[0] != '\n')
			parse_err(INVALID_ELEM);
		free(gnl_buf);
	}
}

int		set_spawning_point(char c, t_info *info, int x, int y)
{
	info->pos.x = x + 0.5;
	info->pos.y = y + 0.5;
	info->plane.x = 0;
	info->plane.y = 0.66;
	if (c == 'N')
	{
		info->dir.x = -1;
		info->dir.y = 0;
	}
	else if (c == 'S')
	{
		info->dir.x = 1;
		info->dir.y = 0;
	}
	else if (c == 'W')
	{
		info->dir.x = 0;
		info->dir.y = -1;
	}
	info->dir.x = 0;
	info->dir.y = 1;
	return (0);
}

void	get_map(int map_fd, t_info *info)
{
	char	*gnl_buf;
	int		i;
	int		k;
	char	c;

	gnl_buf = get_next_line(map_fd);
	while (is_map(gnl_buf))
	{
		free(gnl_buf);
		get_next_line(map_fd);
	}
	i = 0;
	while (i < info->height)
	{
		k = 0;
		while (k < info->width)
		{
			c = gnl_buf[k];
			if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
				info->map[k++][i] = set_spawining_point(c, info, k, i);
			else
				info->map[k++][i] = c;
		}
		i++;
	}
}

char	**parse(t_info *info, char *map_name)
{
	int		map_fd;

	map_fd = open(map_name, O_RDONLY);
	if (map_fd == -1)
		parse_err(CORRUPTED_MAP);
	get_info_until_map(map_fd, info);
	close(map_fd);
	map_fd = open(map_name, O_RDONLY);
	get_map(map_fd, info);
	//examine_map(info);
}
