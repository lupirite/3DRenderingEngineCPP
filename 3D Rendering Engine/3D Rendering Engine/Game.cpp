#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float rotation[3]{ 0, 0, 0 };
float pos[3]{ 0, 0, 3 };

internal void
simulate_game(Input* input, float dt) {
	float** zBuffer = new float*[render_state.width];
	for (int i = 0; i < render_state.width; i++) {
		zBuffer[i] = new float[render_state.height];
		for (int y = 0; y < render_state.height; y++)
			zBuffer[i][y] = 0;
	}
	clear_screen(0x000000);

	float rotSpeed = 150.f; // units per second
	if (is_down(BUTTON_UP)) rotation[0] += rotSpeed * dt;
	if (is_down(BUTTON_DOWN)) rotation[0] -= rotSpeed * dt;
	if (is_down(BUTTON_RIGHT)) rotation[1] += rotSpeed * dt;
	if (is_down(BUTTON_LEFT)) rotation[1] -= rotSpeed * dt;

	int vel[3] = { 0, 0, 0 };
	float moveSpeed = 1.5f; // units per second
	if (is_down(KEY_W)) vel[2] -= 1;
	if (is_down(KEY_A)) vel[0] += 1;
	if (is_down(KEY_S)) vel[2] += 1;
	if (is_down(KEY_D)) vel[0] -= 1;
	if (is_down(BUTTON_SHIFT)) vel[1] += 1;
	if (is_down(BUTTON_SPACE)) vel[1] -= 1;

	float m[3] = { 0, 0, 0 };
	m[0] = norm(vel[0], vel[1], vel[2])[0];
	m[1] = norm(vel[0], vel[1], vel[2])[1];
	m[2] = norm(vel[0], vel[1], vel[2])[2];

	pos[0] += rotate_point(m[0] * moveSpeed * dt, m[2] * moveSpeed * dt, -rotation[1] * (pi / 180))[0];
	pos[1] += m[1] * moveSpeed * dt;
	pos[2] += rotate_point(m[0] * moveSpeed * dt, m[2] * moveSpeed * dt, -rotation[1] * (pi / 180))[1];

	float degrees = 360.f;

	for (int i = 0; i < 3; i++) {
		if (rotation[i] < 0) rotation[i] += 360;
		if (rotation[i] >= 360) rotation[i] = (int)rotation[i] % 360;
	}

	rotation[0] = rotate_point(rotation[0], rotation[2], rotation[1] * (pi / 180))[0];
	rotation[2] = rotate_point(rotation[0], rotation[2], rotation[1] * (pi / 180))[1];

	float pos2[3] = { pos[0] + 0.5f, pos[1] + 0.5f, pos[2] + 0.5f };
	draw_cube(pos, rotation, zBuffer, 0x996666);
	draw_cube(pos2, rotation, zBuffer, 0x888899);

	for (int i = 0; i < render_state.width; i++)
		delete[] zBuffer[i];
	delete[] zBuffer;
}