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
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# define  MOUSE_SCROL_SCALE 2.0;
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
	t_vec2		res[10];
}				t_mlx;

typedef struct	s_img
{
	void		*img_ptr;
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	t_vec2		res;
}				t_img;

typedef union	u_color
{
	int		col;
	char	argb[4];
}				t_color;

void			ft_mlx_img_putpixel(t_img *img, unsigned int x, unsigned int y,
		t_color col);
int				ft_error(int er);
void			img_drawpxl(char *img_data, void *mlx_ptr, t_vec4 dot,
		int line_size);
void			img_drawpxlsafe(char *img_data, t_mlx mlx, t_vec4 dot,
		int line_size);
void			img_drawline(t_vec4 dot0, t_vec4 dot1, t_mlx mlx, t_img cimg);
void			img_drawlinesafe(t_vec4 dot0, t_vec4 dot1, t_mlx mlx,
		t_img cimg);
int				border_check(t_vec4 dot0, t_vec4 dot1, t_vec2 map);
void			ft_transform_point(t_vec4 *v, void *data);
void			print_map(t_vec4 *v, void *data);
void			ft_point_move(t_vec4 *vec4, void *data);
t_mtrx			*x_rot_mtrx(int x);
t_mtrx			*y_rot_mtrx(int y);
t_mtrx			*z_rot_mtrx(int z);
t_mtrx			*ft_scale_mtrx(t_vec3_f v);
t_mtrx			*ft_rotate_mtrx(t_vec3 v);
t_mtrx			*ft_move_mtrx(t_vec3 v);
t_mtrx			*ft_persp_mtrx(int z);
void			mlx_free(t_mlx **mlx);
int				img_to_win(void *param);
int				hook_keydwn(int key, void *param);
int				mouse_hook(int but, int x, int y, void *param);
#endif
