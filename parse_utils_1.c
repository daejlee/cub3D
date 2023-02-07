/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:58:36 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/07 17:08:14 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include "get_next_line.h"
#include <fcntl.h>

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	*set_image(t_info *info, t_img *image, char *filename)
{
	int	width;
	int	height;

	image->ptr = mlx_xpm_file_to_image(info->mlx.ptr, filename, &width, &height);
	image->data = mlx_get_data_addr(image->ptr, &(image->bpp), &(image->line_size), &(image->endian));
	if (!image->ptr || !image->data)
		return (NULL);
	return (image->ptr);
}

int		is_invaild_rgb_val(int rgb_val[3])
{
	if (rgb_val[0] < 0 || rgb_val[0] > 255)
		return (0);
	if (rgb_val[1] < 0 || rgb_val[1] > 255)
		return (0);
	if (rgb_val[2] < 0 || rgb_val[2] > 255)
		return (0);
	return (1);
}

void	parse_err(int err_code)
{
	if (!err_code)
		write(2, "FATAL ERROR: MALLOC FAILED.", 28);
	write(2, "parse error: ", 14);
	if (err_code == CORRUPTED_MAP)
		write(2, "corrupted map file.", 20);
	else if (err_code == CORRUPTED_TEXTURE)
		write(2, "corrupted texture file.", 24);
	else if (err_code == NOT_ENOUGH_ELEM)
		write(2, "not enough map elements.", 25);
	else if (err_code == INVALID_ELEM)
		write(2, "invalid element detedted.", 26);
	else if (err_code == INVAILD_RGB_VAL)
		write(2, "invalid floor/ceiling color val.", 33);
	exit(1);
}

int		is_cardinal_texture(char *gnl_buf)
{
	if (!ft_strncmp(gnl_buf, "NO ", 3))
		return (1);
	else if (!ft_strncmp(gnl_buf, "SO ", 3))
		return (2);
	else if (!ft_strncmp(gnl_buf, "WE ", 3))
		return (3);
	else if (!ft_strncmp(gnl_buf, "EA ", 3))
		return (4);
	else
		return (0);
}
