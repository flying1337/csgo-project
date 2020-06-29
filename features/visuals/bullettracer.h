#pragma once

class c_bullettracer {
public:
	void add(c_vector a, c_vector b);
	void sound_add(c_player* p);

	std::vector<c_vector> data[256] = {  };
};