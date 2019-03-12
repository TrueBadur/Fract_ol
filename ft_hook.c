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
	t_int4	i_pos;
	int		i;
	t_img	img;

	i = MAIN_I - 1;
	while (++i <= mngr->img_num)
	{
		img = mngr->imgs[i];
		i_pos = (t_int4){img.pos.x, img.pos.y, img.pos.x + img.res.x,
				   img.pos.y + img.res.y};
		if (IN_RNGII(i_pos.x, x, i_pos.z) && IN_RNGII(i_pos.y, y, i_pos.w))
			return (i);
	}
	return (MAIN_I);
}

int ft_redraw(void *param, int nimg)
{
	t_mlx mlx;
	t_manager *mngr;
	t_img	*img;

	mngr = (t_manager*)param;
	mlx = mngr->mlx;
	img = &mngr->imgs[nimg];
	if (img->opts.kern < 0)
		return (1);
	img->opts.iter = ft_get_iters(img->opts.strt, img->opts.iter_mod);
	ft_ocl_make_img(img, &mngr->ocl, &img->opts.jc);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], img->img_ptr,
			img->pos.x, img->pos.y);
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
			col->y -= mngr->msmvcd[1].y != y ? (float)(mngr->msmvcd[1].y - y)
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
		IN_RNGII(img->pos.x, x, img->pos.x + img->res.x) &&
		IN_RNGII(img->pos.y, y, img->pos.y + img->res.y))
	{
		img->opts.jc = img->opts.kern >= JULIA ? (t_double2){img->opts.jc.x +
		   (x - mngr->msmvcd[2].x) * img->opts.strt.z / 6, img->opts.jc.y +
		   (y - mngr->msmvcd[2].y) * img->opts.strt.z / 6} : img->opts.jc;
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

void swap_img(t_img *small, t_img *main, int swp_col)
{
	t_frctl_o	o_tmp;

	o_tmp = small->opts;
	small->opts = main->opts;
	main->opts = o_tmp;
	if (!swp_col)
	{
		main->opts.col = small->opts.col;
		small->opts.col = (t_float3){0, 0, 0};
	}
	small->opts.strt.z *= FRCTL_PRV;
	main->opts.strt.z /= small->num >= COL_PR ? SAVE_NUM + COL_PR_NUM
			: FRCTL_PRV;
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


void	set_img(t_manager *mngr, t_int2 start, int mode, t_img *donor)
{
	while (start.x < start.y)
	{
		if (mode == ALL)
		{
			mngr->imgs[start.x].opts = donor->opts;
		}
		else if (mode == KERN)
			mngr->imgs[start.x].opts.kern = donor->opts.kern;
		else if (mode == COL)
			mngr->imgs[start.x].opts.col = donor->opts.col;
		ft_redraw(mngr, start.x);
		start.x++;
	}
}

void	save_redraw(t_manager *mngr, int save)
{
	int i;
	int j;
	t_img *img;

	if (save)
		ft_vecpush(mngr->saves, &mngr->imgs[MAIN_I].opts, sizeof(t_frctl_o));
	i = mngr->saves->len / sizeof(t_frctl_o);
	((t_frctl_o*)mngr->saves->data)[i - 1].strt.z *= save ? SAVE_NUM +
			COL_PR_NUM : 1;
	j = -1;
	while (i-- && ++j < SAVE_NUM)
	{
		mngr->imgs[j + SAVE_PR].opts = ((t_frctl_o*)mngr->saves->data)[i];
		ft_redraw(mngr, j + SAVE_PR);
	}
	while (++j < SAVE_NUM)
	{
		img = &mngr->imgs[j + SAVE_PR];
		ft_bzero(img->data, img->res.x * img->res.y * 4);
		draw_empty_save(mngr, img, 0, 0);
	}
}

int		load_img_pr(t_manager *mngr)
{
	t_img	*main;
	t_img	*load;
	int		i;

	main = &mngr->imgs[MAIN_I];
	load = &mngr->imgs[mngr->cur_img];
	if (mngr->saves->len == 0)
		return (1);
	if (main->opts.kern == load->opts.kern)
	{
		set_img(mngr, (t_int2){MAIN_I, MAIN_I + 1}, ALL, load);
		main->opts.strt.z /= SAVE_NUM + COL_PR_NUM;
	}
	else
	{
		i = MAIN_I;
		while (++i < COL_PR)
			if (mngr->imgs[i].opts.kern == load->opts.kern)
			{
				swap_img(&mngr->imgs[i], &mngr->imgs[MAIN_I], 0);
				set_img(mngr, (t_int2){MAIN_I, MAIN_I + 1}, ALL, load);
				main->opts.strt.z /= SAVE_NUM + COL_PR_NUM;
				set_img(mngr, (t_int2){COL_PR, SAVE_PR}, KERN, &mngr->imgs[MAIN_I]);
				ft_redraw(mngr, i);
			}
	}
	return (0);
}

int		rmb_handle(t_manager *mngr, int x, int y)
{
	t_img	*img;

	img = &mngr->imgs[mngr->cur_img];
	printf("rmb_handle %d\n", mngr->cur_img);
	if (mngr->cur_img != MAIN_I)
	{
		if (mngr->cur_img < COL_PR)
		{
			swap_img(img, &mngr->imgs[MAIN_I], 0);
			set_img(mngr, (t_int2){COL_PR, SAVE_PR}, KERN, &mngr->imgs[MAIN_I]);
		}
		else if (mngr->cur_img < SAVE_PR)
			set_img(mngr, (t_int2){MAIN_I, MAIN_I + 1}, COL, img);
		else if (mngr->cur_img < SAVE_PR_END && IS_CMND_D)
		{
			ft_vecremove(mngr->saves, mngr->saves->len - (mngr->cur_img -
			SAVE_PR + 1) * sizeof(t_frctl_o), sizeof(t_frctl_o));
			img->opts.kern = -1;
			save_redraw(mngr, 0);
		}
		else if (load_img_pr(mngr))
			return (1);
		ft_redraw(mngr, MAIN_I);
	}
	else if (mngr->cur_img == MAIN_I && IS_CNTRL_D)
		save_redraw(mngr, 1);
	return (0);
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
		if (but == 2 && rmb_handle(mngr, x, y))
			return (0);
	}
	if (but == 4 || but == 5)
	{
		scroll(img, but, x, y);
	}
	return (ft_redraw(param, mngr->cur_img));
}
