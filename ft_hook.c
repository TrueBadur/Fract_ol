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

int		mouse_move_handle(int x, int y, void *param)
{
	t_manager	*mngr;
	t_img		*img;

	mngr = (t_manager*)param;
	img = &mngr->imgs[mngr->cur_img];
	if (mngr->mouse_mask & (1 << 2) &&
		IN_RNGII(img->pos.x, x, img->pos.x + (int)img->res.x) &&
		IN_RNGII(img->pos.y, y, img->pos.y + (int)img->res.y))
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
	t_manager	*mngr;

	x += 0;
	y += 0;
	mngr = (t_manager*)param;
	if (but == 3 || but == 2 || but == 1)
		mngr->mouse_mask &= 0 << but;
	return (0);
}

void	scroll(t_img *img, int but, int x, int y)
{
	t_double3	*v;

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
		v->y = v->y + (y - img->pos.y) * (v->z - v->z * MOUSE_SCROL_SCALE);
		v->z *= MOUSE_SCROL_SCALE;
	}
}

int		rmb_handle(t_manager *mngr, int x, int y)
{
	if (mngr->cur_img != MAIN_I || (x = y + y))
	{
		if (mngr->cur_img < COL_PR)
		{
			swap_img(&mngr->imgs[mngr->cur_img], &mngr->imgs[MAIN_I], 0);
			set_img(mngr, (t_int2){COL_PR, SAVE_PR - 1}, NOCOL,
					&mngr->imgs[MAIN_I]);
		}
		else if (mngr->cur_img < SAVE_PR)
			set_img(mngr, (t_int2){MAIN_I, MAIN_I}, COL,
					&mngr->imgs[mngr->cur_img]);
		else if (mngr->cur_img < SAVE_PR_IN_W && IS_CMND_D)
		{
			ft_vecremove(mngr->saves, mngr->saves->len - (mngr->cur_img -
			SAVE_PR + 1) * sizeof(t_frctl_o), sizeof(t_frctl_o));
			mngr->imgs[mngr->cur_img].opts.kern = -1;
			save_redraw(mngr, 0);
		}
		else if (load_img_pr(mngr, mngr->cur_img))
			return (1);
		ft_redraw(mngr, MAIN_I);
	}
	else if (mngr->cur_img == MAIN_I && IS_CNTRL_D)
		save_redraw(mngr, 1);
	return (0);
}

int		mouse_hook(int but, int x, int y, void *param)
{
	t_manager	*mngr;
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
