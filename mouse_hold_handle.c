/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hold_handle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 19:52:27 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/19 19:52:27 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	lmb_hold_hndl(t_mngr *mngr, int x, int y)
{
	t_double3 *v;

	v = &mngr->imgs[mngr->cur_img].opts.strt;
	v->x += mngr->msmvcd[0].x != x ? (mngr->msmvcd[0].x - x) * v->z : 0;
	v->y -= (mngr->msmvcd[0].y != y) ? -(mngr->msmvcd[0].y - y) * v->z : 0;
	mngr->msmvcd[0] = (t_int2){x, y};
}

void	rmb_hold_hndl(t_mngr *mngr, int x, int y)
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
