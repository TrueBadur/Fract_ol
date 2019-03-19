/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 19:45:14 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/19 19:45:14 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

size_t	ft_get_iters(t_double3 start, int iter_mod)
{
	size_t	ret;
	int		x;

	x = (int)(1 / start.z);
	ret = (size_t)log((x - 250) / 100.0 + 2) * 200;
	ret = (int)ret + iter_mod > 0 ? ret + iter_mod : 30;
	ret = ret > 8000 ? 8000 : ret;
	ret = ret < 30 || start.z > 0.01 ? 30 : ret;
	return (ret);
}

int		get_win(t_mngr *mngr, int x, int y)
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

void	restart(t_mngr *mngr)
{
	char kern;

	kern = mngr->imgs[MAIN_I].opts.kern;
	mngr->imgs[MAIN_I].opts.strt = g_starts[kern > JULIA ? JULIA : kern];
	mngr->imgs[MAIN_I].opts.strt.z /= mngr->res;
	ft_redraw(mngr, MAIN_I);
}

void	swap_img(t_img *small, t_img *main, int swp_col)
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

void	set_img(t_mngr *mngr, t_int2 start, int mode, t_img *donor)
{
	t_float3 tmp;

	while (start.x <= start.y)
	{
		if (mode == ALL)
			mngr->imgs[start.x].opts = donor->opts;
		else if (mode == KERN)
			mngr->imgs[start.x].opts.kern = donor->opts.kern;
		else if (mode == COL)
			mngr->imgs[start.x].opts.col = donor->opts.col;
		else if (mode == NOCOL)
		{
			tmp = mngr->imgs[start.x].opts.col;
			mngr->imgs[start.x].opts = donor->opts;
			mngr->imgs[start.x].opts.col = tmp;
			mngr->imgs[start.x].opts.strt.z *= SAVE_NUM + COL_PR_NUM;
		}
		ft_redraw(mngr, start.x);
		start.x++;
	}
}
