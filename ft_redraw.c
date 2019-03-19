/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redraw.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 19:44:26 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/19 19:44:26 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int		ft_redraw(void *param, int nimg)
{
	t_mlx		mlx;
	t_mngr		*mngr;
	t_img		*img;
	int			win;

	mngr = (t_mngr*)param;
	mlx = mngr->mlx;
	img = &mngr->imgs[nimg];
	win = nimg >= SAVE_PR_IN_W ? SAVE_W : MAIN_W;
	if (img->opts.kern < 0)
		return (1);
	img->opts.iter = ft_get_iters(img->opts.strt, img->opts.iter_mod);
	ft_ocl_make_img(img, &mngr->ocl, &img->opts.jc);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[win], img->img_ptr,
							img->pos.x, img->pos.y);
	return (0);
}

void	save_redraw(t_mngr *mngr, int save)
{
	int		i;
	int		j;
	t_img	*img;

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
	if (mngr->mlx.win_ptr[SAVE_W])
		draw_saves(mngr, 0);
	write_savefile(mngr->saves, 0);
}
