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

void total_free(void *param)
{
	int			i;
	t_manager	*mngr;

	i = -1;
	mngr = (t_manager*)param;
	while(g_kernels[++i])
		clReleaseKernel(mngr->ocl.kern[i]);
	free(((t_ocl**)param)[0]->kern);
}

int frct_close(void *param)
{
	total_free(param);
	exit(0);
	return (0);
}
