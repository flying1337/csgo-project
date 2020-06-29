#pragma once

class c_script {
public:
    sol::environment env;
    lua_State* state;

    std::string name;
    std::string path;

    std::vector<c_ui_element*> menu_elements;
    std::map<uint32, std::vector<sol::protected_function>> callbacks;

    bool is_loaded = false;
};

class c_scripting {
public:
	bool initialize();

	void refresh();
	void prepare(c_script &s);

	void load(c_script &s);
	void load(const std::string &s);
	void unload(c_script &s);
	void unload(const std::string &s);

	void run(ImDrawList* dd);
	void run();
	///////////////EVENTS////////////////////
	void player_hurt(c_event* e);
    void player_death(c_event* e);
	void bullet_impact(c_event* e);
    void round_prestart();
    void round_end(c_event* e);
    ////////////////////////////////////////

	std::vector<c_script> scripts;
	std::vector<std::string> load_queue;
    sol::state base_state;

    std::vector<std::string> allow_none;

    c_script* current_script = nullptr;

private:
    sol::table protect(sol::table base, std::vector<std::string> &allowed_write);

    void globals();
    void ns_ui();
    void u_painting();
    void u_cvar();
    void u_client();
    void u_entity();
    void u_event();
};