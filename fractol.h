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
#define FRCTL_NUM 8
#define FRCTL_PRV (FRCTL_NUM - 1)
# define SAVE_NUM 4
# define COL_PR_NUM 3
#define MAX_IMGS FRCTL_PRV + SAVE_NUM + COL_PR_NUM
# define CNTRL_D (1 << 1)
# define SHIFT_D (1 << 0)
# define CMND_D (1 << 2)
# define CNTRL_RLS ~(1 << 1)
# define SHIFT_RLS ~(1 << 0)
# define CMND_RLS ~(1 << 2)
# define IS_CNTRL_D (mngr->key_mask & CNTRL_D)
# define IS_SHIFT_D (mngr->key_mask & SHIFT_D)
# define INFO_H 30

extern char *g_kernels[];
typedef struct	s_mlx
{
	void		*mlx_ptr;
	void		*win_ptr[10];
	size_t		wc;
	size_t		cw;
	t_int2		res[10];
}				t_mlx;

typedef struct
{
	char		kern;
	t_double3	strt;
	t_float3	col;
	size_t		iter;
	int 		iter_mod;
}				t_frctl_o;

typedef struct	s_img
{
	void		*img_ptr;
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	t_uint2		res;
	t_uint2		pos;

	cl_mem 		buf;
	t_frctl_o	opts;
}				t_img;

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
	PR_S,
	COL_PR = PR_S + FRCTL_PRV,
	COL_PR_END = COL_PR + COL_PR_NUM,
}				t_imgs;

typedef struct
{
	t_ocl		ocl;
	t_mlx		mlx;
	t_int2		msmvcd[3];
	t_double2	jc;
	t_img		imgs[MAX_IMGS];
	int			img_num;
	int			cur_img;
	char		mouse_mask;
	char		key_mask;
	char		info;
	unsigned	res;
}				t_manager;

extern t_double3 	g_starts[];
extern t_float3		g_cols[];

void			init_main(t_manager *mngr, char kern);
void			init_previews(t_manager *mngr, char s_kern);
void			init_col_previews(t_manager *mngr, char s_kern);
void			init(t_manager *mngr);
void 			ft_ocl_make_img(t_img *img, t_ocl *ocl, t_double2 *jc);
void 			ft_mlx_create_new_window(t_mlx *mlx, t_int2 res, char *name);
int 			frct_close(void *param);
int				hook_keydwn(int key, void *param);
int				mouse_hook(int but, int x, int y, void *param);
int				mouse_move_handle(int x, int y, void *param);
int				mouse_release(int but, int x, int y, void *param);
int				hook_keyrelease(int key, void *param);
#endif
