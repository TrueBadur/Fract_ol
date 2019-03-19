/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_saves.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 19:09:49 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/19 19:09:49 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	draw_saves(t_mngr *mngr, int d_off)
{
	int			i;
	unsigned	j;
	t_img		*simg;

	i = mngr->saves->len / sizeof(t_frctl_o);
	mngr->sv_off += mngr->sv_off + d_off >= 0 && i - mngr->sv_off - d_off >= 12
			? d_off : 0;
	mngr->sv_off = i < 16 ? 0 : mngr->sv_off;
	i -= mngr->sv_off;
	j = 0;
	mlx_clear_window(mngr->mlx.mlx_ptr, mngr->mlx.win_ptr[SAVE_W]);
	while (i-- && j < MAX_SAVEW - 1)
	{
		j = mngr->saves->len / sizeof(t_frctl_o) - mngr->sv_off - i - 1;
		simg = &mngr->imgs[SAVE_PR_IN_W + j];
		simg->res = (t_uint2){200, 200};
		simg->pos = (t_int2){j % 4 * 200, j / 4 * 200};
		simg->opts = ((t_frctl_o*)mngr->saves->data)[i];
		simg->opts.strt.z = simg->opts.strt.z / (SAVE_NUM + COL_PR_NUM) * 6;
		if (!simg->buf)
			init_img(mngr, SAVE_PR_IN_W + j, simg, SAVE_W);
		ft_redraw(mngr, SAVE_PR_IN_W + j);
	}
}
