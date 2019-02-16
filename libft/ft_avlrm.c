/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_avldel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:55:58 by ehugh-be          #+#    #+#             */
/*   Updated: 2018/12/07 02:47:12 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_btavl	*ft_avlrmmin(t_btavl *tr)
{
	t_btavl	*tmp;

	if (!tr->left)
		return (tr);
	tmp = ft_avlrmmin(tr->left);
	tr->left = tmp->right;
	tmp->right = ft_avlbal(tr);
	return (tmp);
}

t_btavl			*ft_avlrem(t_btavl *tr, int key, void (*f)(void **))
{
	t_btavl *tmp;

	if (!tr)
		return (NULL);
	if (key == tr->key)
	{
		f(&(tr->data));
		if (!tr->right)
		{
			tmp = tr->left;
			free(tr);
			return (tmp);
		}
		tmp = ft_avlrmmin(tr->right);
		tmp->left = tr->left;
		free(tr);
		return (ft_avlbal(tmp));
	}
	else if (key < tr->key)
		tr->left = ft_avlrem(tr->left, key, f);
	else
		tr->right = ft_avlrem(tr->right, key, f);
	return (ft_avlbal(tr));
}
