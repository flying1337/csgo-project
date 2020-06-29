//
// Created by anti on 19. 5. 2020.
//

#include "../../cheat.h"

#define _s base_state

void c_scripting::u_cvar() {
    auto h = _s.new_usertype<c_cvar>(str("cvar"), sol::no_constructor);

    h.set_function(str("set_int"),[](const std::string &cvar_name, int val){
        csgo->cvar->find_var(cvar_name.c_str())->set(val);
    });

    h.set_function(str("get_int"),[](const std::string &cvar_name){
        return csgo->cvar->find_var(cvar_name.c_str())->get_int();
    });

    h.set_function(str("set_float"),[](const std::string &cvar_name, float val){
        csgo->cvar->find_var(cvar_name.c_str())->set(val);
    });

    h.set_function(str("get_float"),[](const std::string &cvar_name){
        return csgo->cvar->find_var(cvar_name.c_str())->get_float();
    });

    h.set_function(str("set_string"),[](const std::string &cvar_name, std::string val){
        csgo->cvar->find_var(cvar_name.c_str())->set(val.c_str());
    });

    h.set_function(str("get_string"),[](const std::string &cvar_name){
        return csgo->cvar->find_var(cvar_name.c_str())->get_string();
    });

}