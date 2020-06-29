#pragma once

class c_autojump {
public:
	void work(c_usercmd* c);

	bool is_hopping = false;

private:
	void autojump(c_usercmd* c);
	void airstrafe(c_usercmd* m_pcmd);
};