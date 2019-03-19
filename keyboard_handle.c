/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_handle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 19:50:22 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/19 19:50:22 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
		restart(mngr);
	if (HELP)
		help(mngr);
	if (SAVE)
		open_saves(mngr);
	return (ft_redraw(param, mngr->cur_img));
}
