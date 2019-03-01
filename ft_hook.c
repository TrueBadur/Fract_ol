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

	mngr = (t_manager*)param;
	img = mngr->imgs[mngr->cur_img];
	mlx = mngr->mlx;
	//TODO repalce with lib ft_printf
	sprintf(t[0], "Number of iterations: %zu, %d", img.opts.iter,
			img.opts.iter_mod);
	sprintf(t[1], "Base color: (%f, %f, %f)", img.opts.col.x, img.opts.col.y,
			img.opts.col.z);
	sprintf(t[2], "Scale: 1:%.0f", (float)(1 / img.opts.strt.z));
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw],
			mngr->imgs[INFO_I].img_ptr, 0, 0);
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

int get_win(t_manager *mngr, int x, int y)
{
	t_int4 m_pos;

	m_pos = (t_int4){mngr->imgs[MAIN_I].pos.x, mngr->imgs[MAIN_I].pos.y,
	mngr->imgs[MAIN_I].pos.x + mngr->res, mngr->imgs[MAIN_I].pos.y + mngr->res};
	if (x > m_pos.x && x < m_pos.z && y > m_pos.y && y < m_pos.w)
		return (MAIN_I);
	if (x > m_pos.z && x < m_pos.z + mngr->res / FRCTL_PRV && y > 0)
		return ((int)(y / ((float)mngr->res / FRCTL_PRV) + 2));
	return (1);
}

int ft_redraw(void *param, int nimg)
{
	t_mlx mlx;
	t_manager *mngr;
	t_img	*img;

	mngr = (t_manager*)param;
	mlx = mngr->mlx;
	img = &mngr->imgs[nimg];
	img->opts.iter = ft_get_iters(img->opts.strt, img->opts.iter_mod);
	ft_ocl_make_img(img, &mngr->ocl, &mngr->jc);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], img->img_ptr,
			img->pos.x, img->pos.y);
	if (mngr->info)
		put_info(param);
	return (0);
}

int		hook_keyrelease(int key, void *param)
{
	t_manager *mngr;

	mngr = (t_manager*)param;
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
	t_manager	*mngr;
	t_img		*img;

	mngr = (t_manager*)param;
	img = &mngr->imgs[mngr->cur_img];
	mngr->key_mask |= (SHIFT * SHIFT_D) | (CNTRL * CNTRL_D) | (CMND * CMND_D);
	if (key == 53)
		frct_close(param);
	if (ITER_P || ITER_M)
	{
		if (!IS_CNTRL_D && !IS_SHIFT_D)
			img->opts.iter_mod += ITER_P ? 1 : -1;
		img->opts.iter_mod += IS_CNTRL_D ? (ITER_P) * 100 + (ITER_M) * -100 : 0;
		img->opts.iter_mod += IS_SHIFT_D ? (ITER_P) * 10 + (ITER_M) * -10 : 0;
	}
	if (INFO && (mngr->info = ~(mngr->info)))
		mngr->imgs[INFO_I].img_ptr = mlx_new_image(&mngr->mlx.mlx_ptr, mngr->res,
												   INFO_H);
	else if (INFO && !(mngr->info = ~(mngr->info)))
		mlx_destroy_image(&mngr->mlx.mlx_ptr, mngr->imgs[INFO_I].img_ptr);
	if (RESTART)
		//TODO
	return (ft_redraw(param, mngr->cur_img));
}

void	lmb_hold_hndl(t_manager *mngr, x, y)
{
	t_double3 *v;

	v = &mngr->imgs[mngr->cur_img].opts.strt;
	v->x += mngr->msmvcd[0].x != x ? (mngr->msmvcd[0].x - x) * v->z : 0;
	v->y -=	(mngr->msmvcd[0].y != y) ? -(mngr->msmvcd[0].y - y) * v->z : 0;
	mngr->msmvcd[0] = (t_int2){x, y};
}

void	rmb_hold_hndl(t_manager *mngr, x, y)
{
	t_float3	*col;

	col = &mngr->imgs[mngr->cur_img].opts.col;
	if (mngr->cur_img == MAIN_I)
	{
		if (IS_SHIFT_D)
			col->z += mngr->msmvcd[1].x != x ? (float)(mngr->msmvcd[1].x - x)
					/ mngr->res : 0;
		else
		{
			col->x += mngr->msmvcd[1].x != x ? (float)(mngr->msmvcd[1].x - x)
					/ mngr->res : 0;
			col->y -= mngr->msmvcd[0].y != y ? (float)(mngr->msmvcd[1].y - y)
					/ mngr->res : 0;
		}
		mngr->msmvcd[1] = (t_int2){x, y};
	}
}

int mouse_move_handle(int x, int y, void *param)
{
	t_manager *mngr;
	t_img		*img;

	mngr = (t_manager*)param;
	img = &mngr->imgs[mngr->cur_img];
	if (mngr->mouse_mask & (1 << 2) &&
		IN_RANGE_II(img->pos.x, x, img->pos.x + img->res.x) &&
		IN_RANGE_II(img->pos.y, y, img->pos.y + img->res.y))
	{
		mngr->jc = img->opts.kern >= JULIA ? (t_double2){mngr->jc.x +
		   (x - mngr->msmvcd[2].x) * img->opts.strt.z / 6, mngr->jc.y +
		   (y - mngr->msmvcd[2].y) * img->opts.strt.z / 6} : mngr->jc;
		mngr->msmvcd[2] = (t_int2){x, y};
	}
	else if (LMB_HOLD)
		lmb_hold_hndl(mngr, x, y);
	else if (RMB_HOLD)
		rmb_hold_hndl(mngr, x, y);
	else
		return (0);
	return (ft_redraw(param, mngr->cur_img));
}

int		mouse_release(int but, int x, int y, void *param)
{
	t_manager *mngr;

	mngr = (t_manager*)param;
	if (but == 3 || but == 2 || but == 1)
		mngr->mouse_mask &= 0 << but;
	return (0);
}

void swap_img(t_img *small, t_img *main)
{
	t_double3	start_tmp;
	int 		tmp;

	small->opts.col = (t_float3){0, 0, 0};
	start_tmp = small->opts.strt;
	small->opts.strt = main->opts.strt;
	main->opts.strt = start_tmp;
	small->opts.strt.z *= FRCTL_PRV;
	main->opts.strt.z /= FRCTL_PRV;
	tmp = small->opts.kern;
	small->opts.kern = main->opts.kern;
	main->opts.kern = (char)tmp;
	tmp = (int)small->opts.iter;
	small->opts.iter = main->opts.iter;
	main->opts.iter = (size_t)tmp;
	tmp = small->opts.iter_mod;
	small->opts.iter_mod = main->opts.iter_mod;
	main->opts.iter_mod = tmp;
}

void	scroll(t_img *img, int but, int x, int y)
{
	t_double3 *v;

	v = &img->opts.strt;
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
}

int		mouse_hook(int but, int x, int y, void *param)
{
	t_manager *mngr;
	t_img		*img;

	mngr = (t_manager*)param;
	mngr->cur_img = get_win(mngr, x, y);
	img = &mngr->imgs[mngr->cur_img];
	if (y > 0 && (but == 3 || but == 2 || but == 1))
	{
		mngr->mouse_mask |= 1 << (but - 1);
		mngr->msmvcd[but - 1] = (t_int2){x, y};
		if (but == 2 && mngr->cur_img != MAIN_I)
		{
			swap_img(img, &mngr->imgs[MAIN_I]);
			ft_redraw(param, MAIN_I);
		}
	}
	if (but == 4 || but == 5)
	{
		scroll(img, but, x, y);
	}
	return (ft_redraw(param, mngr->cur_img));
}
