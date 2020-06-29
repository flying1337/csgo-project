#pragma once

class c_speclist {
public:
	void work();
	void draw();
	void clear() { spectators.clear(); }

	bool did_set_pos = false;
private:
	std::vector<std::string> spectators;
};