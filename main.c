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
	cl_int e;
	size_t gs;
	short kern;

	ls = (img->size_line < 0) ? 0 : (size_t)(img->size_line / 4);
	gs = img->res.x * img->res.y;
	kern = img->opts.kern;
	e = clSetKernelArg(ocl->kern[kern], 0, sizeof(cl_mem), &img->buf);
	e |= clSetKernelArg(ocl->kern[kern], 1, sizeof(size_t) * 2,
			&(size_t[]){img->opts.iter, ls});
	e |= clSetKernelArg(ocl->kern[kern], 2, sizeof(t_double3), &img->opts.strt);
	e |= clSetKernelArg(ocl->kern[kern], 3, sizeof(t_float3), &img->opts.col);
	if (kern >= JULIA)
		e |= clSetKernelArg(ocl->kern[kern], 4, sizeof(t_double2), jc);
	if (e < 0)
		ft_ocl_err_handler(FT_OCL_KERNEL_ARG_ERR);
	if (clEnqueueNDRangeKernel(ocl->queue, ocl->kern[kern], 1, NULL,
			&gs, NULL, 0, NULL, NULL) < 0)
		ft_ocl_err_handler(FT_OCL_ENQUEUE_KERNEL_ERR);
	if (clEnqueueReadBuffer(ocl->queue, img->buf, CL_TRUE, 0, sizeof(int) * gs,
			img->data, 0, NULL, NULL) < 0)
		ft_ocl_err_handler(FT_OCL_READ_BUF_ERR);
}

int			main(int ac, char **av)
{
	t_manager	mngr;
	t_mlx		mlx;

	ac = ac + 0;
	av = av + 0;

	mngr.res = 1200; //TODO get from arguments;
	init(&mngr);
	init_main(&mngr, MNDLBRT);
	init_previews(&mngr, MNDLBRT);
	init_col_previews(&mngr, MNDLBRT);
	mlx = mngr.mlx;
	mlx_hook(mlx.win_ptr[mlx.cw], 2, 5, hook_keydwn, (void*)&mngr);
	mlx_hook(mlx.win_ptr[mlx.cw], 3, 5, hook_keyrelease, (void*)&mngr);
	mlx_hook(mlx.win_ptr[mlx.cw], 17, (1L << 3), frct_close, (void*)&mngr);
	mlx_hook(mlx.win_ptr[mlx.cw], 4, 0, &mouse_hook, (void*)&mngr);
	mlx_hook(mlx.win_ptr[mlx.cw], 6, 0, &mouse_move_handle, (void*)&mngr);
	mlx_hook(mlx.win_ptr[mlx.cw], 5, 0, &mouse_release, (void*)&mngr);
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
