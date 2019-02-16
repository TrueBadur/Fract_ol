/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 08:58:34 by ehugh-be          #+#    #+#             */
/*   Updated: 2018/11/23 23:52:20 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	while (*s && (*s == ' ' || *s == '\n' || *s == '\t'))
		s++;
	len = ft_strlen(s);
	while (len > 0 && (*(s + len) == ' ' || *(s + len) == '\0' || *(s + len) ==
				'\n' || *(s + len) == '\t'))
		len--;
	if (len <= 0)
		return (ft_strnew(1));
	return (ft_strsub(s, 0, len + 1));
}
