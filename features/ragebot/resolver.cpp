
#include "../../cheat.h"
void c_resolver::work(c_player* p) {

    if (!p || !p->health() > 0 || p->is_dormant())
        return;
    auto as = p->get_anim_state();
    if (config->b.rage_resolver && !p->get_player_info().is_bot && as) {
        //needs to be improved/tested


        switch (csgo->brute_stage[p->index()] % 2) {
            case 0:
                as->abs_yaw += 120.f;
                side[p->index()] = 1;
                break;
            case 1:
                as->abs_yaw -= 120.f;
                side[p->index()] = 2;
                break;
        }




    }
}
