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
	t_manager *mngr;
	t_double3 *v;
	t_img	*img;

	mlx = *(((t_mlx**)param)[2]);
	ocl = ((t_ocl**)param)[0];
	mngr = ((t_manager**)param)[1];
	img = &mngr->imgs[mngr->cur_img];
	v = &mngr->imgs[mngr->cur_img].start;
	printf("redraw %p float3(%f, %f, %f) %f\n",v, v->x, v->y, v->z, v->z * 1024);
	ft_ocl_make_img(img, ocl);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], img->img_ptr, 0, 0);
}

int		hook_keydwn(int key, void *param)
{
	t_manager *mngr;


	mngr = ((t_manager**)param)[1];
	printf("key pressed = %d\n", key);
	printf("nan test %d\n", (int) (0.f/0.f));
	if (key == 53)
		exit(0);
	if (key == 24 || key == 27)
		mngr->imgs[mngr->cur_img].mult += (key == 24) ? .01 : -.01;
	ft_redraw(param);
	return (0);
}

int mouse_move_handle(int x, int y, void *param)
{
	t_manager *mngr;
	t_double3	*v;

	mngr = ((t_manager**)param)[1];
	v = &mngr->imgs[mngr->cur_img].start;
	if(mngr->mouse_mask & (1 << 0))
	{
		v->x += (FT_ABS(mngr->mse_mv_crd[0].x - x) > 0) ?
				(mngr->mse_mv_crd[0].x - x) * v->z : 0;
		v->y +=	(FT_ABS(mngr->mse_mv_crd[0].y - y) > 0) ?
				   -(mngr->mse_mv_crd[0].y - y) * v->z : 0;
		mngr->mse_mv_crd[0] = (t_int2){x, y};
		ft_redraw(param);
	}
	if(mngr->mouse_mask & (1 << 1))
		printf("m2 pressed move: x = %d, y = %d\n", x, y);
	if(mngr->mouse_mask & (1 << 2))
		printf("m3 pressed move: x = %d, y = %d\n", x, y);
	return (0);
}

int		mouse_release(int but, int x, int y, void *param)
{
	t_manager *mngr;

	mngr = ((t_manager**)param)[1];
	if (but == 3 || but == 2 || but == 1)
		mngr->mouse_mask &= 0 << but;
	return (0);
}

int		mouse_hook(int but, int x, int y, void *param)
{
	t_double3 *v;
	t_manager *mngr;

	printf("%d\n", but);
	mngr = ((t_manager**)param)[1];
	v = &mngr->imgs[mngr->cur_img].start;
	if (but == 3 || but == 2 || but == 1)
	{
		mngr->mouse_mask |= 1 << (but - 1);
		mngr->mse_mv_crd[but - 1] = (t_int2){x, y};
	}
	printf("mouse_mask = %d\n", mngr->mouse_mask);
	if (but == 4 || but == 5)
	{
		if (but == 4)
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
		printf("test %p float3(%f, %f, %f) %f\n",v, v->x, v->y, v->z, v->z * 1024);
		ft_redraw(param);
	}
	return (0);
}
