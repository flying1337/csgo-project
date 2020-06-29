//
// Created by anti on 19. 5. 2020.
//

#include "../../cheat.h"

#define _s base_state

void c_scripting::u_client() {
    auto c = _s.new_usertype<c_engine>(str("client"), sol::no_constructor);


    c.set_function(str("is_ingame"), [](){
        return csgo->engine->is_in_game();
    });

    c.set_function(str("get_fps"), [](){
        return 1.f / csgo->globals->absframetime;
    });

    c.set_function(str("get_ping"), [](){
        return csgo->engine->get_ping();
    });

    c.set_function(str("get_screen_size"), [](int w, int h){
        return csgo->engine->screen_size(w,h);
    });

    c.set_function(str("get_local_player"), [](){
        return csgo->engine->get_local_player();
    });

    c.set_function(str("exec"), [](const std::string &command){
        csgo->engine->client_cmd_unrestricted(command.c_str());
    });

    c.set_function(str("error"), [](const std::string &text) {
        utils->log(str("lua - error: %s"), text.c_str());
    });

    c.set_function(str("set_clantag"), [](const std::string &clantag){
        utils->set_clantag(clantag.c_str());
    });



}