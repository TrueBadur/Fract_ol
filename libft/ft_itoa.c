/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 15:32:43 by ehugh-be          #+#    #+#             */
/*   Updated: 2018/11/23 22:36:27 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(long n)
{
	long	nb;
	int		len;
	char	*ret;

	len = 2;
	if (n < 0)
		len++;
	nb = n;
	while (!(nb < 10 && nb > -10))
	{
		len++;
		nb /= 10;
	}
	if (!(ret = (char *)malloc(sizeof(char) * len)))
		return (NULL);
	ret[--len] = '\0';
	nb = (n < 0) ? -1 : 1;
	while (n || len)
	{
		ret[--len] = n % 10 * nb + '0';
		n /= 10;
	}
	if (nb < 0)
		ret[0] = '-';
	return (ret);
}

int		ft_itoa_buf(long n, char *res)
{
	long	nb;
	int		len;
	char	*rt;

	rt = res;
	len = 2;
	if (n < 0)
		len++;
	nb = n;
	while (!(nb < 10 && nb > -10))
	{
		len++;
		nb /= 10;
	}
	res[--len] = '\0';
	nb = (n < 0) ? -1 : 1;
	while (n || len)
	{
		res[--len] = n % 10 * nb + '0';
		n /= 10;
	}
	if (nb < 0)
		res[0] = '-';
	return (len);
}
