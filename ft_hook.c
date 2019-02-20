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

void ft_redraw(void *param)
{
	t_mlx mlx;
	t_ocl *ocl;
	t_img *img;
	t_double3 *v;

	mlx = *(((t_mlx**)param)[2]);
	ocl = ((t_ocl**)param)[0];
	img = ((t_img**)param)[1];
	v = &((t_img**)param)[1]->start;
	ft_ocl_make_img(img, ocl);
	printf("float3(%f, %f, %f) %f\n", v->x, v->y, v->z, v->z * 1024);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], img->img_ptr, 0, 0);
}

int		hook_keydwn(int key, void *param)
{

	printf("key pressed = %d\n", key);
	printf("nan test %d\n", (int) (0.f/0.f));
	if (key == 53)
		exit(0);
	if (key == 24 || key == 27)
		((t_img**)param)[1]->mult += (key == 24) ? .01 : -.01;
	ft_redraw(param);
	return (0);
}

int		mouse_hook(int but, int x, int y, void *param)
{
	t_double3 *v;

	v = &((t_img**)param)[1]->start;
	printf("mouse_hook\n");
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
		}
		else
		{
			v->x = v->x + x * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->y =  v->y - y * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->z *= MOUSE_SCROL_SCALE;
		}
	}
	ft_redraw(param);
	return (0);
}
