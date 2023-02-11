/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:58:36 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/11 15:56:30 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	*set_image(t_info *info, t_img *image, char *filename)
{
	int	width;
	int	height;

	image->ptr = mlx_xpm_file_to_image(info->mlx.ptr, filename,
			&width, &height);
	if (!image->ptr)
		return (NULL);
	image->data = mlx_get_data_addr(image->ptr, &(image->bpp),
			&(image->line_size), &(image->endian));
	if (!image->data)
		return (NULL);
	return (image->ptr);
}

int	is_invalid_rgb_val(int rgb_val[3])
{
	if (rgb_val[0] < 0 || rgb_val[0] > 255)
		return (0);
	if (rgb_val[1] < 0 || rgb_val[1] > 255)
		return (0);
	if (rgb_val[2] < 0 || rgb_val[2] > 255)
		return (0);
	return (1);
}

void	err(int err_code)
{
	write(2, "Error\n", 7);
	if (err_code == MALLOC_FAIL)
		write(2, "FATAL ERROR: MALLOC FAILED.\n", 29);
	else if (err_code == UNEXPECTED)
		write(2, "FATAL ERROR: UNEXPECTED BEHAVIOR.\n", 35);
	else if (err_code == INVALID_MAP_FORMAT)
		write(2, "parse error: invalid map format.\n", 34);
	else if (err_code == CORRUPTED_MAP)
		write(2, "parse error: corrupted map file.\n", 34);
	else if (err_code == CORRUPTED_TEXTURE)
		write(2, "parse error: corrupted texture file.\n", 38);
	else if (err_code == NOT_ENOUGH_ELEM)
		write(2, "parse error: not enough map elements.\n", 39);
	else if (err_code == DUPLICATED_ELEM)
		write(2, "parse error: duplicated element detected.\n", 43);
	else if (err_code == INVALID_ELEM)
		write(2, "parse error: invalid element detected.\n", 40);
	else if (err_code == INVAILD_RGB_VAL)
		write(2, "parse error: invalid floor/ceiling color val.\n", 47);
	else if (err_code == INVALID_MAP)
		write(2, "parse error: invalid map.\n", 27);
	else if (err_code == INVALID_ARGUMENT)
		write(2, "parse error: too less/many arguments.\n", 39);
	exit(1);
}

int	is_cardinal_texture(char *gnl_buf)
{
	if (!ft_strncmp(gnl_buf, "NO ", 3))
		return (1);
	else if (!ft_strncmp(gnl_buf, "SO ", 3))
		return (2);
	else if (!ft_strncmp(gnl_buf, "EA ", 3))
		return (3);
	else if (!ft_strncmp(gnl_buf, "WE ", 3))
		return (4);
	else
		return (0);
}
