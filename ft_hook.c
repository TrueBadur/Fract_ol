/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hook.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 19:44:47 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/01/18 18:13:44 by bparker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <stdio.h>

int		hook_keydwn(int key, void *param)
{

	param = NULL;
	if (key == 53)
		exit(0);
	return (0);
}

int		mouse_hook(int but, int x, int y, void *param)
{
	static int num = 0;
	static int butc = 0;
	t_float3 *v;

	if (butc == but)
		num++;
	else
		num = 1;
	butc = but;
	v = (t_float3*)param;
	if (but == 4 || but == 5 || but == 3)
	{
		printf("%d\n", but);
		if (but == 3)
		{
			v->x = v->x + (x / 2.0 / 1024 * 4 + 2);
			v->y = v->y + (y / 2.0 / 1024 * 4 - 2);
			printf("%f, %f\n", v->x, v->y);
			if (num == 2)
				printf("test\n");
		}
		else if (but == 4)
		{
			v->x = v->x + x * (v->z - v->z / MOUSE_SCROL_SCALE);
			v->y =  v->y - y * (v->z - v->z / MOUSE_SCROL_SCALE);
			v->z /= MOUSE_SCROL_SCALE;
			printf("float3(%f, %f, %f) %f\n", v->x, v->y, v->z, v->x + v->z * 1024);
		}
		else
		{
			v->x = v->x + x * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->y =  v->y - y * (v->z - v->z * MOUSE_SCROL_SCALE);
			v->z *= MOUSE_SCROL_SCALE;
			printf("float3(%f, %f, %f) %f\n", v->x, v->y, v->z, v->x + v->z * 1024);
		}

	}
	return (0);
}
