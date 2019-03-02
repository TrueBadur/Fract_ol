/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 05:57:32 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/02/26 05:57:32 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef KEYS_H
#define KEYS_H
# define ITER_P key == 24
# define ITER_M key == 27
# define SHIFT (key == 257)
# define CNTRL (key == 256)
# define CMND (key == 259)
# define INFO (key == 34)
# define RESTART key == 15
# define RMB_HOLD (mngr->mouse_mask & (1 << 1))
# define LMB_HOLD (mngr->mouse_mask & (1 << 0))
#endif