/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/24 14:08:34 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/01/18 18:15:39 by bparker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include "libft/libft.h"
# include "libft/ocl.h"
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# define  MOUSE_SCROL_SCALE 1.2
#define PROGRAM_FILE "mndlbrt.cl"
#define KERNEL_FUNC "mndlbrt"
#define KERN_NUMBER 1
#define RES 1024
# define MW_NAME "FDF by bparker and ehugh-be"
# define CONT_ERR 5
# define CONT_ERR_MSG "error: wrong data in file\n"
# define CONT_ERR_CS 51
# define CONT_ERR_CS_MSG "wrong start of color sequence\n"
# define CONT_ERR_CE 52
# define CONT_ERR_CE_MSG "wrong end of color sequence\n"
# define CONT_ERR_NE 53
# define CONT_ERR_NE_MSG "wrong number format\n"
# define CONT_ERR_NO 54
# define CONT_ERR_NO_MSG "wrong number of objects in line\n"
# define FILE_ERROR 3
# define FILE_ERROR_MSG "error: can't open file\n"
# define ARG_ERROR 3
# define ARG_ERROR_MSG "error: insufficient number of arguments\n"
# define USAGE_MSG "usage: ./fdf file_to_open [window_width window_height]\n"

typedef struct	s_mlx
{
	void		*mlx_ptr;
	void		*win_ptr[10];
	size_t		wc;
	size_t		cw;
	t_int2		res[10];
}				t_mlx;

typedef struct	s_img
{
	void		*img_ptr;
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	t_uint2		res;
	cl_mem 		buf;
}				t_img;

typedef union	u_color
{
	int		col;
	char	argb[4];
}				t_color;

void 		ft_ocl_make_img(t_img *img, t_ocl *ocl, t_double3 start);
int				hook_keydwn(int key, void *param);
int				mouse_hook(int but, int x, int y, void *param);
#endif
