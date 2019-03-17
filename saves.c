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

void	draw_saves(t_manager *mngr, int init, int d_off)
{
	int			i;
	unsigned	j;
	t_img		*simg;
	static int	offset = 0;

	i = mngr->saves->len / sizeof(t_frctl_o);
	offset += offset + d_off >= 0 && i - offset - d_off > 12 ? d_off : 0;
	i -= offset;
	j = 0;
	mlx_clear_window(mngr->mlx.mlx_ptr, mngr->mlx.win_ptr[SAVE_W]);
	while (i-- && j < MAX_SAVEW - 1)
	{
		j = mngr->saves->len / sizeof(t_frctl_o) - offset - i - 1;
		simg = &mngr->imgs[SAVE_PR_IN_W + j];
		simg->res = (t_uint2){200, 200};
		simg->pos = (t_uint2){j % 4 * 200, j / 4 * 200};
		simg->opts = ((t_frctl_o*)mngr->saves->data)[i];
		simg->opts.strt.z = simg->opts.strt.z / (SAVE_NUM + COL_PR_NUM) * 6;
		if (!simg->buf)
			init_f_i(mngr, SAVE_PR_IN_W + j, simg, SAVE_W);
		ft_redraw(mngr, SAVE_PR_IN_W + j);
	}
}

int hook_keydwn_save (int key, void *param)
{
	t_mlx *mlx;
	t_manager *mngr;

	mlx = &((t_manager *) param)->mlx;
	mngr = (t_manager*)param;
	if (key == 53)
	{
		mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr[SAVE_W]);
		mlx->win_ptr[HELP_W] = NULL;
	}
	if (key == 125 || key == 126)
		draw_saves(mngr, 0, key == 125 ? 4 : -4);
	mngr->key_mask |= (SHIFT * SHIFT_D) | (CNTRL * CNTRL_D) | (CMND * CMND_D);
}

int save_close(void *param)
{
	t_mlx	*mlx;

	mlx = &((t_manager*)param)->mlx;
	mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr[SAVE_W]);
	mlx->win_ptr[HELP_W] = NULL;
}

int mouse_hook_save(int key, int x, int y, void *param)
{
	int 		imgn;
	t_manager	*mngr;
	t_img		*img;

	mngr = (t_manager*)param;
	if ((imgn = y / 200 * 4 + x / 200 + SAVE_PR_IN_W) < 0 || imgn > END_I || imgn >= mngr->img_num)
		return (0);
	img = &mngr->imgs[imgn];
	if (key == 2 && IS_CMND_D)
	{
		ft_vecremove(mngr->saves, mngr->saves->len - (imgn - SAVE_PR_IN_W + 1) *
		sizeof(t_frctl_o), sizeof(t_frctl_o));
		img->opts.kern = -1;
		save_redraw(mngr, 0);
		ft_bzero(img->data, img->res.x * img->res.y * 4);
		draw_saves(mngr, 0, 0);
	}
	else if (key == 2)
		load_img_pr(mngr, imgn);
}

int		load_img_pr(t_manager *mngr, int imgtl)
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
	mngr->imgs[MAIN_I].opts.strt.z /= imgtl >= SAVE_PR_IN_W ? 6 : SAVE_NUM + COL_PR_NUM;
	ft_redraw(mngr, i);
	ft_redraw(mngr, MAIN_I);
	return (0);
}

void	open_saves(t_manager *mngr)
{
	t_mlx		*mlx;

	mlx = &mngr->mlx;
	mlx->win_ptr[SAVE_W] = mlx_new_window(mlx->mlx_ptr, 800, 800, "Fract_ol: saves");
	draw_saves(mngr, 1, 0);
	mlx_hook(mlx->win_ptr[SAVE_W], 2, 5, hook_keydwn_save, mngr);
	mlx_hook(mlx->win_ptr[SAVE_W], 17, (1L << 3), save_close, (void*)mngr);
	mlx_hook(mlx->win_ptr[SAVE_W], 4, 0, mouse_hook_save, mngr);
	mlx_hook(mlx->win_ptr[SAVE_W], 3, 5, hook_keyrelease, (void*)mngr);
}

