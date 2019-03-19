/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 03:54:06 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/01 03:54:06 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	free_ocl(t_manager *mngr)
{
	int	i;

	i = -1;
	while (g_kernels[++i])
		clReleaseKernel(mngr->ocl.kern[i]);
	i = mngr->img_num;
	while (i--)
		clReleaseMemObject(mngr->imgs[i].buf);
	clReleaseCommandQueue(mngr->ocl.queue);
	clReleaseProgram(mngr->ocl.program);
	clReleaseContext(mngr->ocl.context);
	clReleaseDevice(mngr->ocl.device);
}

void	free_mlx(t_manager *mngr)
{
	int	i;

	i = mngr->img_num;
	while (i--)
		if (mngr->imgs[i].img_ptr)
			mlx_destroy_image(mngr->mlx.mlx_ptr, mngr->imgs[i].img_ptr);
	i = SAVE_W + 1;
	while (i--)
		if (mngr->mlx.win_ptr[i])
			mlx_destroy_window(mngr->mlx.mlx_ptr, mngr->mlx.win_ptr[i]);
}

void	total_free(void *param)
{
	t_manager	*mngr;

	mngr = (t_manager*)param;
	write_savefile(mngr->saves, 1);
	free_ocl(mngr);
	free(((t_ocl**)param)[0]->kern);
	free_mlx(mngr);
}

int		frct_close(void *param)
{
	total_free(param);
	exit(0);
	return (0);
}
