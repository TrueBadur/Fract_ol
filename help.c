/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 16:16:27 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/03/17 16:16:27 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int		hook_keydwn_help(int key, void *param)
{
	t_mlx	*mlx;

	mlx = &((t_mngr*)param)->mlx;
	if (key == 53)
	{
		mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr[HELP_W]);
		mlx->win_ptr[HELP_W] = NULL;
	}
	return (0);
}

int		help_close(void *param)
{
	t_mlx	*mlx;

	mlx = &((t_mngr*)param)->mlx;
	mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr[HELP_W]);
	mlx->win_ptr[HELP_W] = NULL;
	return (0);
}

void	wrtoh(t_mlx *mlx, int offset, int col, char *str)
{
	static int	n = 0;

	if (!str)
	{
		n = 0;
		return ;
	}
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr[HELP_W], 15 + offset, 20 * n,
			col, str);
	n++;
}

void	help_put_strings(t_mlx *mlx)
{
	wrtoh(mlx, 0, 0, NULL);
	wrtoh(mlx, 0, 0x00ee649a, "Instructions:");
	wrtoh(mlx, 10, 0x00ee649a, "To pan any image use LMB");
	wrtoh(mlx, 10, 0x00ee649a, "To scale any image use mouse wheel");
	wrtoh(mlx, 10, 0x00ee649a, "Hold and move RMB to change color in MAIN");
	wrtoh(mlx, 35, 0x00ee649a, "+Shift for additional changes");
	wrtoh(mlx, 10, 0x00ee649a,
		"Hold and move MMB for changing Julia-like fractals");
	wrtoh(mlx, 10, 0x00ee649a, "RMB on preview to swap it with main image");
	wrtoh(mlx, 10, 0x00ee649a,
		"RMB on save or color preview to load it in main image");
	wrtoh(mlx, 10, 0x00ee649a,
		"CNTRL + RMB on MAIN image to save current state");
	wrtoh(mlx, 10, 0x00ee649a, "CMND + RMB on SAVE image to delete it");
	wrtoh(mlx, 10, 0x00ee649a, "+/- to increase/decrease number of iterations");
	wrtoh(mlx, 20, 0x00ee649a, "hold CNTRL to increase by 100");
	wrtoh(mlx, 20, 0x00ee649a, "hold SHIFT to increase by 10");
	wrtoh(mlx, 10, 0x00ee649a, "r to return to fractal's base position");
	wrtoh(mlx, 10, 0x00ee649a, "s to open window with all saves");
	wrtoh(mlx, 10, 0x00ee649a, "h to open this window");
}

void	help(t_mngr *mngr)
{
	t_mlx	*mlx;

	mlx = &mngr->mlx;
	if (mlx->win_ptr[HELP_W])
		return ;
	mlx->win_ptr[HELP_W] = mlx_new_window(mlx->mlx_ptr, 700, 500,
										"Fract_ol: Help");
	help_put_strings(mlx);
	if (!(mngr->imgs[MAIN_I].img_ptr))
		wrtoh(mlx, 40, 0x0000ff00, "Press ESC to continue");
	mlx_hook(mlx->win_ptr[HELP_W], 2, 5, hook_keydwn_help, (void*)mngr);
	mlx_hook(mlx->win_ptr[HELP_W], 17, (1L << 3), help_close, (void*)mngr);
}
