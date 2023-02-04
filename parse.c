#include "cub3d.h"
#include "libft.h"
#include "./libft_garage/gnl/get_next_line.h"
#include <fcntl.h>

void	*get_cardinal_texture(t_info *info, char *gnl_buf)
{
	int		code;
	char	**temp_arr;

	code = is_cardinal_texture(gnl_buf) - 1;
	temp_arr = ft_split(gnl_buf, ' ');
	if (!temp_arr)
		parse_err("fatal error: malloc failed.\n");
	if (!set_image(info, info->wall + code, temp_arr[1]))
		parse_err("corrupted texture file.\n");
	free_arr(temp_arr);
	return (gnl_buf);
}

void	*get_floor_ceiling_color(t_info *info, char *gnl_buf)
{
	int		code;
	char	**temp;
	char	**color_arr;
	int		rgb_val[3];

	code = is_floor_ceiling_color(gnl_buf);
	temp = ft_split(gnl_buf, ' ');
	color_arr = ft_split(temp[1], ',');
	if (!temp || !color_arr)
		parse_err("fatal error: malloc failed.\n");
	rgb_val[0] = ft_atoi(color_arr[0]);
	rgb_val[1] = ft_atoi(color_arr[1]);
	rgb_val[2] = ft_atoi(color_arr[2]);
	if (!is_invalid_rgb_val(color_arr))
		parse_err("invalid floor/ceiling color val.\n");
	if (code == 1)
		info->floor = get_rgb_val(color_arr);
	else
		info->ceil = get_rgb_val(color_arr);
}

char	**parse(t_info *info, char *map_name)
{
	int		map_fd;
	char	*gnl_buf;

	map_fd = open(map_name, O_RDONLY);
	if (map_fd == -1)
		parse_err("corrupted map file.\n");
	gnl_buf = NULL;
	while (1)
	{
		gnl_buf = get_next_line(map_fd);
		if (!gnl_buf)
			break ;
		else if (is_cardinal_texture(gnl_buf))
			get_cardinal_texture(info, gnl_buf);
		else if (is_floor_ceiling_color(gnl_buf))
			get_floor_ceiling_color(info, gnl_buf);
		else if (is_map(gnl_buf))
		{
			if (!is_info_in_order());
		}
		free(gnl_buf);
	}
}
