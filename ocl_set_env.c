/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ocl_set_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 19:36:13 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/19 19:36:13 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	ft_ocl_set_env(t_ocl *ocl)
{
	cl_int	err;
	int		i;

	ocl->kern = malloc(sizeof(cl_kernel) * FRCTL_NUM);
	i = -1;
	while (g_kernels[++i])
	{
		ocl->kern[i] = clCreateKernel(ocl->program, g_kernels[i], &err);
		if (err < 0)
			ft_ocl_err_handler(FT_OCL_KERNEL_ERR);
	}
}
