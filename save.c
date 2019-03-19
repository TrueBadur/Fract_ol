/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 14:06:11 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/19 14:06:11 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	write_savefile(t_vector *vec, char del)
{
	FILE		*fd;
	unsigned	i;

	if (vec->len == 0)
		return ;
	if ((fd = fopen(SAVE_FILE, "w")) != NULL)
	{
		i = -1;
		while (++i < vec->len / sizeof(t_frctl_o))
		{
			if (((t_frctl_o*)vec->data)[i].kern < 0)
				continue ;
			fwrite(&(((t_frctl_o*)vec->data)[i]), sizeof(t_frctl_o), 1, fd);
		}
	}
	if (del)
		ft_vecdel((void**)&vec);
	fclose(fd);
}
