/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   saves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 16:17:43 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/17 16:17:43 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int		hook_keydwn_save(int key, void *param)
{
	t_mlx	*mlx;
	t_mngr	*mngr;

	mlx = &((t_mngr *)param)->mlx;
	mngr = (t_mngr*)param;
	if (key == 53)
	{
		mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr[SAVE_W]);
		mlx->win_ptr[SAVE_W] = NULL;
	}
	if (key == 125 || key == 126)
		draw_saves(mngr, key == 125 ? 4 : -4);
	mngr->key_mask |= (SHIFT * SHIFT_D) | (CNTRL * CNTRL_D) | (CMND * CMND_D);
	return (0);
}

int		save_close(void *param)
{
	t_mlx	*mlx;

	mlx = &((t_mngr*)param)->mlx;
	mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr[SAVE_W]);
	mlx->win_ptr[SAVE_W] = NULL;
	return (0);
}

int		mouse_hook_save(int key, int x, int y, void *param)
{
	int		imgn;
	t_mngr	*mngr;
	t_img	*img;

	mngr = (t_mngr*)param;
	if ((imgn = y / 200 * 4 + x / 200 + SAVE_PR_IN_W) < 0 || imgn > END_I ||
	imgn >= mngr->img_num)
		return (0);
	img = &mngr->imgs[imgn];
	if (key == 2 && IS_CMND_D)
	{
		ft_vecremove(mngr->saves, mngr->saves->len - (imgn + mngr->sv_off -
		SAVE_PR_IN_W + 1) * sizeof(t_frctl_o), sizeof(t_frctl_o));
		img->opts.kern = -1;
		save_redraw(mngr, 0);
		ft_bzero(img->data, img->res.x * img->res.y * 4);
		draw_saves(mngr, 0);
	}
	else if (key == 2)
		load_img_pr(mngr, imgn);
	return (0);
}

int		load_img_pr(t_mngr *mngr, int imgtl)
{
	t_img	*load;
	int		i;

	load = &mngr->imgs[imgtl];
	i = MAIN_I;
	if (mngr->saves->len == 0)
		return (1);
	if (mngr->imgs[MAIN_I].opts.kern != load->opts.kern)
		while (++i < COL_PR)
			if (mngr->imgs[i].opts.kern == load->opts.kern)
			{
				swap_img(&mngr->imgs[i], &mngr->imgs[MAIN_I], 0);
				break ;
			}
	set_img(mngr, (t_int2){MAIN_I, MAIN_I}, ALL, load);
	set_img(mngr, (t_int2){COL_PR, SAVE_PR - 1}, KERN, &mngr->imgs[MAIN_I]);
	mngr->imgs[MAIN_I].opts.strt.z /= imgtl >= SAVE_PR_IN_W ? 6 :
			SAVE_NUM + COL_PR_NUM;
	ft_redraw(mngr, i);
	ft_redraw(mngr, MAIN_I);
	return (0);
}

void	open_saves(t_mngr *mngr)
{
	t_mlx		*mlx;

	mngr->sv_off = 0;
	mlx = &mngr->mlx;
	mlx->win_ptr[SAVE_W] = mlx_new_window(mlx->mlx_ptr, 800, 800,
			"Fract_ol: saves");
	draw_saves(mngr, 0);
	mlx_hook(mlx->win_ptr[SAVE_W], 2, 5, hook_keydwn_save, mngr);
	mlx_hook(mlx->win_ptr[SAVE_W], 17, (1L << 3), save_close, (void*)mngr);
	mlx_hook(mlx->win_ptr[SAVE_W], 4, 0, mouse_hook_save, mngr);
	mlx_hook(mlx->win_ptr[SAVE_W], 3, 5, hook_keyrelease, (void*)mngr);
}
