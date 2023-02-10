/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 18:30:20 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/10 18:30:53 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_cardinal_texture(t_info *info, char *gnl_buf, int *task_cnt_adr)
{
	int		code;
	char	**temp_arr;

	code = is_cardinal_texture(gnl_buf) - 1;
	if (info->wall_img[code].ptr)
		err(DUPLICATED_ELEM);
	temp_arr = ft_split(gnl_buf, ' ');
	if (!temp_arr)
		err(MALLOC_FAIL);
	if (!temp_arr[1] || temp_arr[2])
		err(INVALID_ELEM);
	if (!set_image(info, info->wall_img + code, temp_arr[1]))
		err(CORRUPTED_TEXTURE);
	free_arr(temp_arr);
	(*task_cnt_adr)--;
}

static char	**prep_n_check_err(char *gnl_buf)
{
	char	**temp_arr;
	char	**color_arr;

	temp_arr = ft_split(gnl_buf, ' ');
	if (!temp_arr)
		err(MALLOC_FAIL);
	else if (!temp_arr[1] || temp_arr[2])
		err(INVALID_ELEM);
	color_arr = ft_split(temp_arr[1], ',');
	if (!color_arr)
		err(MALLOC_FAIL);
	else if (!color_arr[2] || color_arr[3])
		err(INVALID_ELEM);
	free_arr(temp_arr);
	return (color_arr);
}

void	get_floor_ceiling_color(t_info *info, char *gnl_buf, int *task_cnt_adr)
{
	int		code;
	int		rgb_val[3];
	char	**color_arr;

	code = is_floor_ceiling_color(gnl_buf);
	color_arr = prep_n_check_err(gnl_buf);
	rgb_val[0] = ft_atoi(color_arr[0]);
	rgb_val[1] = ft_atoi(color_arr[1]);
	rgb_val[2] = ft_atoi(color_arr[2]);
	free_arr(color_arr);
	if (!is_invalid_rgb_val(rgb_val))
		err(INVAILD_RGB_VAL);
	if (code == 1)
	{
		if (info->floor != -1)
			err(DUPLICATED_ELEM);
		info->floor = get_rgb_val(rgb_val);
	}
	else
	{
		if (info->ceil != -1)
			err(DUPLICATED_ELEM);
		info->ceil = get_rgb_val(rgb_val);
	}
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
			err(NOT_ENOUGH_ELEM);
		gnl_buf = remove_tailing_nl(gnl_buf);
		if (is_cardinal_texture(gnl_buf))
			get_cardinal_texture(info, gnl_buf, &task_cnt);
		else if (is_floor_ceiling_color(gnl_buf))
			get_floor_ceiling_color(info, gnl_buf, &task_cnt);
		else if (!is_map(gnl_buf))
		{
			if (task_cnt)
				err(NOT_ENOUGH_ELEM);
			get_map_slots(info, gnl_buf, map_fd);
			break ;
		}
		else if (gnl_buf[0] != '\n')
			err(INVALID_ELEM);
		free(gnl_buf);
	}
}
