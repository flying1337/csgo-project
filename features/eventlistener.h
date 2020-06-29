#pragma once

class c_eventlistener : public c_baseeventlistener {
public:
	void initialize();
	void release();

	void	fire_game_event(c_event* event);
	int		get_event_debug_id() { return 42; }
};