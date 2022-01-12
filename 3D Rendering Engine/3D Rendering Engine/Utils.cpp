#include <cmath>

#define global_variable static
#define internal static

float const pi = 3.141592653589793238;

inline float
clamp(float min, float val, float max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

inline float
min3(float x1, float x2, float x3) {
	if (x1 < x2 && x1 < x3)
		return x1;
	if (x2 < x1 && x2 < x3)
		return x2;
	else
		return x3;
}

inline float
max3(float x1, float x2, float x3) {
	if (x1 > x2 && x1 > x3)
		return x1;
	if (x2 > x1 && x2 > x3)
		return x2;
	else
		return x3;
}

float avg3(float x1, float y1, float x2, float y2, float x3, float y3, float z1, float z2, float z3, float x, float y) {
	float d1 = sqrt(pow(x - x1, 2) + pow((y - y1), 2));
	float d2 = sqrt(pow(x - x2, 2) + pow((y - y2), 2));
	float d3 = sqrt(pow(x - x3, 2) + pow((y - y3), 2));
	float td = d1 + d2 + d3;
	float z = ((d1 / td) * z1 + (d2 / td) * z2 + (d3 / td) * z3) / 3;
	return z;
}

inline float
abso(float x) {
	if (x < 0)
		x = -x;
	return x;
}

float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return abso((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
	float A = area(x1, y1, x2, y2, x3, y3);
	float A1 = area(x, y, x2, y2, x3, y3);
	float A2 = area(x1, y1, x, y, x3, y3);
	float A3 = area(x1, y1, x2, y2, x, y);

	return (A == A1 + A2 + A3);
}

bool isClockwise(float x1, float y1, float x2, float y2, float x3, float y3) {
	float p1 = (x2 - x1) * (y2 + y1);
	float p2 = (x3 - x2) * (y3 + y2);
	float p3 = (x1 - x3) * (y1 + y3);
	return (p1 + p2 + p3) > 0;
}

float * rotate_about_point(float cx, float cy, float angle, float px, float py)
{
	static float r[2];

	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	px -= cx;
	py -= cy;

	// rotate point
	float xnew = px * c - py * s;
	float ynew = px * s + py * c;

	// translate point back:
	px = xnew + cx;
	py = ynew + cy;
	r[0] = px;
	r[1] = py;
	return r;
}

float* rotate_point(float px, float py, float angle)
{
	static float r[2];

	r[0] = px * cos(angle) - py * sin(angle);
	r[1] = px * sin(angle) + py * cos(angle);

	return r;
}

float mag(float px, float py, float pz)
{
	return(sqrt(px * px + py * py + pz * pz));
}

float* norm(float px, float py, float pz)
{
	float m = mag(px, py, pz);
	float r[3] = { 0, 0, 0 };
	r[0] = px * m;
	r[1] = py * m;
	r[2] = pz * m;
	return r;
}