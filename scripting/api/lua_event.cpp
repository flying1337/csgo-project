//
// Created by anti on 19. 5. 2020.
//
#include "../../cheat.h"

#define _s base_state


void c_scripting::u_event(){
    auto ev = _s.new_usertype<c_event>(str("event"), sol::no_constructor);


    ev.set_function(str("get_int"),[](c_event* self, const std::string &ev_name){
        return self->get_int(ev_name.c_str());
    });

    ev.set_function(str("get_float"),[](c_event* self, const std::string &ev_name){
        return self->get_float(ev_name.c_str());
    });

    ev.set_function(str("get_string"),[](c_event* self, const std::string &ev_name){
        return self->get_string(ev_name.c_str());
    });
    ev.set_function(str("get_bool"),[](c_event* self, const std::string &ev_name){
        return self->get_bool(ev_name.c_str());
    });


}
