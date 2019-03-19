/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 03:41:03 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/01 03:41:03 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_double3 g_starts[] = {(t_double3){-2.15, -1.5, 3.0},
						(t_double3){-2.35, -2.55, 4.0},
						(t_double3){-17.48, -17.6, 35.0},
						(t_double3){-17.48, -17.6, 35.0},
						(t_double3){-1.76, -1.56, 3.4}};
t_float3 g_cols[] = {(t_float3){0.0, 0.0, 0.0},
					(t_float3){0.0, 0.6, 1.0},
					(t_float3){0.0, (float)-1.5, 2.5},
					(t_float3){2.0, 0.8, -0.8},
					(t_float3){0.21, 2.8, -2.35},
					(t_float3){-2.0, -0.06, 2.7}};

void	init(t_mngr *mngr)
{
	unsigned	res;
	t_mlx		*mlx;

	res = mngr->res;
	mlx = &mngr->mlx;
	mngr->mlx.mlx_ptr = mlx_init();
	ft_bzero(mngr->mlx.win_ptr, sizeof(void*) * 10);
	ft_bzero(mngr->imgs, sizeof(t_img) * END_I);
	mngr->mlx.wc = 1;
	mngr->mlx.cw = 0;
	mngr->mlx.res[MAIN_W] = (t_int2){res + L_COL_W(res) + R_COL_W(res), res};
	mngr->img_num = 2;
	mngr->cur_img = MAIN_I;
	mngr->mouse_mask = 0;
	mngr->key_mask = 0;
	mngr->info = 0;
	mngr->mlx.win_ptr[MAIN_W] = mlx_new_window(mlx->mlx_ptr,
			mlx->res[mlx->cw].x, mlx->res[mlx->cw].y, "Fract_ol: main");
	ft_ocl_dev_cont_prog(&mngr->ocl, PROGRAM_FILE);
	ft_ocl_set_env(&mngr->ocl);
	get_saves(mngr, SAVE_FILE);
}

void	init_img(t_mngr *mngr, int nimg, t_img *img, int win)
{
	cl_int	err;
	size_t	gs;

	if (!img)
		img = &mngr->imgs[nimg];
	img->num = nimg >= 0 ? nimg : img->num;
	img->img_ptr = mlx_new_image(mngr->mlx.mlx_ptr, img->res.x, img->res.y);
	img->data = mlx_get_data_addr(img->img_ptr, &img->bpp, &img->size_line,
								&img->endian);
	if (nimg > MAIN_I && nimg < SAVE_PR)
		img->opts.strt.z *= nimg >= COL_PR ? SAVE_NUM + COL_PR_NUM : FRCTL_PRV;
	gs = (size_t)img->res.x * img->res.y;
	img->buf = clCreateBuffer(mngr->ocl.context, CL_MEM_READ_WRITE,
							sizeof(int) * gs, NULL, &err);
	if (err < 0)
		ft_ocl_err_handler(FT_OCL_BUFFER_ERR);
	mngr->img_num = nimg > 0 ? nimg + 1 : mngr->img_num;
	if (img->opts.kern < 0)
		return ;
	ft_ocl_make_img(img, &mngr->ocl, &img->opts.jc);
	mlx_put_image_to_window(mngr->mlx.mlx_ptr, mngr->mlx.win_ptr[win],
							img->img_ptr, img->pos.x, img->pos.y);
}

void	init_r_col(t_mngr *mngr, char s_kern)
{
	unsigned	i;
	t_uint2		pr_res;
	char		kern;

	pr_res = (t_uint2){R_COL_W(mngr->res), R_COL_W(mngr->res)};
	i = PR_S;
	while (i < COL_PR)
	{
		kern = (char)(((i - PR_S + s_kern) + 1) % FRCTL_NUM);
		mngr->imgs[i].opts.iter = 101;
		mngr->imgs[i].opts.kern = kern;
		mngr->imgs[i].pos = (t_int2){mngr->res + L_COL_W(mngr->res),
									(i - PR_S) * pr_res.x};
		mngr->imgs[i].opts.strt = g_starts[mngr->imgs[i].opts.kern > JULIA ?
										JULIA : mngr->imgs[i].opts.kern];
		mngr->imgs[i].opts.strt.z /= mngr->res;
		mngr->imgs[i].res = pr_res;
		mngr->imgs[i].opts.iter_mod = 0;
		mngr->imgs[i].opts.col = g_cols[0];
		init_img(mngr, i++, NULL, MAIN_W);
	}
}

void	init_l_col(t_mngr *mngr)
{
	unsigned	i;
	t_uint2		pr_res;
	char		kern;

	pr_res = (t_uint2){L_COL_W(mngr->res), L_COL_W(mngr->res)};
	i = COL_PR;
	while (i < SAVE_PR_IN_W)
	{
		mngr->imgs[i].res = pr_res;
		mngr->imgs[i].pos = (t_int2){0, (i - COL_PR) * pr_res.x};
		if (i >= SAVE_PR)
		{
			init_save(mngr, i++);
			continue ;
		}
		kern = mngr->imgs[MAIN_I].opts.kern;
		mngr->imgs[i].opts.iter = 101;
		mngr->imgs[i].opts.kern = kern;
		mngr->imgs[i].opts.strt = g_starts[mngr->imgs[i].opts.kern > JULIA ?
										JULIA : mngr->imgs[i].opts.kern];
		mngr->imgs[i].opts.strt.z /= mngr->res;
		mngr->imgs[i].opts.iter_mod = 0;
		mngr->imgs[i].opts.col = g_cols[i - COL_PR + 1];
		init_img(mngr, i++, NULL, MAIN_W);
	}
}

void	init_main(t_mngr *mngr, char kern)
{
	mngr->imgs[MAIN_I].opts.iter = 101;
	mngr->imgs[MAIN_I].opts.kern = kern;
	mngr->imgs[MAIN_I].pos = (t_int2){L_COL_W(mngr->res), 0};
	mngr->imgs[MAIN_I].res = (t_uint2){mngr->res, mngr->res};
	mngr->imgs[MAIN_I].opts.strt = g_starts[kern > JULIA ? JULIA : kern];
	mngr->imgs[MAIN_I].opts.strt.z /= mngr->res;
	mngr->imgs[MAIN_I].opts.iter_mod = 0;
	mngr->imgs[MAIN_I].opts.col = g_cols[1];
	init_img(mngr, MAIN_I, NULL, MAIN_W);
}
