#pragma once

#define GRID_WIDTH 100
#define GRID_HEIGHT 100
#define GRID_CELL_WIDTH 2
#define GRID_CELL_HEIGHT 2
#define GRID_START_X 0
#define GRID_START_Y 0

#define VERTEX_COMPONENTS 3
#define QUAD_COMPONENTS 4

float verts[GRID_WIDTH * GRID_HEIGHT * VERTEX_COMPONENTS];
GLushort elems[GRID_WIDTH * GRID_HEIGHT * QUAD_COMPONENTS];

inline void populateGrid()
{
	for (auto i = 0; i < GRID_WIDTH; i++)
	{
		for (auto j = 0; j < GRID_HEIGHT; j++)
		{
			auto apos = i * GRID_WIDTH + j, ppos = apos * VERTEX_COMPONENTS;
			verts[ppos] = i * GRID_CELL_WIDTH + GRID_START_X;
			verts[ppos + 1] = 0.0f;
			verts[ppos + 2] = -j * GRID_CELL_HEIGHT + GRID_START_Y;

			if (i != (GRID_WIDTH - 1) && j != (GRID_HEIGHT - 1))
			{
				auto epos = apos * QUAD_COMPONENTS, npos = apos + GRID_WIDTH;
				elems[epos] = apos;
				elems[epos + 1] = npos;
				elems[epos + 2] = npos + 1;
				elems[epos + 3] = apos + 1;
			}
		}
	}
}