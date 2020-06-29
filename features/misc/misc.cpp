//
// Created by anti on 18. 5. 2020.
//

#include "../../cheat.h"
#include "misc.h"


void c_misc::clantag(){

    static bool did_run_last = false;

    if(!csgo->local_player)
        return;

    if (!config->b.misc_clantag){

        if (did_run_last){
            did_run_last = false;
            utils->set_clantag(str("") );
        }
        return;
    }

    static float last_change = 0.f;

    const std::string org_name = str ( " uc-sflying   " );
    static std::string name = org_name;

        if (csgo->globals->realtime - last_change > 0.45f) {
            last_change = csgo->globals->realtime;
            std::rotate(name.begin(), name.begin() + 1, name.end());
            utils->set_clantag(name.data());
        }


    did_run_last = true;

}