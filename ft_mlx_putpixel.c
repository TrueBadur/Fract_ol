/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mlx_putpixel.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 15:59:00 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/02/16 15:59:03 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void ft_mlx_img_putpixel(t_img *img, unsigned int x, unsigned int y, t_color col)
{
    ((int*)img->data)[x * img->size_line + y] = col.col;
}
/*
t_color ft_mlx_getcolor()
{

}*/
