/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:08:56 by hkong             #+#    #+#             */
/*   Updated: 2023/02/02 17:42:31 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	on_keydown(int keycode, t_info *info)
{
	if (keycode == BUTTON_ESC)
		on_destroy();
	else if (keycode == BUTTON_LEFT)
		rotate(info, 1);
	else if (keycode == BUTTON_RIGHT)
		rotate(info, -1);
	else if (keycode == BUTTON_W)
		move(info, 1, 1);
	else if (keycode == BUTTON_A)
		move(info, -1, 1);
	else if (keycode == BUTTON_S)
		move(info, -1, -1);
	else if (keycode == BUTTON_D)
		move(info, 1, -1);
	return (0);
}

int	on_destroy(void)
{
	exit(0);
	return (0);
}

void	rotate(t_info *info, int dir)
{
	double	old_dir_x;
	double	old_plane_x;
	double	speed;

	speed = ROTATE_SPEED * dir;
	old_dir_x = info->dir.x;
	info->dir.x = info->dir.x * cos(speed) - info->dir.y * sin(speed);
	info->dir.y = old_dir_x * sin(speed) + info->dir.y * cos(speed);
	old_plane_x = info->plane.x;
	info->plane.x = info->plane.x * cos(speed) - info->plane.y * sin(speed);
	info->plane.y = old_plane_x * sin(speed) + info->plane.y * cos(speed);
}

void	move(t_info *info, double dir_x, double dir_y)
{
	t_dvector	next_pos;

	if (dir_x * dir_y > 0)
	{
		next_pos.x = info->pos.x + info->dir.x * MOVE_SPEED * dir_x;
		next_pos.y = info->pos.y + info->dir.y * MOVE_SPEED * dir_y;
	}
	else
	{
		next_pos.x = info->pos.x + info->dir.y * MOVE_SPEED * dir_x;
		next_pos.y = info->pos.y + info->dir.x * MOVE_SPEED * dir_y;
	}
	if (worldMap[(int)next_pos.x][(int)next_pos.y] == '0')
	{
		info->pos.x = next_pos.x;
		info->pos.y = next_pos.y;
	}
}
