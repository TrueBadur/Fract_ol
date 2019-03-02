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

void	get_saves(t_manager *mngr, const char *fname)
{
	FILE	*fd;
	int		i;
	size_t	rr;
	t_frctl_o *tmp;

	if ((fd = fopen(fname, "r")) > 0)
	{
		rr = sizeof(t_frctl_o);
		mngr->saves = ft_vecinit(rr);
		tmp = malloc(rr);
		i = -1;
		while (1)
		{
			i++;
			fread(tmp, rr, 1, fd);
			if (feof(fd))
				break ;
			ft_vecpush(mngr->saves, tmp, rr);
		}
		fclose(fd);
		if (i == 0)
			mngr->saves = ft_vecinit(4 * sizeof(t_frctl_o));
		free(tmp);
	}
	else
		mngr->saves = ft_vecinit(4 * sizeof(t_frctl_o));
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
	mngr->mlx.res[mngr->mlx.cw] = (t_int2){res + L_COL_W(res) + R_COL_W(res),
										   res};
	mngr->img_num = 2;
	mngr->cur_img = MAIN_I;
	mngr->mouse_mask = 0;
	mngr->key_mask = 0;
	mngr->info = 0;
	ft_mlx_create_new_window(&mngr->mlx, mngr->mlx.res[mngr->mlx.cw], "Fract_ol: main");
	ft_ocl_dev_cont_prog(&mngr->ocl, PROGRAM_FILE);
	ft_ocl_set_env(&mngr->ocl);
	get_saves(mngr, SAVE_FILE);
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
		img->opts.strt.z *= nimg >= COL_PR ? SAVE_NUM + COL_PR_NUM : FRCTL_PRV;
	gs = (size_t)img->res.x * img->res.y;
	img->buf = clCreateBuffer(mngr->ocl.context, CL_MEM_READ_WRITE,
							  sizeof(int) * gs, NULL, &err);
	if (err < 0)
		ft_ocl_err_handler(FT_OCL_BUFFER_ERR);
	mngr->img_num = nimg + 1;
	if (img->opts.kern < 0)
		return ;
	ft_ocl_make_img(img, &mngr->ocl, &(t_double2){0,0});
	mlx_put_image_to_window(mngr->mlx.mlx_ptr, mngr->mlx.win_ptr[mngr->mlx.cw],
							img->img_ptr, img->pos.x, img->pos.y);
}

void	init_r_col(t_manager *mngr, char s_kern)
{
	unsigned	i;
	t_uint2		pr_res;
	char 		kern;

	pr_res = (t_uint2){R_COL_W(mngr->res), R_COL_W(mngr->res)};
	i = PR_S;
	while (i < COL_PR)
	{
		kern = (char)((i - PR_S + s_kern) + 1 % FRCTL_NUM);
		mngr->imgs[i].opts.iter = 101;
		mngr->imgs[i].opts.kern = kern;
		mngr->imgs[i].pos = (t_uint2){mngr->res + L_COL_W(mngr->res),
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

void	draw_empty_save(t_manager *mngr, t_img *img, int i, int new)
{
	t_mlx *mlx;

	mlx = &mngr->mlx;
	img->opts.kern = -1;
	if (new)
		init_f_i(mngr, i);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr[mngr->mlx.cw],
							img->img_ptr, img->pos.x, img->pos.y);
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr[mngr->mlx.cw],
				   img->pos.x + 30, img->pos.y + img->res.y / 2,
				   0x00aaaaaa, "Save slot");
}

void	init_save(t_manager *mngr, int i)
{
	t_img	*img;
	t_vector	*v;
	size_t		s;

	img = &mngr->imgs[i];
	v = mngr->saves;
	s = v->len / sizeof(t_frctl_o);
	if (s < i - SAVE_PR + 1)
		draw_empty_save(mngr, img, i, 1);
	else
	{
		img->opts = ((t_frctl_o*)v->data)[s - 1 - i + SAVE_PR];
		init_f_i(mngr, i);
	}
}

void	init_l_col(t_manager *mngr)
{
	unsigned	i;
	t_uint2		pr_res;
	char 		kern;

	pr_res = (t_uint2){L_COL_W(mngr->res), L_COL_W(mngr->res)};
	i = COL_PR;
	while (i < SAVE_PR_END)
	{
		mngr->imgs[i].res = pr_res;
		mngr->imgs[i].pos = (t_uint2){0, (i - COL_PR) * pr_res.x};
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
		init_f_i(mngr, i++);
	}
}

void	init_main(t_manager *mngr, char kern)
{
	mngr->imgs[MAIN_I].opts.iter = 101;
	mngr->imgs[MAIN_I].opts.kern = kern;
	mngr->imgs[MAIN_I].pos = (t_uint2){L_COL_W(mngr->res), 0};
	mngr->imgs[MAIN_I].res = (t_uint2){mngr->res, mngr->res};
	mngr->imgs[MAIN_I].opts.strt = g_starts[kern > JULIA ? JULIA : kern];
	mngr->imgs[MAIN_I].opts.strt.z /= mngr->res;
	mngr->imgs[MAIN_I].opts.iter_mod = 0;
	mngr->imgs[MAIN_I].opts.col = g_cols[1];
	init_f_i(mngr, MAIN_I);
}