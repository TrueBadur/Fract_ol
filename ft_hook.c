/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hook.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 19:44:47 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/01/18 18:13:44 by bparker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <stdio.h>

int		hook_keydwn(int key, void *param)
{

	param = NULL;
	if (key == 53)
		exit(0);
	return (0);
}

int		mouse_hook(int but, int x, int y, void *param)
{
	t_double3 *v;
	t_mlx mlx;

	mlx = *(((t_mlx**)param)[3]);
	v = ((t_double3**)param)[0];
	if (but == 4 || but == 5 || but == 3)
	{
		printf("%d\n", but);
		if (but == 3)
		{
			v->x = v->x + (x / 2.0 / 1024 * 4 + 2);
			v->y = v->y + (y / 2.0 / 1024 * 4 - 2);
			printf("%f, %f\n", v->x, v->y);
		}
		else if (but == 4)
		{
			v->x = v->x + x * (v->z - v->z / MOUSE_SCROL_SCALE);
			v->y =  v->y - y * (v->z - v->z / MOUSE_SCROL_SCALE);
			v->z /= MOUSE_SCROL_SCALE;
			printf("test %p\n", ((t_ocl**)param)[2]->device);
			ft_ocl_make_img(((t_img**)param)[2], ((t_ocl**)param)[1], *v);
			printf("test\n");
			mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], ((t_img**)param)[2]->img_ptr, 0, 0);

			printf("float3(%f, %f, %f) %f\n", v->x, v->y, v->z, v->z * 1024);
		}
		else
		{
			v->x = v->x + x * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->y =  v->y - y * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->z *= MOUSE_SCROL_SCALE;
			ft_ocl_make_img(((t_img**)param)[2], ((t_ocl**)param)[1], *v);
			printf("float3(%f, %f, %f) %f\n", v->x, v->y, v->z, v->z * 1024);
			mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], ((t_img**)param)[2]->img_ptr, 0, 0);
		}

	}
	return (0);
}
