/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bparker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/23 11:17:51 by bparker           #+#    #+#             */
/*   Updated: 2019/01/14 20:33:02 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "fractol.h"

/*int			img_to_win(void *param)
{
	t_img		img;
	t_mlx		*mlx;

	mlx = (t_mlx *)(((int **)param)[2]);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, img.img_ptr, 0, 0);
	mlx_destroy_image(mlx->mlx_ptr, img.img_ptr);
	return (0);
}*/

/*t_mlx	*set_mlx(t_mlx *mlx)
{
}*/

void ft_mlx_create_new_window(t_mlx *mlx, t_vec2 res, const char *name)
{
	mlx->cw += mlx->wc ? 1 : 0;
	mlx->res[mlx->cw] = res;
	mlx->win_ptr[mlx->cw] = mlx_new_window(mlx->mlx_ptr, mlx->res[mlx->cw].x,
										 mlx->res[mlx->cw].y, MW_NAME);
	mlx->wc++;
}

static void init_mlx(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	mlx->wc = 0;
	mlx->cw = 0;
}

int			main(int ac, char **av)
{
	t_mlx		mlx;

	init_mlx(&mlx);	
	ft_mlx_create_new_window(&mlx, (t_vec2){1024, 1024}, "test");
	//create mandelbrot img
	//put img in window
	mlx_hook(mlx.win_ptr[mlx.cw], 2, 5, hook_keydwn, NULL);
	//mlx_mouse_hook(mlx.win_ptr, &mouse_hook, NULL);
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
