/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bparker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/23 11:17:51 by bparker           #+#    #+#             */
/*   Updated: 2019/01/14 20:33:02 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "fractol.h"

char *g_kernels[] = {"mndlbrt",
					 "brnng_shp",
					 "tantan",
					 "fabs_tan",
					 "julia",
					 "julia_tantan",
					 "julia_fabstan",
					 "julia_fabsfabs",
					 NULL};

void ft_mlx_create_new_window(t_mlx *mlx, t_int2 res, char *name)
{
	mlx->cw += mlx->wc ? 1 : 0;
	mlx->res[mlx->cw] = res;
	mlx->win_ptr[mlx->cw] = mlx_new_window(mlx->mlx_ptr, mlx->res[mlx->cw].x,
										 mlx->res[mlx->cw].y, name);
	mlx->wc++;
}

void ft_ocl_make_img(t_img *img, t_ocl *ocl, t_double2 *jc)
{
	size_t ls;
	cl_int err;
	size_t gs;
	char test[50];

	ls = img->size_line / 4;
	gs = img->res.x * img->res.y;
	clGetKernelInfo(ocl->kernel[img->kern], CL_KERNEL_FUNCTION_NAME, 50, test, NULL);
	printf("(%s) %s\n",g_kernels[img->kern], test);
	printf("start (%f, %f, %f)\n", img->start.x, img->start.y, img->start.z);
	err = clSetKernelArg(ocl->kernel[img->kern], 0, sizeof(cl_mem), &img->buf);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_KERNEL_ARG_ERR);
	err = clSetKernelArg(ocl->kernel[img->kern], 1, sizeof(unsigned int) * 2, &(unsigned int[]){img->iter, ls});
	err |= clSetKernelArg(ocl->kernel[img->kern], 2, sizeof(t_double3), &img->start);
	err |= clSetKernelArg(ocl->kernel[img->kern], 3, sizeof(t_float3), &img->col);
	if (img->kern >= JULIA) {
		printf("start (%f, %f)\n", jc->x, jc->y);
		err |= clSetKernelArg(ocl->kernel[img->kern], 4, sizeof(t_double2), jc);
	}
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_KERNEL_ARG_ERR);
	err = clEnqueueNDRangeKernel(ocl->queue, ocl->kernel[img->kern], 1, NULL, &gs, NULL, 0, NULL, NULL);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_ENQUEUE_KERNEL_ERR);
	err = clEnqueueReadBuffer(ocl->queue, img->buf, CL_TRUE, 0, sizeof(int) * gs, img->data, 0, NULL, NULL);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_READ_BUF_ERR);
}

void ft_ocl_set_env(t_ocl *ocl)
{
	cl_int err;
	int i;

	ocl->kernel = malloc(sizeof(cl_kernel) * FRACTAL_NUMBER);
	i = -1;
	while(g_kernels[++i])
	{
		ocl->kernel[i] = clCreateKernel(ocl->program, g_kernels[i], &err);
		if (err < 0)
			ft_ocl_err_handler(err, FT_OCL_KERNEL_ERR);
	}
}

static void init(t_mlx *mlx, t_manager *mngr, t_ocl *ocl)
{
	mlx->mlx_ptr = mlx_init();
	mlx->wc = 1;
	mlx->cw = 0;
	mngr->img_num = 2;
	mngr->cur_img = MAIN_I;
	mngr->mouse_mask = 0;
	mngr->key_mask = 0;
	mngr->imgs[mngr->cur_img].col = (t_float3){0.0, 0.6, 1.0};
	mngr->imgs[mngr->cur_img].iter_mod = 0;
	mngr->info = 0;
	ft_mlx_create_new_window(mlx, (t_int2){RES + RES / FRACTAL_PRV, RES}, "Fract_ol: main");
	ft_ocl_dev_cont_prog(ocl, PROGRAM_FILE);
	ft_ocl_set_env(ocl);
}

void total_free(void *param)
{
	int i;

	i = -1;
	while(g_kernels[++i])
		clReleaseKernel(((t_ocl**)param)[0]->kernel[i]);
	free(((t_ocl**)param)[0]->kernel);
}

int frct_close(void *param)
{
	total_free(param);
	exit(0);
	return (0);
}

void init_f_i(t_img *img, t_mlx *mlx, t_double3 start, t_uint2 res, size_t krn, t_ocl *ocl, t_uint2 pos)
{
	cl_int err;
	size_t gs;

	img->img_ptr = mlx_new_image(mlx->mlx_ptr, res.x, res.y);
	img->data = mlx_get_data_addr(img->img_ptr, &img->bpp, &img->size_line, &img->endian);
	img->res = res;
	img->start = start;
	img->iter = 101;
	img->pos = pos;
	if (img->res.x != RES)
		img->start.z *= FRACTAL_PRV;
	img->kern = krn;
	gs = (size_t)img->res.x * img->res.y;
	img->buf = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE, sizeof(int) * gs, NULL, &err);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_BUFFER_ERR);
	ft_ocl_make_img(img, ocl, &(t_double2){0,0});
}

int			main(int ac, char **av)
{
	t_manager	mngr;
	t_mlx		mlx;
	//t_img		*img;
	t_ocl		ocl;

	ac = ac + 0;
	av = av + 0;
	init(&mlx, &mngr, &ocl);
	init_f_i(&mngr.imgs[MAIN_I], &mlx, MNDLBRT_START, (t_uint2){RES, RES}, MNDLBRT, &ocl, (t_uint2){0, 0});
	init_f_i(&mngr.imgs[RIGHT1_I], &mlx, FABSTAN_START, (t_uint2){RES / FRACTAL_PRV, RES / FRACTAL_PRV}, FABS_TAN, &ocl, (t_uint2){RES, 0 * RES / FRACTAL_PRV});
	init_f_i(&mngr.imgs[RIGHT2_I], &mlx, BRNNG_SHP_START, (t_uint2){RES / FRACTAL_PRV, RES / FRACTAL_PRV}, BRNNG_SHP, &ocl, (t_uint2){RES, 1 * RES / FRACTAL_PRV});
	init_f_i(&mngr.imgs[RIGHT3_I], &mlx, TANTAN_START, (t_uint2){RES / FRACTAL_PRV, RES / FRACTAL_PRV}, TANTAN, &ocl, (t_uint2){RES, 2 * RES / FRACTAL_PRV});
	init_f_i(&mngr.imgs[RIGHT4_I], &mlx, JULIA_START, (t_uint2){RES / FRACTAL_PRV, RES / FRACTAL_PRV}, JULIA, &ocl, (t_uint2){RES, 3 * RES / FRACTAL_PRV});
	init_f_i(&mngr.imgs[RIGHT5_I], &mlx, JULIA_START, (t_uint2){RES / FRACTAL_PRV, RES / FRACTAL_PRV}, JULIA_TANTAN, &ocl, (t_uint2){RES, 4 * RES / FRACTAL_PRV});
	init_f_i(&mngr.imgs[RIGHT6_I], &mlx, JULIA_START, (t_uint2){RES / FRACTAL_PRV, RES / FRACTAL_PRV}, JULIA_FABSTAN, &ocl, (t_uint2){RES, 5 * RES / FRACTAL_PRV});
	init_f_i(&mngr.imgs[RIGHT7_I], &mlx, JULIA_START, (t_uint2){RES / FRACTAL_PRV, RES / FRACTAL_PRV}, JULIA_FABSFABS, &ocl, (t_uint2){RES, 6 * RES / FRACTAL_PRV});
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr.imgs[MAIN_I].img_ptr, 0, 0);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr.imgs[RIGHT1_I].img_ptr, RES, mngr.imgs[RIGHT1_I].pos.y);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr.imgs[RIGHT2_I].img_ptr, RES, mngr.imgs[RIGHT2_I].pos.y);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr.imgs[RIGHT3_I].img_ptr, RES, mngr.imgs[RIGHT3_I].pos.y);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr.imgs[RIGHT4_I].img_ptr, RES, mngr.imgs[RIGHT4_I].pos.y);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr.imgs[RIGHT5_I].img_ptr, RES, mngr.imgs[RIGHT5_I].pos.y);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr.imgs[RIGHT6_I].img_ptr, RES, mngr.imgs[RIGHT6_I].pos.y);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], mngr.imgs[RIGHT7_I].img_ptr, RES, mngr.imgs[RIGHT7_I].pos.y);

	mlx_hook(mlx.win_ptr[mlx.cw], 2, 5, hook_keydwn, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 3, 5, hook_keyrelease, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 17, (1L << 3), frct_close, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 4, 0, &mouse_hook, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 6, 0, &mouse_move_handle, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 5, 0, &mouse_release, (void*[]){&ocl, &mngr, &mlx});
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
