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
#include "libft/ocl.h"
#define PROGRAM_FILE "mndlbrt.cl"
#define KERNEL_FUNC "mndlbrt"
#define RES 1024

void ft_mlx_create_new_window(t_mlx *mlx, t_int2 res, char *name)
{
	mlx->cw += mlx->wc ? 1 : 0;
	mlx->res[mlx->cw] = res;
	mlx->win_ptr[mlx->cw] = mlx_new_window(mlx->mlx_ptr, mlx->res[mlx->cw].x,
										 mlx->res[mlx->cw].y, name);
	mlx->wc++;
}

static void init_mlx(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	mlx->wc = 0;
	mlx->cw = 0;
}

void ft_ocl_make_img(t_img *img, t_ocl *ocl, t_double3 start)
{
	size_t ls;
	cl_int err;
	size_t iter;
	size_t gs;

	ls = img->size_line / 4;
	gs = img->res.x * img->res.y;
	iter = 20;
	err = clSetKernelArg(ocl->kernel, 1, sizeof(unsigned int) * 2, &(unsigned int[]){iter, ls});
	err |= clSetKernelArg(ocl->kernel, 2, sizeof(t_double3), &start);
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
	t_mlx		mlx;
	t_img		img;
	t_ocl		ocl;
	t_double3	start;

	ac = ac + 0;
	av = av + 0;
	init_mlx(&mlx);	
	ft_mlx_create_new_window(&mlx, (t_int2){RES, RES}, "test");
	//create mandelbrot img
	//init img
	img.img_ptr = mlx_new_image(mlx.mlx_ptr, mlx.res[mlx.cw].x, mlx.res[mlx.cw].y);
	img.data = mlx_get_data_addr(img.img_ptr, &img.bpp, &img.size_line, &img.endian);
	img.res = (t_uint2){RES, RES};
	start = (t_double3){-2.0, 2.0, 4.0 / 1024};
	ft_ocl_dev_cont_prog(&ocl, PROGRAM_FILE);
	ft_ocl_set_env(&img , &ocl);
	ft_ocl_make_img(&img, &ocl, start);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win_ptr[mlx.cw], img.img_ptr, 0, 0);

	//put img in window
	mlx_hook(mlx.win_ptr[mlx.cw], 2, 5, hook_keydwn, NULL);
	mlx_hook(mlx.win_ptr[mlx.cw], 17, (1L << 3), frct_close, NULL);
	mlx_hook(mlx.win_ptr[mlx.cw], 4, 0, &mouse_hook, (void*[]){&start, &ocl, &img, &mlx});
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
