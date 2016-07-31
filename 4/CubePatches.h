#define GRID_WIDTH 512
#define GRID_HEIGHT 512

float verts[GRID_WIDTH * GRID_HEIGHT * 3];
GLushort elems[GRID_WIDTH * GRID_HEIGHT * 4];

inline void populateGrid()
{
	for (auto i = 0; i < 512; i++)
	{
		for (auto j = 0; j < 512; j++)
		{
			auto apos = i * 512 + j, ppos = apos * 3;
			verts[ppos] = i;
			verts[ppos + 1] = 0.0f;
			verts[ppos + 2] = -j;

			if (i != 511 && j != 511)
			{
				auto epos = apos * 4, npos = apos + 512;
				elems[epos] = apos;
				elems[epos + 1] = npos;
				elems[epos + 2] = npos + 1;
				elems[epos + 3] = apos + 1;
			}
		}
	}
}