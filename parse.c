/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:58:32 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/08 22:05:09 by hkong            ###   ########.fr       */
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
	if (info->wall_img[code].ptr) //ptr는 NULL로 초기화 되어 있어야 한다.
		parse_err(DUPLICATED_ELEM);
	temp_arr = ft_split(gnl_buf, ' ');
	if (!temp_arr)
		parse_err(MALLOC_FAIL);
	if (!temp_arr[1] || temp_arr[2])
		parse_err(INVALID_ELEM);
	if (!set_image(info, info->wall_img + code, temp_arr[1]))
		parse_err(CORRUPTED_TEXTURE);
	free_arr(temp_arr);
	(*task_cnt_adr)--;
}

void	get_floor_ceiling_color(t_info *info, char *gnl_buf, int *task_cnt_adr)
{
	int		code;
	char	**temp_arr;
	char	**color_arr;
	int		rgb_val[3];

	code = is_floor_ceiling_color(gnl_buf);
	temp_arr = ft_split(gnl_buf, ' ');
	if (!temp_arr)
		parse_err(MALLOC_FAIL);
	else if (!temp_arr[1] || temp_arr[2])
		parse_err(INVALID_ELEM);
	color_arr = ft_split(temp_arr[1], ',');
	if (!color_arr)
		parse_err(MALLOC_FAIL);
	else if (!color_arr[2] || color_arr[3])
		parse_err(INVALID_ELEM);
	rgb_val[0] = ft_atoi(color_arr[0]);
	rgb_val[1] = ft_atoi(color_arr[1]);
	rgb_val[2] = ft_atoi(color_arr[2]);
	if (!is_invalid_rgb_val(rgb_val))
		parse_err(INVAILD_RGB_VAL);
	if (code == 1)
	{
		if (info->floor != -1)
			parse_err(DUPLICATED_ELEM);
		info->floor = get_rgb_val(rgb_val);
	}
	else
	{
		if (info->ceil != -1)
			parse_err(DUPLICATED_ELEM);
		info->ceil = get_rgb_val(rgb_val);
	}
	free_arr(temp_arr);
	free_arr(color_arr);
	(*task_cnt_adr)--;
}

void	get_info_until_map(int map_fd, t_info *info)
{
	char	*gnl_buf;
	int		task_cnt;

	task_cnt = 6;
	while (1)
	{
		gnl_buf = get_next_line(map_fd);
		if (!gnl_buf)
			parse_err(NOT_ENOUGH_ELEM);
		if (ft_strlen(gnl_buf) != 1 && gnl_buf[ft_strlen(gnl_buf) - 1] == '\n')
			gnl_buf[ft_strlen(gnl_buf) - 1] = 0;
		if (is_cardinal_texture(gnl_buf))
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
	info->dir.x = 0;
	info->dir.y = 0;
	info->plane.x = 0;
	info->plane.y = 0;
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

void	get_map(int map_fd, t_info *info)
{
	char	*gnl_buf;
	int		i;
	int		k;
	char	c;
	int		spawn_flag;

	gnl_buf = get_next_line(map_fd);
	if (ft_strlen(gnl_buf) != 1 && gnl_buf[ft_strlen(gnl_buf) - 1] == '\n')
		gnl_buf[ft_strlen(gnl_buf) - 1] = 0;
	while (is_map(gnl_buf))
	{
		free(gnl_buf);
		gnl_buf = get_next_line(map_fd);
		if (ft_strlen(gnl_buf) != 1 && gnl_buf[ft_strlen(gnl_buf) - 1] == '\n')
			gnl_buf[ft_strlen(gnl_buf) - 1] = 0;
	}
	i = 0;
	spawn_flag = 0;
	while (i < info->height)
	{
		if (ft_strlen(gnl_buf) != 1 && gnl_buf[ft_strlen(gnl_buf) - 1] == '\n')
			gnl_buf[ft_strlen(gnl_buf) - 1] = 0;
		if (is_map(gnl_buf))
			parse_err(INVALID_ELEM);
		k = 0;
		while (k < info->width)
		{
			if ((unsigned int)k < ft_strlen(gnl_buf))
				c = gnl_buf[k];
			else
				c = ' ';
			if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
			{
				if (spawn_flag)
					parse_err(INVALID_MAP);
				info->map[i][k] = set_spawning_point(c, info, i, k);
				spawn_flag = 1;
			}
			else
				info->map[i][k] = c;
			k++;
		}
		free(gnl_buf);
		gnl_buf = get_next_line(map_fd);
		i++;
	}
	if (!spawn_flag)
		parse_err(INVALID_MAP);
}

void	examine_map(t_info *info)
{
	int	x;
	int	y;

	x = -1;
	while (++x < info->height)
	{
		if (x == 0 || x == info->height - 1)
		{
			y = -1;
			while (++y < info->width)
			{
				if (info->map[x][y] == ' ')
					dfs(info, x, y);
				else if (info->map[x][y] != '1')
					parse_err(INVALID_MAP);
			}
		}
		else
		{
			if (info->map[x][0] == ' ')
				dfs(info, x, 0);
			else if (info->map[x][0] != '1')
				parse_err(INVALID_MAP);
			if (info->map[x][info->width - 1] == ' ')
				dfs(info, x, info->width - 1);
			else if (info->map[x][info->width - 1] != '1')
				parse_err(INVALID_MAP);
		}
	}
	//빈칸이 있는지 체크
}

void	parse(t_info *info, char *map_name)
{
	int		map_fd;

	map_fd = open(map_name, O_RDONLY);
	if (map_fd == -1)
		parse_err(CORRUPTED_MAP);
	get_info_until_map(map_fd, info);
	close(map_fd);
	map_fd = open(map_name, O_RDONLY);
	get_map(map_fd, info);
	examine_map(info);
}
