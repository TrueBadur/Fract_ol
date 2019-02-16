/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_avlnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 12:57:52 by ehugh-be          #+#    #+#             */
/*   Updated: 2018/12/06 17:56:51 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_btavl	*ft_avlnew(void *data, int key, size_t size)
{
	t_btavl	*ret;

	if (!(ret = malloc(sizeof(t_btavl))))
		return (NULL);
	if (!data)
	{
		ret->data = NULL;
		ret->size = 0;
	}
	else if (!(ret->data = malloc(size)))
	{
		free(ret);
		return (NULL);
	}
	else
	{
		ft_memcpy(ret->data, data, size);
		ret->size = size;
	}
	ret->key = key;
	ret->left = NULL;
	ret->right = NULL;
	ret->h = 1;
	return (ret);
}
