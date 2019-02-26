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
#define PROGRAM_FILE "mndlbrt.cl"
#define KERNEL_FUNC "mndlbrt"


void ft_mlx_create_new_window(t_mlx *mlx, t_int2 res, char *name)
{
	mlx->cw += mlx->wc ? 1 : 0;
	mlx->res[mlx->cw] = res;
	mlx->win_ptr[mlx->cw] = mlx_new_window(mlx->mlx_ptr, mlx->res[mlx->cw].x,
										 mlx->res[mlx->cw].y, name);
	mlx->wc++;
}

static void init(t_mlx *mlx, t_manager *mngr)
{
	mlx->mlx_ptr = mlx_init();
	mlx->wc = 0;
	mlx->cw = 0;
	mngr->img_num = 1;
	mngr->cur_img = 0;
	mngr->mouse_mask = 0;
	mngr->key_mask = 0;
	mngr->imgs[mngr->cur_img].col = (t_float3){0.0, 0.6, 1.0};
}


void ft_ocl_make_img(t_img *img, t_ocl *ocl)
{
	size_t ls;
	cl_int err;
	size_t gs;

	ls = img->size_line / 4;
	gs = img->res.x * img->res.y;

	err = clSetKernelArg(ocl->kernel, 1, sizeof(unsigned int) * 2, &(unsigned int[]){img->iter, ls});
	err |= clSetKernelArg(ocl->kernel, 2, sizeof(t_double3), &img->start);
	err |= clSetKernelArg(ocl->kernel, 3, sizeof(t_float3), &img->col);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_KERNEL_ARG_ERR);
	err = clEnqueueNDRangeKernel(ocl->queue, ocl->kernel, 1, NULL, &gs, NULL, 0, NULL, NULL);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_ENQUEUE_KERNEL_ERR);
	err = clEnqueueReadBuffer(ocl->queue, img->buf, CL_TRUE, 0, sizeof(int) * gs, img->data, 0, NULL, NULL);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_READ_BUF_ERR);
}

void ft_ocl_set_env(t_img *img, t_ocl *ocl)
{
	size_t gs;
	cl_int err;

	gs = (size_t)img->res.x * img->res.y;
	img->buf = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE, sizeof(int) * gs, NULL, &err);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_BUFFER_ERR);
	ocl->queue = clCreateCommandQueue(ocl->context, ocl->device, 0, &err);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_QUEUE_ERR);
	ocl->kernel = clCreateKernel(ocl->program, KERNEL_FUNC, &err);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_KERNEL_ERR);
	err = clSetKernelArg(ocl->kernel, 0, sizeof(cl_mem), &img->buf);
	if (err < 0)
		ft_ocl_err_handler(err, FT_OCL_KERNEL_ARG_ERR);
}

int frct_close(void *param)
{
	(void)param;
	exit(0);
	return (0);
}

int			main(int ac, char **av)
{
	t_manager	mngr;
	t_mlx		mlx;
	t_img		*img;
	t_ocl		ocl;

	ac = ac + 0;
	av = av + 0;
	init(&mlx, &mngr);
	ft_mlx_create_new_window(&mlx, (t_int2){RES, RES}, "test");
	//create mandelbrot img
	//init img
	img = &mngr.imgs[0];

	img->img_ptr = mlx_new_image(mlx.mlx_ptr, mlx.res[mlx.cw].x, mlx.res[mlx.cw].y);
	img->data = mlx_get_data_addr(img->img_ptr, &img->bpp, &img->size_line, &img->endian);
	img->res = (t_uint2){RES, RES};
	img->start = (t_double3){-2.0, 2.0, 4.0 / RES};
	img->iter = 101;
	ft_ocl_dev_cont_prog(&ocl, PROGRAM_FILE);
	ft_ocl_set_env(img , &ocl);
	ft_ocl_make_img(img, &ocl);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], img->img_ptr, 0, 0);

	//put img in window
	mlx_hook(mlx.win_ptr[mlx.cw], 2, 5, hook_keydwn, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 3, 5, hook_keyrelease, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 17, (1L << 3), frct_close, NULL);
	mlx_hook(mlx.win_ptr[mlx.cw], 4, 0, &mouse_hook, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 6, 0, &mouse_move_handle, (void*[]){&ocl, &mngr, &mlx});
	mlx_hook(mlx.win_ptr[mlx.cw], 5, 0, &mouse_release, (void*[]){&ocl, &mngr, &mlx});
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
