/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkong <hkong@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:12:32 by hkong             #+#    #+#             */
/*   Updated: 2023/02/08 16:44:06 by hkong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_pixel(t_img image, int x, int y, unsigned int color)
{
	*(unsigned int *)(image.data + \
						x * image.line_size + y * (image.bpp / 8)) = color;
}

unsigned int	get_pixel(t_img image, int x, int y)
{
	unsigned int	color;

	color = *(unsigned int *)(image.data \
								+ x * image.line_size + y * (image.bpp / 8));
	return (color);
}
