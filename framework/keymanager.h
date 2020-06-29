#pragma once

class c_keymanager {
public:
	bool is_key_down(int key);
	bool is_key_up(int key);
	bool is_key_toggled(int key);

	bool check(int key, int style);

private:
    bool prev[256] = {false};
};