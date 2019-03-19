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

void	get_saves(t_manager *mngr, const char *fname)
{
	FILE		*fd;
	int			i;
	t_frctl_o	*tmp;

	if ((fd = fopen(fname, "r")) > 0)
	{
		mngr->saves = ft_vecinit(sizeof(t_frctl_o));
		tmp = malloc(sizeof(t_frctl_o));
		i = -1;
		while (1)
		{
			i++;
			fread(tmp, sizeof(t_frctl_o), 1, fd);
			if (feof(fd))
				break ;
			ft_vecpush(mngr->saves, tmp, sizeof(t_frctl_o));
		}
		fclose(fd);
		if (i == 0)
			mngr->saves = ft_vecinit(4 * sizeof(t_frctl_o));
		free(tmp);
	}
	else
		mngr->saves = ft_vecinit(4 * sizeof(t_frctl_o));
}

void	draw_empty_save(t_manager *mngr, t_img *img, int i, int new)
{
	t_mlx *mlx;

	mlx = &mngr->mlx;
	img->opts.kern = -1;
	if (new)
		init_img(mngr, i, NULL, MAIN_W);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr[MAIN_W],
							img->img_ptr, img->pos.x, img->pos.y);
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr[MAIN_W],
		img->pos.x + 30, img->pos.y + img->res.y / 2, 0x00aaaaaa, "Save slot");
}

void	init_save(t_manager *mngr, int i)
{
	t_img		*img;
	t_vector	*v;
	int			s;

	img = &mngr->imgs[i];
	v = mngr->saves;
	s = v->len / sizeof(t_frctl_o);
	if (s < i - SAVE_PR + 1)
		draw_empty_save(mngr, img, i, 1);
	else
	{
		img->opts = ((t_frctl_o*)v->data)[s - (i - SAVE_PR) - 1];
		init_img(mngr, i, NULL, MAIN_W);
	}
}
