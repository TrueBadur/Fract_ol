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

void put_info(void *param)
{
	t_mlx mlx;
	t_manager *mngr;
	char 	t[5][50];
	t_img	img;

	mngr = ((t_manager**)param)[1];
	img = mngr->imgs[mngr->cur_img];
	mlx = *(((t_mlx**)param)[2]);
	sprintf(t[0], "Number of iterations: %zu, %d", img.iter, img.iter_mod);
	sprintf(t[1], "Base color: (%f, %f, %f)", img.col.x, img.col.y, img.col.z);
	sprintf(t[2], "Scale: 1:%.0f", (float)(1 / img.start.z));
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr->imgs[INFO_I].img_ptr, 0, 0);
	mlx_string_put(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], 10, 2, 0x000000ff, t[0]);
	mlx_string_put(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], 380, 2, 0x000000ff, t[1]);
	mlx_string_put(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], 850, 2, 0x000000ff, t[2]);
}

size_t ft_get_iters(t_double3 start, int iter_mod)
{
	size_t  ret;
	int x;

	x = 1 / start.z;
	ret = log((x - 250) / 100 + 2) * 200 + iter_mod;
	ret = ret > 160000 ? 160000 : ret;
	ret = ret < 30 || start.z > 0.01 ? 30 : ret;
	return (ret);
}

void ft_redraw(void *param)
{
	t_mlx mlx;
	t_manager *mngr;
	t_img	*img;

	mlx = *(((t_mlx**)param)[2]);
	mngr = ((t_manager**)param)[1];
	img = &mngr->imgs[mngr->cur_img];
	img->iter = ft_get_iters(img->start, img->iter_mod);
	ft_ocl_make_img(img, ((t_ocl**)param)[0]);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], img->img_ptr, 0, 0);
	if (mngr->info)
		put_info(param);
}

int		hook_keyrelease(int key, void *param)
{
	t_manager *mngr;

	mngr = ((t_manager**)param)[1];
	if (SHIFT || CMND || CNTRL)
	{
		mngr->key_mask &= SHIFT ? SHIFT_RLS : 0;
		mngr->key_mask &= CNTRL ? CNTRL_RLS : 0;
		mngr->key_mask &= CMND ? CMND_RLS : 0;
	}
	return (0);
}

int		hook_keydwn(int key, void *param)
{
	t_manager *mngr;
	t_mlx		*mlx;

	mngr = ((t_manager**)param)[1];
	mlx = ((t_mlx**)param)[2];
	printf("key pressed = %d\n", key);
	printf("key mask = %d\n", mngr->key_mask);
	printf("CNTRL? %d, SHIFT? %d\n", IS_CNTRL_DOWN, IS_SHIFT_DOWN);
	if (key == 53)
		exit(0);
	if (ITERS_PLUS || ITERS_MINUS)
	{
		if (!IS_CNTRL_DOWN && !IS_SHIFT_DOWN)
			mngr->imgs[mngr->cur_img].iter_mod += ITERS_PLUS ? 1 : -1;
		mngr->imgs[mngr->cur_img].iter_mod += IS_CNTRL_DOWN ? (ITERS_PLUS) * 100 + (ITERS_MINUS) * -100 : 0;
		mngr->imgs[mngr->cur_img].iter_mod += IS_SHIFT_DOWN ? (ITERS_PLUS) * 10 + (ITERS_MINUS) * -10 : 0;
	}
	if (SHIFT || CMND || CNTRL)
	{
		mngr->key_mask |= SHIFT ? SHIFT_DOWN : 0;
		mngr->key_mask |= CNTRL ? CNTRL_DOWN : 0;
		mngr->key_mask |= CMND ? CMND_DOWN : 0;
	}
	if (INFO)
	{
		if (!mngr->info)
		{
			mlx->res[INFO_I] = INFO_I_RES;
			mngr->imgs[INFO_I].img_ptr = mlx_new_image(mlx->mlx_ptr, mlx->res[INFO_I].x, mlx->res[INFO_I].y);
		}
		else
			mlx_destroy_image(mlx->mlx_ptr, mngr->imgs[INFO_I].img_ptr);
		mngr->info = ~(mngr->info);
	}
	if (RESTART)
	{
		mngr->imgs[mngr->cur_img].start = (t_double3){-2.15, 1.5, 3.0 / RES};
		mngr->imgs[mngr->cur_img].col = (t_float3){0.0, 0.6, 1.0};
	}
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
		v->x += mngr->mse_mv_crd[0].x != x ?
				(mngr->mse_mv_crd[0].x - x) * v->z : 0;
		v->y -=	(mngr->mse_mv_crd[0].y != y) ?
				   -(mngr->mse_mv_crd[0].y - y) * v->z : 0;
		mngr->mse_mv_crd[0] = (t_int2){x, y};
		ft_redraw(param);
	}
	if(mngr->mouse_mask & (1 << 1))
	{
		if (IS_SHIFT_DOWN)
			mngr->imgs[mngr->cur_img].col.z = (float)x / RES;
		else
		{
			mngr->imgs[mngr->cur_img].col.x = (float) x / RES;
			mngr->imgs[mngr->cur_img].col.y = (float) y / RES;
		}
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

	mngr = ((t_manager**)param)[1];
	v = &mngr->imgs[mngr->cur_img].start;
	if (y > 0 && (but == 3 || but == 2 || but == 1))
	{
		mngr->mouse_mask |= 1 << (but - 1);
		mngr->mse_mv_crd[but - 1] = (t_int2){x, y};
	}
	if (but == 4 || but == 5)
	{
		if (but == 4)
		{
			v->x = v->x + x * (v->z - v->z / MOUSE_SCROL_SCALE);
			v->y =  v->y + y * (v->z - v->z / MOUSE_SCROL_SCALE);
			v->z /= MOUSE_SCROL_SCALE;
		}
		else
		{
			v->x = v->x + x * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->y =  v->y + y * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->z *= MOUSE_SCROL_SCALE;
		}
		ft_redraw(param);
	}
	return (0);
}
