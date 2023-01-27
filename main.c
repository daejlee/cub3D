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

typedef struct s_info 
{
	void	*mlx;
	void	*mlx_win;

	double	posX;
	double	posY;
	double	dirX;
	double	dirY;
	double	planeX;
	double	planeY;
	double	old_time;
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

int	draw_map_test(t_info *info)
{
	(void)info;

	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
			mlx_pixel_put(info->mlx, info->mlx_win, i, j, 0x0000FF00);
	}
	return (0);
}

int	draw_map(t_info *info)
{
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		double	cameraX = 2 * i / (double)SCREEN_WIDTH - 1;
		double	rayDirX = info->dirX + info->planeX * cameraX;
		double	rayDirY = info->dirY + info->planeY * cameraX;

		int mapX = (int)info->posX;
		int	mapY = (int)info->posY;

		double	sideDistX;
		double	sideDistY;

		double	deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    	double	deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

		double	perpWallDist;

		int	stepX;
		int	stepY;

		int	hit = 0;
		int	side;

		if(rayDirX < 0)
    	{
        	stepX = -1;
        	sideDistX = (info->posX - mapX) * deltaDistX;
    	}
    	else
    	{
    		stepX = 1;
    		sideDistX = (mapX + 1.0 - info->posX) * deltaDistX;
    	}
    	if(rayDirY < 0)
    	{
        	stepY = -1;
        	sideDistY = (info->posY - mapY) * deltaDistY;
    	}
    	else
    	{
        	stepY = 1;
        	sideDistY = (mapY + 1.0 - info->posY) * deltaDistY;
    	}

		//perform DDA
    	while(hit == 0)
    	{
        	//jump to next map square, either in x-direction, or in y-direction
        	if(sideDistX < sideDistY)
        	{
        		sideDistX += deltaDistX;
        		mapX += stepX;
        		side = 0;
    		}
     	   else
    		{
    			sideDistY += deltaDistY;
        		mapY += stepY;
    			side = 1;
			}
    		//Check if ray has hit a wall
			if(worldMap[mapX][mapY] > 0) hit = 1;
		}

		if(side == 0) perpWallDist = (sideDistX - deltaDistX);
    	else          perpWallDist = (sideDistY - deltaDistY);

    	int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

		int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
    	if(drawStart < 0) drawStart = 0;
    	int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
    	if(drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
		for (int pixel = drawStart; pixel <= drawEnd; pixel++)
			mlx_pixel_put(info->mlx, info->mlx_win, i, pixel, 0x0000FF00);
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
	info->posX = 12.5;
	info->posX = 12.5;
	/* 플레이어 방향. 역시 맵에서 읽어서 저장해야 함 */
	info->dirX = -1;
	info->dirY = 0;
	/* 카메라 평면 벡터. 플레이어 방향에 맞춰 변해야 함 */
	info->planeX = 0;
	info->planeY = 0.66;
	/* time of previous frame */
	info->old_time = 0;

	/* mlx 초기화 및 hook */
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
	info->mlx = mlx;
	info->mlx_win = mlx_win;
	mlx_hook(mlx_win, ON_DESTROY_EVENT, 0, on_destroy, NULL);
	mlx_hook(mlx_win, ON_KEYDOWN_EVENT, 0, on_keydown, NULL);
	mlx_loop_hook(mlx, draw_map, info);
	mlx_loop(mlx);
	return (0);
}