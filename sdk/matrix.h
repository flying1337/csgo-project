#pragma once

class c_matrix3x4 {
public:
	float* operator[](int i) { return mtx[i]; }

	float mtx[3][4];
};

class c_matrix4x4 {
public:
	float mtx[4][4] = {0};

	inline float* operator[](int i) { return mtx[i]; }
};