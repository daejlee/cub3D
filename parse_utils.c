#include "./include/cub3d.h"
#include "./libft_garage/libft/libft.h"
#include "./libft_garage/gnl/get_next_line.h"
#include <fcntl.h>

void	*set_image(t_info *info, t_img *image, char *filename)
{
	int	width;
	int	height;

	//todo: file 존재/형식 올바른지 확인
	image->ptr = mlx_xpm_file_to_image(info->mlx.ptr, filename, &width, &height);
	image->data = mlx_get_data_addr(image->ptr, &(image->bpp), &(image->line_size), &(image->endian));
	if (!image->ptr || !image->data)
		return (NULL);
}

void	*parse_err(char *err_message)
{
	write(2, err_message, ft_strlen(err_message));
	//free
	return (NULL);
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

int		is_floor_ceiling_color(char *gnl_buf)
{
	if (!ft_strncmp(gnl_buf, "F ", 2))
		return (1);
	else if (!ft_strncmp(gnl_buf, "C ", 2))
		return (2);
	else
		return (0);
}
