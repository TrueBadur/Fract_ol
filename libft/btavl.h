/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btavl.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 16:44:14 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/01/07 16:47:12 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BTAVL_H
# define BTAVL_H
# include "libft.h"

typedef struct		s_btavl
{
	void			*data;
	size_t			size;
	int				key;
	struct s_btavl	*left;
	struct s_btavl	*right;
	unsigned char	h;
}					t_btavl;

t_btavl				*ft_avlnew(void *data, int key, size_t size);
unsigned char		ft_avlh(t_btavl *tr);
int					ft_avlbfact(t_btavl *node);
void				ft_avlfixh(t_btavl *tr);
t_btavl				*ft_avlrr(t_btavl *tr);
t_btavl				*ft_avlrl(t_btavl *tr);
t_btavl				*ft_avlbal(t_btavl *tr);
t_btavl				*ft_avlins(t_btavl *tr, t_btavl *node);
t_btavl				*ft_avlrem(t_btavl *tr, int key, void (*f)(void **));
t_btavl				*ft_avlfree(t_btavl *tr);
void				*ft_avlsearch(t_btavl *tr, int key);
#endif
