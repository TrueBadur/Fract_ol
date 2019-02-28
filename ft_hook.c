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

	printf("test\n");
	mngr = ((t_manager**)param)[1];
	img = mngr->imgs[mngr->cur_img];
	mlx = *(((t_mlx**)param)[2]);
	//TODO repalce with lib ft_printf
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

int get_win(int x, int y)
{
	if (x > 0 && x < RES && y > 0 && y < RES)
		return (MAIN_I);
	if (x > RES && x < RES + RES / FRACTAL_PRV && y > 0)
		return ((int)(y / ((float)RES / FRACTAL_PRV) + 2));
	return (-1);
}

void ft_redraw(void *param, int nimg)
{
	t_mlx mlx;
	t_manager *mngr;
	t_img	*img;
	t_double2 *jc;

	mlx = *(((t_mlx**)param)[2]);
	mngr = ((t_manager**)param)[1];
	img = &mngr->imgs[nimg];
	img->iter = ft_get_iters(img->start, img->iter_mod);
	jc = &mngr->jc;
	ft_ocl_make_img(img, ((t_ocl**)param)[0], jc);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], img->img_ptr,
			img->pos.x, img->pos.y);
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
		frct_close(param);
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
	ft_redraw(param, mngr->cur_img);
	return (0);
}

int mouse_move_handle(int x, int y, void *param)
{
	t_manager *mngr;
	t_double3	*v;
	int			win;
	t_img		*img;

	mngr = ((t_manager**)param)[1];
	img = &mngr->imgs[mngr->cur_img];
	printf("x = %d, y = %d\n", x , y);
	v = &mngr->imgs[mngr->cur_img].start;
	printf("win %d start x = %f, y = %f, z = %f\n",win, v->x , v->y, v->z);
	if (mngr->mouse_mask & (1 << 2) &&
	IN_RANGE_II(img->pos.x, x, img->pos.x + img->res.x) &&
	IN_RANGE_II(img->pos.y, y, img->pos.y + img->res.y))
	{
		mngr->jc = img->kern >= JULIA ? (t_double2){mngr->jc.x + (x - mngr->mse_mv_crd[2].x) * img->start.z / 6,
													mngr->jc.y + (y - mngr->mse_mv_crd[2].y) * img->start.z / 6} : mngr->jc;
		mngr->mse_mv_crd[2] = (t_int2){x, y};
	}
	else if(mngr->mouse_mask & (1 << 0))
	{
		v->x += mngr->mse_mv_crd[0].x != x ?
				(mngr->mse_mv_crd[0].x - x) * v->z : 0;
		v->y -=	(mngr->mse_mv_crd[0].y != y) ?
				   -(mngr->mse_mv_crd[0].y - y) * v->z : 0;
		mngr->mse_mv_crd[0] = (t_int2){x, y};
	}
	else if(mngr->mouse_mask & (1 << 1) && mngr->cur_img == MAIN_I)
	{
		if (IS_SHIFT_DOWN)
			mngr->imgs[mngr->cur_img].col.z += mngr->mse_mv_crd[1].x != x ?
											   (float)(mngr->mse_mv_crd[1].x - x) / RES : 0;
		else
		{
			mngr->imgs[mngr->cur_img].col.x += mngr->mse_mv_crd[1].x != x ?
											   (float)(mngr->mse_mv_crd[1].x - x) / RES : 0;
			mngr->imgs[mngr->cur_img].col.y -= mngr->mse_mv_crd[0].y != y ?
					(float)(mngr->mse_mv_crd[1].y - y) / RES : 0;
		}
		mngr->mse_mv_crd[1] = (t_int2){x, y};
	}
	else
		return (0);
	ft_redraw(param, mngr->cur_img);
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

void swap_img(t_img *small, t_img *main)
{
	t_double3	start_tmp;
	int 		tmp;

	printf("smal n = %d, main n = %d\n", small->kern, main->kern);
	small->col = (t_float3){0, 0, 0};
	start_tmp = small->start;
	small->start = main->start;
	main->start = start_tmp;
	small->start.z *= FRACTAL_PRV;
	main->start.z /= FRACTAL_PRV;
	tmp = small->kern;
	small->kern = main->kern;
	main->kern = tmp;
	tmp = small->iter;
	small->iter = main->iter;
	main->iter = tmp;
	tmp = small->iter_mod;
	small->iter_mod = main->iter_mod;
	main->iter_mod = tmp;
	printf("smal n = %d, main n = %d\n", small->kern, main->kern);
}

int		mouse_hook(int but, int x, int y, void *param)
{
	t_double3 *v;
	t_manager *mngr;
	t_img		*img;
	int tmp;

	mngr = ((t_manager**)param)[1];
	tmp = get_win(x, y);
	mngr->cur_img = tmp < 0 ? 0 : tmp;
	img = &mngr->imgs[mngr->cur_img];
	v = &img->start;
	if (y > 0 && (but == 3 || but == 2 || but == 1))
	{
		mngr->mouse_mask |= 1 << (but - 1);
		mngr->mse_mv_crd[but - 1] = (t_int2){x, y};
		if (but == 2 && mngr->cur_img != MAIN_I)
		{
			swap_img(img, &mngr->imgs[MAIN_I]);
			ft_redraw(param, MAIN_I);
			ft_redraw(param, mngr->cur_img);
		}
	}
	if (but == 4 || but == 5)
	{
		if (but == 4)
		{
			v->x = v->x + (x - img->pos.x) * (v->z - v->z / MOUSE_SCROL_SCALE);
			v->y = v->y + (y - img->pos.y) * (v->z - v->z / MOUSE_SCROL_SCALE);
			v->z /= MOUSE_SCROL_SCALE;
		}
		else
		{
			v->x = v->x + (x - img->pos.x) * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->y =  v->y + (y - img->pos.y) * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->z *= MOUSE_SCROL_SCALE;
		}
		ft_redraw(param, mngr->cur_img);
	}
	return (0);
}
