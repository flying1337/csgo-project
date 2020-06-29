//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

void __fastcall hkDrawModelExecute(void* a, void*, void* b, const c_drawmodelstate& c, c_modelrenderinfo& render_info, c_matrix3x4* e) {
    if (!core->is_fully_loaded || !csgo->local_player || csgo->modelrender->is_forced_material_override() || core->hide_from_obs) {
        oDrawModelExecute(a, b, c, render_info, e);
        return;
    }

    auto ent = csgo->entities->get_entity(render_info.entity_index);
    if (ent && ent->is_player()) {
        if (!features->coloredmodels->work(ent, a, b, c, render_info, e)) {
            if (csgo->goes_thru[ent->index()])
                csgo->renderview->set_blend(0.25f);

            oDrawModelExecute(a, b, c, render_info, e);

            if (csgo->goes_thru[ent->index()])
                csgo->renderview->set_blend(1.f);
        }
    } else
        oDrawModelExecute(a, b, c, render_info, e);
}