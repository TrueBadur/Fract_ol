/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_avlsearch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 18:43:37 by ehugh-be          #+#    #+#             */
/*   Updated: 2018/12/06 18:48:32 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_avlsearch(t_btavl *tr, int key)
{
	if (!tr)
		return (NULL);
	if (tr->key == key)
		return (tr->data);
	if (key < tr->key)
		return (ft_avlsearch(tr->left, key));
	else
		return (ft_avlsearch(tr->right, key));
}
