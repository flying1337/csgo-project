#pragma once

class c_discord {
public:
	void initialize();
	void release();

	void run_callbacks();

	void set_activity(DiscordRichPresence rpc);
	void clear_activity();

	bool is_initializing = false;
	bool is_initialized = false;

	DiscordUser user;
private:
	
};