struct Cube {
	int verts[8][3]{
		{-1, -1, -1}, // left bottom back
		{1, -1, -1}, // right bottom back
		{1, 1, -1}, // right top back
		{-1, 1, -1}, // left top back
		{-1, -1, 1}, // left bottom front
		{1, -1, 1}, // right bottom front
		{1, 1, 1}, // right top front
		{-1, 1, 1} // left top front
	};

	int tris[12][3]{
		{3, 1, 0}, // back bl
		{3, 2, 1}, // back tr
		{4, 5, 7}, // front bl
		{7, 5, 6}, // front tr
		{4, 3, 0}, // left bl
		{3, 4, 7}, // left tr
		{5, 1, 2}, // right bl
		{2, 6, 5}, // right tr
		{0, 1, 4}, // bottom br
		{1, 5, 4}, // bottom fl
		{6, 2, 3}, // top br
		{6, 3, 7}  // top fl
	};
};

Cube cube;