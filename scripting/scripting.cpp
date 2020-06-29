#include "../cheat.h"
#include "scripting.h"

using namespace std::chrono_literals;

void panic(sol::optional<std::string> msg) {
    utils->log(str("lua - panic: %s"), msg ? msg->c_str() : str("unknown error"));
}

template <typename ... Args>
void run(unsigned int cbk, Args&... args) {
    for (auto &s : scripting->scripts) {
        if (!s.is_loaded) continue;

        scripting->current_script = &s;
        for (auto fn : s.callbacks[cbk]) {
            auto r = fn(const_cast<const Args&>(args)...);
            if (!r.valid()) {
                sol::error err = r;
                utils->log(str("lua - error: %s"), err.what());

                csgo->engine->client_cmd_unrestricted(str("play buttons/blip1"));
                scripting->unload(s);

                break;
            }
        }
        scripting->current_script = nullptr;
    }
}


bool c_scripting::initialize() {
    base_state = sol::state(sol::c_call<decltype(&panic), &panic>);
    base_state.open_libraries(sol::lib::base, sol::lib::string, sol::lib::package, sol::lib::math);

    base_state[str("collectgarbage")] = sol::lua_nil;
    base_state[str("dofile")] = sol::lua_nil;
    base_state[str("_G")] = sol::lua_nil;
    base_state[str("getfenv")] = sol::lua_nil;
    base_state[str("load")] = sol::lua_nil;
    base_state[str("loadfile")] = sol::lua_nil;
    base_state[str("loadstring")] = sol::lua_nil;
    base_state[str("rawequal")] = sol::lua_nil;
    base_state[str("rawget")] = sol::lua_nil;
    base_state[str("rawset")] = sol::lua_nil;
    base_state[str("setfenv")] = sol::lua_nil;
    base_state[str("string.dump")] = sol::lua_nil;

    globals();
    ns_ui();
    u_painting();
    u_cvar();
    u_client();
    u_entity();
    u_event();

	return true;
}

sol::table c_scripting::protect(sol::table base, std::vector<std::string> &chk) {
    auto prot = base_state.create_table();

    auto mt = prot[sol::metatable_key].get_or_create<sol::table>();
    mt["__index"] = base;
    mt["__newindex"] = [&](sol::table self, sol::object key, sol::object value) -> void {
        auto k = key.as<std::string>();

        for (auto c : chk) {
            if (c == k) {
                base[key] = value;
                return;
            }
        }

        if (!scripting->current_script)
            utils->log(str("lua - access violation: cannot write %s"), key.as<std::string>().c_str());
        else
            utils->log(str("lua - access violation: cannot write %s (script %s.lua)"), key.as<std::string>().c_str(), scripting->current_script->name.c_str());
    };
    mt["__metatable"] = false;

    return prot;
}

void c_scripting::refresh() {
    auto old = scripts;

    scripts.clear();
    for (auto f : std::filesystem::directory_iterator(str("c:/uc-sflying/ext"))) {
        if (!f.is_regular_file())
            continue;

        auto path = f.path();
        if (path.filename().extension() != str(".lua"))
            continue;

        c_script s;
        s.path = path.string();
        s.name = path.filename().replace_extension("").string();

        scripts.push_back(s);
    }

    for (auto &s : scripts) {
        for (auto &o : old) {
            if (s.name == o.name) {
                s.callbacks = o.callbacks;
                s.env = o.env;
                s.is_loaded = o.is_loaded;
                s.menu_elements = o.menu_elements;
            }
        }
    }
}

void c_scripting::prepare(c_script &s) {
    s.env = sol::environment(base_state, sol::create, base_state.globals());
}

void c_scripting::load(c_script &s) {
    if (s.is_loaded)
        return;

    prepare(s);

    current_script = &s;
    auto r = base_state.safe_script_file(s.path, s.env, [](lua_State*, sol::protected_function_result r) {
        return r;
    });
    current_script = nullptr;
    
    if (!r.valid()) {
        sol::error err = r;
        utils->log(str("lua - error: %s"), err.what());

        csgo->engine->client_cmd_unrestricted(str("play buttons/blip1"));
        s.is_loaded = false;
        return;
    }

    csgo->engine->client_cmd_unrestricted(str("play buttons/blip2"));
    s.is_loaded = true;
}

void c_scripting::load(const std::string &s) {
    for (auto &sc : scripts) {
        if (sc.name == s)
            load(sc);
    }
}

void c_scripting::unload(c_script &s) {
    if (!s.is_loaded)
        return;

    s.callbacks.clear();

    for (auto e : s.menu_elements) {
        if (e)
            e->gone = true;
    }

    s.menu_elements.clear();
    s.env.abandon();
    s.is_loaded = false;
}

void c_scripting::unload(const std::string &s) {
    for (auto &sc : scripts) {
        if (sc.name == s)
            unload(sc);
    }
}

void c_scripting::run(ImDrawList *dd) {
    ::run(fnv("paint"), dd);
}

void c_scripting::run(){
    ::run(fnv("create_move"));
}
void c_scripting::player_hurt(c_event* e){
    ::run(fnv("player_hurt"), e);
}
void c_scripting::bullet_impact(c_event* e){
    ::run(fnv("bullet_impact"), e);
}
void c_scripting::player_death(c_event* e){
    ::run(fnv("player_death"), e);
}
void c_scripting::round_prestart(){
    ::run(fnv("round_prestart"));
}
void c_scripting::round_end(c_event* e){
    ::run(fnv("round_end"), e);
}