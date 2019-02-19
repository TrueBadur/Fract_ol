__kernel void mndlbrt(__global int *img, const uint2 it_len, const double3 start)
{
	double2 prev;
	double2 value;
	unsigned int gi;
	unsigned int i;

	//printf("starrt = %d\n",len);
	prev = (double2){0.0, 0.0};
	gi = get_global_id(0);
	value = (double2){gi % it_len.y, gi / it_len.y};
	value.x = start.x + value.x * start.z;
	value.y = start.y - value.y * start.z;
	//printf("%f, %f\n", value.x, value.y);
	i = 0;
	for (; i < it_len.x; i++)
	{
		prev = (double2){prev.x * prev.x - prev.y * prev.y + value.x,
				  2 * prev.x * prev.y + value.y};
		if (prev.x * prev.x + prev.y * prev.y > 4)
			break ;
	}
	//printf("i = %d\n", i);
	//img[gi] = (i == iters) ? 0 : 0x00FF0000;
	//img[gi] = (int)(i / 100.0);
	//printf("%f\n", (i / iters * 256.0));
	float a = ((float)i / it_len.x) * 0x0ff00f00;
	img[gi] = (int)a;
}