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

#ifndef FRACTOL_H
# define FRACTOL_H
# include "libft/libft.h"
# include "libft/ft_ocl/ocl.h"
# include "keys.h"
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# define MOUSE_SCROL_SCALE 1.2
# define PROGRAM_FILE "mndlbrt.cl"
# define SAVE_FILE "frctl_save.sv"
# define FRCTL_NUM 8
# define FRCTL_PRV (FRCTL_NUM - 1)
# define SAVE_NUM 4
# define COL_PR_NUM 4
# define MAX_SAVEW 16
# define L_COL_W(x) ((x) / (SAVE_NUM + COL_PR_NUM))
# define R_COL_W(x) ((x) / (FRCTL_PRV))
# define MAX_IMGS (FRCTL_NUM + SAVE_NUM + COL_PR_NUM + MAX_SAVEW + 1)
# define CNTRL_D (1 << 1)
# define SHIFT_D (1 << 0)
# define CMND_D (1 << 2)
# define CNTRL_RLS ~(1 << 1)
# define SHIFT_RLS ~(1 << 0)
# define CMND_RLS ~(1 << 2)
# define IS_CNTRL_D (mngr->key_mask & CNTRL_D)
# define IS_SHIFT_D (mngr->key_mask & SHIFT_D)
# define IS_CMND_D (mngr->key_mask & CMND_D)
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

typedef struct	s_frctl_o
{
	t_double3	strt;
	t_float3	col;
	size_t		iter;
	int			iter_mod;
	t_double2	jc;
	char		kern;
}				t_frctl_o;

typedef struct	s_img
{
	void		*img_ptr;
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	t_uint2		res;
	t_int2		pos;
	int			num;

	cl_mem		buf;
	t_frctl_o	opts;
}				t_img;
typedef enum	e_windows
{
	MAIN_W,
	HELP_W,
	SAVE_W,
}				t_windows;

typedef enum	e_frctls
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

typedef enum	e_imgs
{
	INFO_I,
	MAIN_I,
	PR_S,
	COL_PR = PR_S + FRCTL_PRV,
	SAVE_PR = COL_PR + COL_PR_NUM,
	SAVE_PR_IN_W = SAVE_PR + SAVE_NUM,
	SAVEW_END = SAVE_PR_IN_W + MAX_SAVEW,
	END_I = SAVEW_END
}				t_imgs;

typedef enum	e_swap_modes
{
	ALL,
	KERN,
	COL,
	NOCOL,
	NO,
}				t_swap_modes;

typedef struct	s_manager
{
	t_ocl		ocl;
	t_mlx		mlx;
	t_int2		msmvcd[3];
	t_img		imgs[MAX_IMGS];
	int			img_num;
	int			cur_img;
	char		mouse_mask;
	char		key_mask;
	char		info;
	unsigned	res;
	t_vector	*saves;
	int			sv_off;
}				t_manager;

extern t_double3	g_starts[];
extern t_float3		g_cols[];

void			help(t_manager *mngr);
void			draw_saves(t_manager *mngr, int d_off);
int				hook_keyrelease(int key, void *param);
void			save_redraw(t_manager *mngr, int save);
int				ft_redraw(void *param, int nimg);
int				load_img_pr(t_manager *mngr, int imgtl);
void			swap_img(t_img *small, t_img *main, int swp_col);
void			set_img(t_manager *mngr, t_int2 start, int mode, t_img *donor);
void			open_saves(t_manager *mngr);
void			init_main(t_manager *mngr, char kern);
void			init_r_col(t_manager *mngr, char s_kern);
void			init_l_col(t_manager *mngr);
void			init(t_manager *mngr);
void			init_img(t_manager *mngr, int nimg, t_img *img, int win);
void			ft_ocl_make_img(t_img *img, t_ocl *ocl, t_double2 *jc);
int				frct_close(void *param);
int				hook_keydwn(int key, void *param);
int				mouse_hook(int but, int x, int y, void *param);
int				mouse_move_handle(int x, int y, void *param);
int				mouse_release(int but, int x, int y, void *param);
void			draw_empty_save(t_manager *mngr, t_img *img, int i, int new);
void			write_savefile(t_vector *vec, char del);
#endif
