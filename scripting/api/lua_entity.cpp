//
// Created by anti on 19. 5. 2020.
//

#include "../../cheat.h"

#define _s base_state

void c_scripting::u_entity(){
    auto e = _s.new_usertype<c_player>(str("entity"), sol::no_constructor);


    e.set_function(str("is_dormant"), [](int index){
        auto p = (c_player*)csgo->entities->get_entity(index);
        if (p)
            return p->is_dormant();
    });
    e.set_function(str("is_enemy"), [](int index){
        auto p = (c_player*)csgo->entities->get_entity(index);
        if (p)
            return p->team() != csgo->local_player->team();
    });

    e.set_function(str("get_players"), [](){
        int players = 0;
        for (auto i = 0; i < 65; i++) {
            auto e = (c_player*)csgo->entities->get_entity(i);
            if (!e || !e->is_player()) continue;
            if (e->flags() & FL_GODMODE) continue;
            if (e->is_immune()) continue;

            players++;
        }

        return players;
    });


    e.set_function(str("health"), [](int index){
        auto p = (c_player*)csgo->entities->get_entity(index);
        if (p)
            return p->health();
    });

    e.set_function(str("get_weapon"), [](int index){
        auto p = (c_player*)csgo->entities->get_entity(index);
        if (p) {
            auto wep = (c_weapon *)csgo->entities->get_entity_handle(p->weapon());
            return wep->econ_index();
        }
    });

    e.set_function(str("get_name"), [](int index){
        auto p = (c_player*)csgo->entities->get_entity(index);
        if (p) {
            return p->get_player_info().name;
        }
    });
    

}
