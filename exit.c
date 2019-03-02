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

void	write_savefile(t_vector *vec)
{
	FILE 		*fd;
	int 		i;
	char 		buf[50];

	if (vec->len == 0)
		return ;
	if ((fd = fopen(SAVE_FILE, "w")) != NULL)
	{
		i = vec->len / sizeof(t_frctl_o);
		while (--i + 1)
		{
			if (((t_frctl_o*)vec->data)[i].kern < 0)
				continue ;
			fwrite(&(((t_frctl_o*)vec->data)[i]), sizeof(t_frctl_o), 1, fd);
		}
	}
	ft_vecdel(&vec);
	fclose(fd);
}

void total_free(void *param)
{
	int			i;
	t_manager	*mngr;

	i = -1;
	mngr = (t_manager*)param;
	write_savefile(mngr->saves);
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
