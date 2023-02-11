/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:58:32 by daejlee           #+#    #+#             */
/*   Updated: 2023/02/11 15:37:39 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse(t_info *info, char *map_name)
{
	int	map_fd;

	check_map_format(map_name);
	map_fd = open(map_name, O_RDONLY);
	if (map_fd == -1)
		err(CORRUPTED_MAP);
	get_info_until_map(map_fd, info);
	close(map_fd);
	map_fd = open(map_name, O_RDONLY);
	get_map(map_fd, info);
	examine_map(info);
}
