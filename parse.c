#include "cub3d.h"
#include "libft.h"
#include "get_next_line.h"
#include <fcntl.h>

void	get_cardinal_texture(t_info *info, char *gnl_buf)
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
}

int	get_rgb_val(int rgb_val[3])
{
	int	res;

	res = rgb_val[2];
	res += (rgb_val[1] * 16 * 16);
	res += (rgb_val[0] * 16 * 16 * 16 * 16);
	return (res);
}

void	get_floor_ceiling_color(t_info *info, char *gnl_buf)
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
}

void	get_map_slots(t_info *info, char *gnl_buf, int map_fd)
{
	int	width;
	int	height;
	int	i;

	width = ft_strlen(gnl_buf);
	height = 1;
	while (1)
	{
		free(gnl_buf);
		gnl_buf = get_next_line(map_fd);
		if (!gnl_buf)
			break ;
		if (ft_strlen(gnl_buf) > width)
			width = ft_strlen(gnl_buf);
		height++;
	}
	info->map = (char **)malloc(sizeof(char *) * width);
	if (!info->map)
		parse_err(MALLOC_FAIL);
	i = 0;
	while (i < width)
	{
		info->map[i++] = (char *)malloc(sizeof(char) * height);
		if (!info->map[i - 1])
			parse_err(MALLOC_FAIL);
	}
}

char	**parse(t_info *info, char *map_name)
{
	int		map_fd;
	char	*gnl_buf;
	int		task_cnt;

	map_fd = open(map_name, O_RDONLY);
	if (map_fd == -1)
		parse_err(CORRUPTED_MAP);
	gnl_buf = NULL;
	task_cnt = 6;
	while (1)
	{
		gnl_buf = get_next_line(map_fd);
		if (!gnl_buf)
			parse_err(NOT_ENOUGH_ELEM);
		else if (is_cardinal_texture(gnl_buf))
		{
			task_cnt--;
			get_cardinal_texture(info, gnl_buf);
		}
		else if (is_floor_ceiling_color(gnl_buf))
		{
			task_cnt--;
			get_floor_ceiling_color(info, gnl_buf);
		}
		else if (is_map(gnl_buf))
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
