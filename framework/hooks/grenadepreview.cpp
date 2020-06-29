//
// Created by anti on 23. 5. 2020.
//

#include "../../cheat.h"

bool __fastcall grenade_preview_get_bool(void* ecx, void* edx){
    if (config->b.vis_grenade_preview)
        return true;

    //return oGrenadePreview_get_bool(ecx, edx);
}