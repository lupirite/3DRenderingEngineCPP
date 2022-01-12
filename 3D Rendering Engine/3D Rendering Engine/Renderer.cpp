internal void
clear_screen(unsigned int color) {
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void
draw_rect_in_pixels(int x0, int y0, int x1, int y1, unsigned int color) {

	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++) {
		unsigned int* pixel = (unsigned int*)render_state.memory + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

internal void
draw_tri_in_pixels(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int color) {

	int minX = clamp(0, min3(x1, x2, x3), render_state.width);
	int minY = clamp(0, min3(y1, y2, y3), render_state.height);
	int maxX = clamp(0, max3(x1, x2, x3), render_state.width);
	int maxY = clamp(0, max3(y1, y2, y3), render_state.height);

	for (int y = minY; y < maxY; y++) {
		for (int x = minX; x < maxX; x++) {
			if (isInside(x1, y1, x2, y2, x3, y3, x, y)) {
				unsigned int* pixel = (unsigned int*)render_state.memory + x + y * render_state.width;
				*pixel++ = color;
			}
		}
	}
}

internal void
draw_tri3D_in_pixels(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, unsigned int color, float**& zBuffer) {

	int minX = clamp(0, min3(x1, x2, x3), render_state.width);
	int minY = clamp(0, min3(y1, y2, y3), render_state.height);
	int maxX = clamp(0, max3(x1, x2, x3), render_state.width);
	int maxY = clamp(0, max3(y1, y2, y3), render_state.height);

	for (int y = minY; y < maxY; y++) {
		for (int x = minX; x < maxX; x++) {
			if (isInside(x1, y1, x2, y2, x3, y3, x, y)) {
				float z = avg3(x1, y1, x2, y2, x3, y3, z1, z2, z3, x, y);
				if (z > 0) {
					if (zBuffer[x][y] > z || zBuffer[x][y] == 0) {
						zBuffer[x][y] = z;
						unsigned int* pixel = (unsigned int*)render_state.memory + x + y * render_state.width;
						// color
						*pixel++ = color;
					}
				}
			}
		}
	}
}

global_variable float render_scale = 0.01f;

internal void
draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color) {

	x *= render_state.height*render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	// Change to pixels
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void
draw_tri(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color) {
	if (isClockwise(x1, y1, x2, y2, x3, y3)) {
		x1 *= render_state.height * render_scale;
		y1 *= render_state.height * render_scale;
		x2 *= render_state.height * render_scale;
		y2 *= render_state.height * render_scale;
		x3 *= render_state.height * render_scale;
		y3 *= render_state.height * render_scale;

		x1 += render_state.width / 2.f;
		y1 += render_state.height / 2.f;
		x2 += render_state.width / 2.f;
		y2 += render_state.height / 2.f;
		x3 += render_state.width / 2.f;
		y3 += render_state.height / 2.f;

		draw_tri_in_pixels(x1, y1, x2, y2, x3, y3, color);
	}
}

internal void
draw_tri3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float**& zBuffer, unsigned int color) {
	if (isClockwise(x1, y1, x2, y2, x3, y3)) {
		x1 *= render_state.height * render_scale;
		y1 *= render_state.height * render_scale;
		x2 *= render_state.height * render_scale;
		y2 *= render_state.height * render_scale;
		x3 *= render_state.height * render_scale;
		y3 *= render_state.height * render_scale;

		x1 += render_state.width / 2.f;
		y1 += render_state.height / 2.f;
		x2 += render_state.width / 2.f;
		y2 += render_state.height / 2.f;
		x3 += render_state.width / 2.f;
		y3 += render_state.height / 2.f;

		draw_tri3D_in_pixels(x1, y1, z1, x2, y2, z2, x3, y3, z3, color, zBuffer);
	}
}

internal void
draw_cube(float pos[3], float rotation[2], float**& zBuffer, unsigned int color) {
	float vertScreenPos[8][3];
	for (int i = 0; i < sizeof(cube.verts) / sizeof(cube.verts[0]); i++) {
		float p[3];
		// X
		p[1] = rotate_point(cube.verts[i][1] + pos[1] * 2, cube.verts[i][2] + pos[2] * 2, rotation[0] * (pi / 180))[0];
		p[2] = rotate_point(cube.verts[i][1] + pos[1] * 2, cube.verts[i][2] + pos[2] * 2, rotation[0] * (pi / 180))[1];
		// Y
		p[0] = rotate_point(cube.verts[i][0] + pos[0] * 2, p[2], rotation[1] * (pi / 180))[0];
		p[2] = rotate_point(cube.verts[i][0] + pos[0] * 2, p[2], rotation[1] * (pi / 180))[1];
		// Z
		p[0] = rotate_point(p[0], p[1], rotation[2] * (pi / 180))[0];
		p[1] = rotate_point(p[0], p[1], rotation[2] * (pi / 180))[1];
		p[0] *= 20 / p[2] * 2;
		p[1] *= 20 / p[2] * 2;
		vertScreenPos[i][0] = p[0];
		vertScreenPos[i][1] = p[1];
		vertScreenPos[i][2] = p[2];
	}
	for (int i = 0; i < sizeof(cube.tris) / sizeof(cube.tris[0]); i++) {
		draw_tri3D(vertScreenPos[cube.tris[i][0]][0], vertScreenPos[cube.tris[i][0]][1], vertScreenPos[cube.tris[i][0]][2], vertScreenPos[cube.tris[i][1]][0], vertScreenPos[cube.tris[i][1]][1], vertScreenPos[cube.tris[i][1]][2], vertScreenPos[cube.tris[i][2]][0], vertScreenPos[cube.tris[i][2]][1], vertScreenPos[cube.tris[i][2]][2], zBuffer, color);
	}
}