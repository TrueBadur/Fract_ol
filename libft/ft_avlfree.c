/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_avlfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 15:36:52 by ehugh-be          #+#    #+#             */
/*   Updated: 2018/12/06 17:56:38 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_btavl	*ft_avlfree(t_btavl *tr)
{
	if (!tr)
		return (NULL);
	if (tr->left)
		ft_avlfree(tr->left);
	if (tr->right)
		ft_avlfree(tr->right);
	free(tr->data);
	free(tr);
	return (NULL);
}
