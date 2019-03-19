/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bparker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/23 11:17:51 by bparker           #+#    #+#             */
/*   Updated: 2019/03/19 15:01:59 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "fractol.h"

char *g_kernels[] = {"mndlbrt", "brnng_shp", "tantan", "fabs_tan", "julia",
	"julia_tantan", "julia_fabstan", "julia_fabsfabs", NULL};

void	ft_ocl_make_img(t_img *img, t_ocl *ocl, t_double2 *jc)
{
	size_t	ls;
	cl_int	e;
	size_t	gs;
	short	kern;

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

void	base_help(void)
{
	int tmp;

	write(1, "Usage: ./fractol [resolution of main image] "
				"[name of fractal for main image]\n", 77);
	write(1, "List of available fractals:\n", 28);
	tmp = -1;
	while (g_kernels[++tmp])
	{
		if (tmp > 0)
			write(1, ", ", 2);
		write(1, g_kernels[tmp], ft_strlen(g_kernels[tmp]));
	}
	write(1, "\n", 1);
	exit(0);
}

int		parse_args(t_mngr *mngr, int ac, char **av)
{
	int tmp;
	int i;

	if (ac > 1)
	{
		i = 1;
		if (ac == 2 && !ft_strcmp(av[1], "-h"))
			base_help();
		if ((tmp = ft_atoi(av[1])) > 10 && tmp < 5000 && i++)
			mngr->res = (unsigned)tmp;
		else
			mngr->res = 1300;
		if (i < ac)
		{
			tmp = -1;
			while (g_kernels[++tmp])
				if (!ft_strcmp(av[i], g_kernels[tmp]))
					return (tmp);
		}
		return (0);
	}
	mngr->res = 1300;
	return (0);
}

int		main(int ac, char **av)
{
	t_mngr	mngr;
	t_mlx	mlx;
	int		main_image_num;

	main_image_num = parse_args(&mngr, ac, av);
	init(&mngr);
	help(&mngr);
	init_main(&mngr, (char)main_image_num);
	init_r_col(&mngr, (char)main_image_num);
	init_l_col(&mngr);
	mlx = mngr.mlx;
	mlx_hook(mlx.win_ptr[MAIN_W], 2, 5, hook_keydwn, (void*)&mngr);
	mlx_hook(mlx.win_ptr[MAIN_W], 3, 5, hook_keyrelease, (void*)&mngr);
	mlx_hook(mlx.win_ptr[MAIN_W], 17, (1L << 3), frct_close, (void*)&mngr);
	mlx_hook(mlx.win_ptr[MAIN_W], 4, 0, &mouse_hook, (void*)&mngr);
	mlx_hook(mlx.win_ptr[MAIN_W], 6, 0, &mouse_move_handle, (void*)&mngr);
	mlx_hook(mlx.win_ptr[MAIN_W], 5, 0, &mouse_release, (void*)&mngr);
	mlx_loop(mlx.mlx_ptr);
	return (0);
}
