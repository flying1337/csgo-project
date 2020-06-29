#include "../cheat.h"

void discord_ready(const DiscordUser* user) {
	features->discord->user = *user;
	features->discord->is_initializing = false;
	features->discord->is_initialized = true;

	core->is_loadanim_shown = false;

	DiscordRichPresence rpc;
	memset(&rpc, 0, sizeof(rpc));
	rpc.details = str("In main menu");

	features->discord->set_activity(rpc);
}

void discord_disconnected(int code, const char* msg) {
	features->discord->is_initializing = false;
	features->discord->is_initialized = false;

	core->is_loadanim_shown = false;

	menu->wnd->get_parent()->add_message(str("Discord"), msg);
}

void discord_errored(int code, const char* msg) {
    menu->wnd->get_parent()->add_message(str("Discord"), msg);
}

void c_discord::set_activity(DiscordRichPresence rpc) {
	if (!is_initialized) return;
	Discord_UpdatePresence(&rpc);
}

void c_discord::clear_activity() {
	if (!is_initialized) return;
	Discord_ClearPresence();
}

void c_discord::initialize() {
	is_initializing = true;

	core->loadanim_text = str("Connecting to Discord...");
	core->is_loadanim_shown = true;

	DiscordEventHandlers h;
	memset(&h, 0, sizeof(DiscordEventHandlers));
	h.ready = discord_ready;
	h.errored = discord_errored;

	Discord_Initialize(str("617370438850445317"), &h, 0, 0);
}

void c_discord::run_callbacks() {
	if (!is_initialized && !is_initializing) return;
	Discord_RunCallbacks();
}

void c_discord::release() {
	is_initializing = true;

	Discord_Shutdown();

	is_initializing = false;
	is_initialized = false;
}