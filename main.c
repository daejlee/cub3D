#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <mlx.h>

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
# define MAP_WIDTH 24
# define MAP_HEIGHT 24

# define BUTTON_W 13
# define BUTTON_A 0
# define BUTTON_S 1
# define BUTTON_D 2
# define BUTTON_ESC 53
# define ON_KEYDOWN_EVENT 2
# define ON_DESTROY_EVENT 17

typedef struct s_mlx
{
	void	*ptr;
	void	*win_ptr;
}	t_mlx;

typedef struct s_dvector
{
	double	x;
	double	y;
}	t_dvector;

typedef struct s_ivector
{
	int	x;
	int	y;
}	t_ivector;

typedef struct s_info
{
	t_mlx		mlx;
	t_dvector	pos;
	t_dvector	dir;
	t_dvector	plane;
	double		old_time;
}	t_info;

char worldMap[MAP_WIDTH][MAP_HEIGHT]=
{
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','1','1','1','1','1','0','0','0','0','1','0','1','0','1','0','0','0','1'},
  {'1','0','0','0','0','0','1','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','1','0','0','0','1','0','0','0','0','1','0','0','0','1','0','0','0','1'},
  {'1','0','0','0','0','0','1','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','1','1','0','1','1','0','0','0','0','1','0','1','0','1','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','N','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','1','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','1','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','0','0','0','1','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','1','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','1','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
  {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
};

int	on_destroy(void)
{
	exit(0);
	return (0);
}

int	on_keydown(int keycode)
{
	if (keycode == BUTTON_ESC)
		exit(0);
	return (0);
}

int	draw_map(t_info *info)
{
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		t_dvector	ray;
		t_dvector	side_dist;
		t_dvector	delta_dist;
		t_ivector	map;
		t_ivector	step;
		double		perpWallDist;
		double		screen_x;
		int			hit = 0;
		int			side;
		
		screen_x = 2 * i / (double)SCREEN_WIDTH - 1;
		ray.x = info->dir.x + info->plane.x * screen_x;
		ray.y = info->dir.y + info->plane.y * screen_x;

		map.x = (int)info->pos.x;
		map.y = (int)info->pos.y;

		delta_dist.x = (ray.x == 0) ? 1e30 : fabs(1 / ray.x);
    	delta_dist.y = (ray.y == 0) ? 1e30 : fabs(1 / ray.y);

		if (ray.x < 0)
    	{
        	step.x = -1;
        	side_dist.x = (info->pos.x - map.x) * delta_dist.x;
    	}
    	else
    	{
    		step.x = 1;
    		side_dist.x = (map.x + 1.0 - info->pos.x) * delta_dist.x;
    	}
    	if (ray.y < 0)
    	{
        	step.y = -1;
        	side_dist.y = (info->pos.y - map.y) * delta_dist.y;
    	}
    	else
    	{
        	step.y = 1;
        	side_dist.y = (map.y + 1.0 - info->pos.y) * delta_dist.y;
    	}

		//perform DDA
    	while (hit == 0)
    	{
        	//jump to next map square, either in x-direction, or in y-direction
        	if(side_dist.x < side_dist.y)
        	{
        		side_dist.x += delta_dist.x;
        		map.x += step.x;
        		side = 0;
    		}
     	   else
    		{
    			side_dist.y += delta_dist.y;
        		map.y += step.y;
    			side = 1;
			}
    		//Check if ray has hit a wall
			if(worldMap[map.x][map.y] > '0') hit = 1;
		}

		if (side == 0) perpWallDist = (side_dist.x - delta_dist.x);
    	else          perpWallDist = (side_dist.y - delta_dist.y);

    	int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

		int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
    	if(drawStart < 0) drawStart = 0;
    	int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
    	if(drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
		int	color;
		if (side == 0)
			color = 0x0000FF00;
		else
			color = 0x000000FF;
		for (int pixel = drawStart; pixel <= drawEnd; pixel++)
			mlx_pixel_put(info->mlx.ptr, info->mlx.win_ptr, i, pixel, color);
	}
	return (0);
}

int	main()
{
	void	*mlx;
	void	*mlx_win;
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	/* 플레이어 위치. 맵에서 읽어서 저장해야 함 */
	info->pos.x = 12.5;
	info->pos.y = 12.5;
	/* 플레이어 방향. 역시 맵에서 읽어서 저장해야 함 */
	info->dir.x = -1;
	info->dir.y = 0;
	/* 카메라 평면 벡터. 플레이어 방향에 맞춰 변해야 함 */
	info->plane.x = 0;
	info->plane.y = 0.66;
	/* time of previous frame */
	info->old_time = 0;

	/* mlx 초기화 및 hook */
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
	info->mlx.ptr = mlx;
	info->mlx.win_ptr = mlx_win;
	mlx_hook(mlx_win, ON_DESTROY_EVENT, 0, on_destroy, NULL);
	mlx_hook(mlx_win, ON_KEYDOWN_EVENT, 0, on_keydown, NULL);
	mlx_loop_hook(mlx, draw_map, info);
	mlx_loop(mlx);
	return (0);
}