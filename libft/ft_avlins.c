/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_avlins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 15:28:41 by ehugh-be          #+#    #+#             */
/*   Updated: 2018/12/06 19:08:18 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_btavl	*ft_avlins(t_btavl *tr, t_btavl *node)
{
	if (!node)
		return (tr);
	if (!tr)
		return (node);
	if (node->key == tr->key)
		return (tr);
	if (node->key < tr->key)
		tr->left = ft_avlins(tr->left, node);
	else
		tr->right = ft_avlins(tr->right, node);
	return (ft_avlbal(tr));
}
