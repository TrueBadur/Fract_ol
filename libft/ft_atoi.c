/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 11:48:55 by ehugh-be          #+#    #+#             */
/*   Updated: 2018/11/24 02:31:16 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int				answer;
	int				pos;
	unsigned int	d;

	answer = 0;
	pos = 1;
	while (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' || *str
			== '\v' || *str == '\f')
		str++;
	if (*str == '-')
	{
		pos = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (1)
	{
		if ((d = *str - '0') > 9)
			break ;
		answer = answer * 10 + d;
		str++;
	}
	return (answer * pos);
}
