__kernel void mndlbrt(__global int *img, const uint2 it_len, const double3 start)
{
	double2 prev;
	double2 value;
	unsigned int gi;
	unsigned int i;
	double xs = 0, ys = 0;
	float sn;

	prev = (double2){0.0, 0.0};
	gi = get_global_id(0);
	value = (double2){gi % it_len.y, gi / it_len.y};
	value.x = start.x + value.x * start.z;
	value.y = start.y - value.y * start.z;
	i = 0;
	for (; i < it_len.x && xs + ys < 256; i++)
	{
		prev = (double2){xs - ys + value.x,
						 2 * prev.x * prev.y + value.y};
		xs = prev.x * prev.x;
		ys = prev.y * prev.y;
	}
	if(xs + ys != 1)
		sn = (float)i - log2(log2(xs + ys)) + 4.0;
	else
		sn = (float)i + 4.0;
	double4 bla = (0.5 + 0.5*cos((3.0 + sn*0.15 + (double4){0.0,0.0,0.6,1.0})));
	img[gi] = (((int)(256 * bla.y)) << 16) | ((int)(256 * bla.z)) << 8 | ((int)(256 * bla.w));
}