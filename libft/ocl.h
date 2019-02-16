/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ocl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 20:26:24 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/02/16 20:26:27 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OCL_H
#define FT_OCL_H
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <stdio.h>

typedef struct s_ocl
{
    cl_context      context;
    cl_device_id    device;
    cl_program      program;
    cl_kernel       kernel;
    cl_command_queue    queue;
};

cl_device_id ft_create_device(void);
#endif
