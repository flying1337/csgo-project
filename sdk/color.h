#pragma once

class c_color {
public:
	char r = 0, g = 0, b = 0, a = 255;

	c_color() {}
	c_color(int r, int g, int b, int a = 255) { this->r = r; this->g = g; this->b = b; this->a = a; }

	bool operator==(const c_color& o) { return *(unsigned int*)(&o.r) == *(unsigned int*)(&r); }
};