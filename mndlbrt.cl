__local int *f() = {&tan, &sin};

__kernel void mndlbrt(__global int *img, const uint2 it_len, const double3 start,
		const float3 col)
{
	double4 prev;
	double2 value;
	unsigned int gi;
	unsigned int i;
	double xs = 0, ys = 0;

	prev = 0.0;
	gi = get_global_id(0);
	value = (double2){gi % it_len.y, gi / it_len.y};
	value.x = start.x + value.x * start.z;
	value.y = start.y - value.y * start.z;
	xs = value.x * value.x;
	ys = value.y * value.y;
	i = 0;
	xs = pow(value.x - 1 / 4.0, 2) + pow(value.y, 2);
	if ((double)(xs * (xs + (value.x - 1 / 4.0))) <= (double)(1 / 4.0 * pow(value.y, 2)))
	{
		img[gi] = 0;
		return;
	}
	for (; i < it_len.x && xs + ys < 256; i++)
	{
		xs = prev.x * prev.x;
		ys = prev.y * prev.y;
		prev.zw = ((double2){xs - ys + value.x,
						 2 * prev.x * prev.y + value.y});
		if (prev.z == prev.x && prev.w == prev.y)
		{
			img[gi] = 0;
			return;
		}
		prev.xy = prev.zw;
	}
	if (i == it_len.x)
	{
		img[gi] = 0;
		return ;
	}
	if(xs + ys != 1)
		prev.x = (float)i - log2(log2(xs + ys)) + 4.0;
	else
		prev.x = (float)i + 4.0;
	double3 bla = 256 * (0.5 + 0.5*cos((3.0 + prev.x * 0.15 + (double3){col.x, col.y, col.z})));
	img[gi] = (((int)(bla.x)) << 16) | ((int)(bla.y)) << 8 | ((int)(bla.z));
}

/*__kernel void julia(__global int *img, const uint2 it_len, const double3 start,
		const float3 col, const float2 c)
{
	unsigned int gi;
	unsigned int i;
	gi = get_global_id(0);
	value = (double2){gi % it_len.y, gi / it_len.y};

	for (; i < it_len.x && xs + ys < 256; i++)
	{
		xs = prev.x * prev.x;
		ys = prev.y * prev.y;
		prev.zw = ((double2){xs - ys + value.x,
							 2 * prev.x * prev.y + value.y});
		if (prev.z == prev.x && prev.w == prev.y)
		{
			img[gi] = 0;
			return;
		}
		prev.xy = prev.zw;
	}
	if(xs + ys != 1)
		prev.x = (float)i - log2(log2(xs + ys)) + 4.0;
	else
		prev.x = (float)i + 4.0;
	double3 bla = 256 * (0.5 + 0.5*cos((3.0 + prev.x * 0.15 + (double3){col.x, col.y, col.z})));
	img[gi] = (((int)(bla.x)) << 16) | ((int)(bla.y)) << 8 | ((int)(bla.z));
}*/

__kernel void brnng_shp(__global int *img, const uint2 it_len, const double3 start,
					  const float3 col)
{
	double4 prev;
	double2 value;
	unsigned int gi;
	unsigned int i;
	double xs = 0, ys = 0;
	double m = test[0](1);

	prev = 0.0;
	gi = get_global_id(0);
	value = (double2){gi % it_len.y, gi / it_len.y};
	value.x = start.x + value.x * start.z;
	value.y = start.y + value.y * start.z;
	xs = value.x * value.x;
	ys = value.y * value.y;
	i = 0;
	for (; i < it_len.x && xs + ys < 256; i++)
	{
		xs = prev.x * prev.x;
		ys = prev.y * prev.y;
		prev.zw = ((double2){xs - ys + value.x),
							 (2 * prev.x * prev.y) + value.y});
		if (prev.z == prev.x && prev.w == prev.y)
		{
			img[gi] = 0;
			return;
		}
		prev.xy = prev.zw;
	}
	if (i == it_len.x)
	{
		img[gi] = 0;
		return ;
	}
	if(xs + ys != 1)
		prev.x = (float)i - log2(log2(xs + ys)) + 4.0;
	else
		prev.x = (float)i + 4.0;
	double3 bla = 256 * (0.5 + 0.5*cos((3.0 + prev.x * 0.15 + (double3){col.x, col.y, col.z})));
	img[gi] = (((int)(bla.x)) << 16) | ((int)(bla.y)) << 8 | ((int)(bla.z));
}

__kernel void tantan(__global int *img, const uint2 it_len, const double3 start,
						const float3 col)
{
	double4 prev;
	double2 value;
	unsigned int gi;
	unsigned int i;
	double xs = 0, ys = 0;

	prev = 0.0;
	gi = get_global_id(0);
	value = (double2){gi % it_len.y, gi / it_len.y};
	value.x = start.x + value.x * start.z;
	value.y = start.y + value.y * start.z;
	xs = value.x * value.x;
	ys = value.y * value.y;
	i = 0;
	for (; i < it_len.x / 10 && xs + ys < 256; i++)
	{
		xs = prev.x * prev.x;
		ys = prev.y * prev.y;
		prev.zw = ((double2){tan(xs - ys + value.x),
							 tan(2 * prev.x * prev.y) + value.y});
		if (prev.z == prev.x && prev.w == prev.y)
		{
			img[gi] = 0;
			return;
		}
		prev.xy = prev.zw;
	}
	if (i == it_len.x)
	{
		img[gi] = 0;
		return ;
	}
	if(xs + ys != 1)
		prev.x = (float)i - log2(log2(xs + ys)) + 4.0;
	else
		prev.x = (float)i + 4.0;
	double3 bla = 256 * (0.5 + 0.5*cos((3.0 + prev.x * 0.15 + (double3){col.x, col.y, col.z})));
	img[gi] = (((int)(bla.x)) << 16) | ((int)(bla.y)) << 8 | ((int)(bla.z));
}

__kernel void fabs_tan(__global int *img, const uint2 it_len, const double3 start,
						const float3 col)
{
	double4 prev;
	double2 value;
	unsigned int gi;
	unsigned int i;
	double xs = 0, ys = 0;

	prev = 0.0;
	gi = get_global_id(0);
	value = (double2){gi % it_len.y, gi / it_len.y};
	value.x = start.x + value.x * start.z;
	value.y = start.y + value.y * start.z;
	xs = value.x * value.x;
	ys = value.y * value.y;
	i = 0;
	for (; i < it_len.x && xs + ys < 256; i++)
	{
		xs = prev.x * prev.x;
		ys = prev.y * prev.y;
		prev.zw = ((double2){fabs(xs - ys + value.x),
							 tan(2 * prev.x * prev.y) + value.y});
		if (prev.z == prev.x && prev.w == prev.y)
		{
			img[gi] = 0;
			return;
		}
		prev.xy = prev.zw;
	}
	if (i == it_len.x)
	{
		img[gi] = 0;
		return ;
	}
	if(xs + ys != 1)
		prev.x = (float)i - log2(log2(xs + ys)) + 4.0;
	else
		prev.x = (float)i + 4.0;
	double3 bla = 256 * (0.5 + 0.5*cos((3.0 + prev.x * 0.15 + (double3){col.x, col.y, col.z})));
	img[gi] = (((int)(bla.x)) << 16) | ((int)(bla.y)) << 8 | ((int)(bla.z));
}