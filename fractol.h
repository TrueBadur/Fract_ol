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
# include "libft/ft_ocl/ocl.h"
# include "keys.h"
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# define  MOUSE_SCROL_SCALE 1.2
#define PROGRAM_FILE "mndlbrt.cl"
#define FRACTAL_NUMBER 8
#define FRACTAL_PRV (FRACTAL_NUMBER - 1)
#define RES 1200
#define MNDLBRT_START (t_double3){-2.15, -1.5, 3.0 / RES}
#define JULIA_START (t_double3){-1.76, -1.56, 3.4 / RES}
#define BRNNG_SHP_START (t_double3){-2.35, -2.55, 4.0 / RES}
#define TANTAN_START (t_double3){-17.48, -17.6, 35.0 / RES}
#define FABSTAN_START (t_double3){-17.48, -17.6, 35.0 / RES}
#define MAX_IMGS 10
# define CNTRL_DOWN (1 << 1)
# define SHIFT_DOWN (1 << 0)
# define CMND_DOWN (1 << 2)
# define CNTRL_RLS ~(1 << 1)
# define SHIFT_RLS ~(1 << 0)
# define CMND_RLS ~(1 << 2)
# define IS_CNTRL_DOWN (mngr->key_mask & CNTRL_DOWN)
# define IS_SHIFT_DOWN (mngr->key_mask & SHIFT_DOWN)
# define INFO_I_RES (t_int2){RES, 30}


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
	char		kern;
	int			bpp;
	int			size_line;
	int			endian;
	t_uint2		res;
	t_uint2		pos;

	cl_mem 		buf;
	size_t 		iter;
	int 		iter_mod;
	t_double3	start;
	t_float3	col;
}				t_img;

typedef union	u_color
{
	int		col;
	char	argb[4];
}				t_color;

typedef enum
{
	MNDLBRT,
	BRNNG_SHP,
	TANTAN,
	FABS_TAN,
	JULIA,
	JULIA_TANTAN,
	JULIA_FABSTAN,
	JULIA_FABSFABS,
}				t_frctls;

typedef enum
{
	INFO_I,
	MAIN_I,
	RIGHT1_I,
	RIGHT2_I,
	RIGHT3_I,
	RIGHT4_I,
	RIGHT5_I,
	RIGHT6_I,
	RIGHT7_I
}				t_imgs;

typedef struct
{
	t_int2	mse_mv_crd[3];
	t_double2	jc;
	t_img	imgs[MAX_IMGS];
	int 	img_num;
	int 	cur_img;
	char 	mouse_mask;
	char 	key_mask;
	char	info;
}				t_manager;

void 			ft_ocl_make_img(t_img *img, t_ocl *ocl, t_double2 *jc);
int 			frct_close(void *param);
int				hook_keydwn(int key, void *param);
int				mouse_hook(int but, int x, int y, void *param);
int				mouse_move_handle(int x, int y, void *param);
int				mouse_release(int but, int x, int y, void *param);
int				hook_keyrelease(int key, void *param);
#endif
