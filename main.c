#include "cub3d.h"
#include "mlx.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>

typedef struct s_cub3d
{
	void	*mp;
	void	*wp;

	double	pos_x;		//x and y start position
	double	pos_y;
	double	dir_x;		//initial direction vector
	double	dir_y;
	double	plane_x;	//the 2d raycaster version of camera plane
	double	plane_y;
	double	time;		//time of current frame
	double	old_time;	//time of previous frame
	double	camera_x;	//x_coordinate in camera space
	double	raydir_x;	//dirX +- planeX
	double	raydir_y;	//dirY +- planeY
	int		map_x;		//박스 x, y 정수 좌표
	int		map_y;

	/*** DDA vars ***/
	double	side_dist_x;	//length of ray from current position to next x or y-side
	double	side_dist_y;
	double	del_dist_x;		//length of ray from one x or y-side to next x or y-side
	double	del_dist_y;
	int		step_x;			//what direction to step in x or y-direction (either +1 or -1)
	int		step_y;
	int		hit_flag;		//was there a wall hit?
	int		side;			//was a NS or a EW wall hit?

	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		color;
}	t_cub3d;

t_cub3d	init_p(t_cub3d *p)
{
	p->pos_x = 22;
	p->pos_y = 12;
	p->dir_x = -1;
	p->dir_y = 0;
	p->plane_x = 0;
	p->plane_y = 0.66;
	p->time = 0;
	p->old_time = 0;
}

int main(int argc, char *argv[])
{
	t_cub3d	p;
	int		x;
	
	p = init_p(&p);
	p.mp = mlx_init();
	p.wp = mlx_new_window(p.mp, screenWidth, screenHeight, "cub3D");
	int	temp = 0;
	while(temp < screenWidth)
		mlx_pixel_put(p.mp, p.wp, temp++, 240, 255);
	x = 0;
	while(x < screenWidth)
	{
		//calculate ray position and direction
		p.camera_x = 2 * x / (double)screenWidth - 1;
		printf("camera_x is %f\n", p.camera_x);
		p.raydir_x = p.dir_x + p.plane_x * p.camera_x;
		p.raydir_y = p.dir_y + p.plane_y * p.camera_x;
		p.map_x = (int)p.pos_x;
		p.map_y = (int)p.pos_y;
		p.del_dist_x = fabs(1 / p.raydir_x);
		p.del_dist_y = fabs(1 / p.raydir_y);
		p.hit_flag = 0;
		//calculate step and initial sideDist
		if (p.raydir_x < 0)
		{
			p.step_x = -1;
			p.side_dist_x = (p.pos_x - p.map_x) * p.del_dist_x;
		}
		else
		{
			p.step_x = 1;
			p.side_dist_x = (p.map_x + 1 - p.pos_x) * p.del_dist_x;
		}
		if (p.raydir_y < 0)
		{
			p.step_y = -1;
			p.side_dist_y = (p.pos_y - p.map_y) * p.del_dist_y;
		}
		else
		{
			p.step_y = 1;
			p.side_dist_y = (p.map_y + 1 - p.pos_y) * p.del_dist_y;
		}
		p.hit_flag = 0;
		while (!p.hit_flag)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (p.side_dist_x < p.side_dist_y)
			{
				p.side_dist_x += p.del_dist_x;
				p.map_x += p.step_x;
				p.side = 0;
			}
			else
			{
				p.side_dist_y += p.del_dist_y;
				p.map_y += p.step_y;
				p.side = 1;
			}
			if (worldMap[p.map_x][p.map_y])
				p.hit_flag = 1;
		}
		//DDA done.
		//Calculate distance projected on camera direction
		if (!p.side)
			p.perp_wall_dist = (p.map_x - p.pos_x + ((1 - p.step_x) / 2)) / p.raydir_x; //방향벡터로 나누면 벡터의 크기가 나온다.
		else
			p.perp_wall_dist = (p.map_y - p.pos_y + ((1 - p.step_y) / 2)) / p.raydir_y;
		p.line_height = (int)(screenHeight / p.perp_wall_dist);
		p.draw_start = -p.line_height / 2 + screenHeight / 2;
		if (p.draw_start < 0)
			p.draw_start = 0;
		p.draw_end = p.line_height / 2 + screenHeight / 2;
		if (p.draw_end >= screenHeight)
			p.draw_end = screenHeight - 1;
		p.color = 255;
		// if (worldMap[p.map_x][p.map_y] == 1)
		// 	p.color = RGB_Red;
		// else if (worldMap[p.map_x][p.map_y] == 2)
		// 	p.color = RGB_Green;
		// else if (worldMap[p.map_x][p.map_y] == 3)
		// 	p.color = RGB_Blue;
		// else if (worldMap[p.map_x][p.map_y] == 4)
		// 	p.color = RGB_White;
		// else
		// 	p.color = RGB_Yellow;
		// if (p.side == 1)
		// 	p.color /= 2;
		mlx_clear_window(p.mp, p.wp);
		printf("%i and %i\n", p.draw_start, p.draw_end);
		while (p.draw_start <= p.draw_end)
			mlx_pixel_put(p.mp, p.wp, x, p.draw_start++, p.color);
		x++;
	}
	mlx_loop(p.mp);
	return (0);
}
