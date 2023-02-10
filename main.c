/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:06:27 by hkong             #+#    #+#             */
/*   Updated: 2023/02/10 18:48:33 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_info	*info;

	if (argc != 2)
	{
		write(2, "too many/less arg.", 19);
		return (1);
	}
	info = init_info();
	info->mlx.ptr = mlx_init();
	info->mlx.win_ptr = mlx_new_window(info->mlx.ptr, \
										SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
	info->screen.ptr = NULL;
	parse(info, argv[1]);
	mlx_hook(info->mlx.win_ptr, ON_DESTROY_EVENT, 0, on_destroy, NULL);
	mlx_hook(info->mlx.win_ptr, ON_KEYDOWN_EVENT, 0, on_keydown, info);
	mlx_loop_hook(info->mlx.ptr, draw_map, info);
	mlx_loop(info->mlx.ptr);
	return (0);
}
