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
					 (t_float3){1.0, 0.3, 0.0},
					 (t_float3){0.45, 1.17, -0.78},
					 (t_float3){1.0, 1.0, 1.0}};

void ft_ocl_set_env(t_ocl *ocl)
{
	cl_int	err;
	int		i;

	ocl->kern = malloc(sizeof(cl_kernel) * FRCTL_NUM);
	i = -1;
	while(g_kernels[++i])
	{
		ocl->kern[i] = clCreateKernel(ocl->program, g_kernels[i], &err);
		if (err < 0)
			ft_ocl_err_handler(FT_OCL_KERNEL_ERR);
	}
}

void	init(t_manager *mngr)
{
	unsigned	res;

	res = mngr->res;
	mngr->mlx.mlx_ptr = mlx_init();
	mngr->mlx.wc = 0;
	mngr->mlx.cw = 0;
	//mngr->mlx.res[mngr->mlx.cw] = (t_int2){res + res / FRCTL_PRV +
	//									   res / SAVE_NUM + res / COL_PR_NUM, res};
	mngr->mlx.res[mngr->mlx.cw] = (t_int2){res + res / FRCTL_PRV +
										   res / COL_PR_NUM, res};
	mngr->img_num = 2;
	mngr->cur_img = MAIN_I;
	mngr->mouse_mask = 0;
	mngr->key_mask = 0;
	mngr->info = 0;
	ft_mlx_create_new_window(&mngr->mlx, mngr->mlx.res[mngr->mlx.cw], "Fract_ol: main");
	ft_ocl_dev_cont_prog(&mngr->ocl, PROGRAM_FILE);
	ft_ocl_set_env(&mngr->ocl);
}

void	init_f_i(t_manager *mngr, int nimg)
{
	cl_int	err;
	size_t	gs;
	t_img	*img;

	img = &mngr->imgs[nimg];
	img->img_ptr = mlx_new_image(mngr->mlx.mlx_ptr, img->res.x, img->res.y);
	img->data = mlx_get_data_addr(img->img_ptr, &img->bpp, &img->size_line,
			&img->endian);
	if (nimg > MAIN_I)
		img->opts.strt.z *= nimg >= COL_PR ? COL_PR_NUM : FRCTL_PRV;
	gs = (size_t)img->res.x * img->res.y;
	img->buf = clCreateBuffer(mngr->ocl.context, CL_MEM_READ_WRITE,
							  sizeof(int) * gs, NULL, &err);
	if (err < 0)
		ft_ocl_err_handler(FT_OCL_BUFFER_ERR);
	ft_ocl_make_img(img, &mngr->ocl, &(t_double2){0,0});
	mlx_put_image_to_window(mngr->mlx.mlx_ptr, mngr->mlx.win_ptr[mngr->mlx.cw],
							img->img_ptr, img->pos.x, img->pos.y);
}

void	init_previews(t_manager *mngr, char s_kern)
{
	unsigned	i;
	t_uint2		pr_res;
	char 		kern;

	pr_res = (t_uint2){mngr->res / FRCTL_PRV, mngr->res / FRCTL_PRV};
	i = PR_S;
	while (i < COL_PR)
	{
		kern = (char)((i - PR_S + s_kern) + 1 % FRCTL_NUM);
		mngr->imgs[i].opts.iter = 101;
		mngr->imgs[i].opts.kern = kern;
		mngr->imgs[i].pos = (t_uint2){mngr->res + mngr->res / COL_PR_NUM,
								(i - PR_S) * pr_res.x};
		mngr->imgs[i].opts.strt = g_starts[mngr->imgs[i].opts.kern > JULIA ?
									 JULIA : mngr->imgs[i].opts.kern];
		mngr->imgs[i].opts.strt.z /= mngr->res;
		mngr->imgs[i].res = pr_res;
		mngr->imgs[i].opts.iter_mod = 0;
		mngr->imgs[i].opts.col = g_cols[0];
		init_f_i(mngr, i++);
	}
}

void	init_col_previews(t_manager *mngr, char s_kern)
{
	unsigned	i;
	t_uint2		pr_res;
	char 		kern;

	pr_res = (t_uint2){mngr->res / COL_PR_NUM, mngr->res / COL_PR_NUM};
	i = COL_PR;
	while (i < COL_PR_END)
	{
		kern = mngr->imgs[MAIN_I].opts.kern;
		mngr->imgs[i].opts.iter = 101;
		mngr->imgs[i].opts.kern = kern;
		mngr->imgs[i].pos = (t_uint2){0, (i - COL_PR) * pr_res.x};
		mngr->imgs[i].opts.strt = g_starts[mngr->imgs[i].opts.kern > JULIA ?
										   JULIA : mngr->imgs[i].opts.kern];
		mngr->imgs[i].opts.strt.z /= mngr->res;
		mngr->imgs[i].res = pr_res;
		mngr->imgs[i].opts.iter_mod = 0;
		mngr->imgs[i].opts.col = g_cols[i - COL_PR + 2];
		init_f_i(mngr, i++);
	}
}

void	init_main(t_manager *mngr, char kern)
{
	mngr->imgs[MAIN_I].opts.iter = 101;
	mngr->imgs[MAIN_I].opts.kern = kern;
	mngr->imgs[MAIN_I].pos = (t_uint2){mngr->res / COL_PR_NUM, 0};
	mngr->imgs[MAIN_I].res = (t_uint2){mngr->res, mngr->res};
	mngr->imgs[MAIN_I].opts.strt = g_starts[kern > JULIA ? JULIA : kern];
	mngr->imgs[MAIN_I].opts.strt.z /= mngr->res;
	mngr->imgs[MAIN_I].opts.iter_mod = 0;
	mngr->imgs[MAIN_I].opts.col = g_cols[1];
	init_f_i(mngr, MAIN_I);
}