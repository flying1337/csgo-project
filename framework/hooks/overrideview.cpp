//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"
#include "../config.h"

void __stdcall hkOverrideView(c_viewsetup* v) {

    oOverrideView(v);

    if (csgo->engine->is_connected() && csgo->engine->is_in_game() && csgo->local_player) {
        static auto unscopedfov = 90.f;
        static auto defaultfov = 90.f;

        if (csgo->local_player->health() > 0) {
            if (csgo->local_player->is_scoped()) {
                v->fov += std::clamp(config->i.misc_overridefov, 0, 60);
                v->fov += (unscopedfov - v->fov) * (1.f - config->i.misc_zoom_fov / 100.f);
            } else {
                defaultfov = v->fov;
                v->fov += config->i.misc_overridefov;
                unscopedfov = v->fov;
            }
        } else {
            if (csgo->local_player->spec_mode() == 5)
                v->fov = defaultfov + config->i.misc_overridefov;
            else {
                auto spec = (c_player*)csgo->entities->get_entity_handle(csgo->local_player->spectator());
                if (spec) {
                    if (spec->is_scoped()) {
                        v->fov += std::clamp(config->i.misc_overridefov, 0, 60);
                        v->fov += (unscopedfov - v->fov) * (1.f - config->i.misc_zoom_fov / 100.f);
                    } else
                        v->fov += config->i.misc_overridefov;
                } else
                    v->fov += config->i.misc_overridefov;
            }
        }

        static auto zms = csgo->cvar->find_var(str("zoom_sensitivity_ratio_mouse"));
        zms->set(config->i.misc_zoom_fov / 100.f);

        if (keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style)) {
            if (!csgo->is_in_tp)
                v->origin.z = csgo->local_player->origin().z + 64.f;
        }

        if (csgo->is_in_tp) {
            for (auto i = 0; i < 65; i++)
                csgo->goes_thru[i] = false;

            c_trace tr;
            c_vector cam_forward, cam_right, cam_up;

            auto origin = csgo->local_player->get_abs_origin();

            utils->angle_vectors(v->angles, cam_forward, cam_right, cam_up);
            auto end_point = c_vector(origin.x, origin.y, origin.z +
            (keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style) ? 64.f : csgo->local_player->view_offset().z));
            auto cam_offset = end_point + (cam_forward * -config->i.vis_thirdperson_dist);

            c_tracefilter_world filter;
            filter.skip = csgo->local_player;

            c_ray ray;
            ray.init(end_point, cam_offset, c_vector(-14.0f, -14.0f, -14.0f), c_vector(14.0f, 14.0f, 14.0f));

            csgo->enginetrace->trace_ray(ray, MASK_SOLID & ~CONTENTS_MONSTER, &filter, &tr);
            {
                c_tracefilter_everything filter;
                filter.skip = csgo->local_player;

                c_ray ray;
                ray.init(tr.start, tr.end);

                c_trace tr;
                csgo->enginetrace->trace_ray(ray, MASK_SOLID | CONTENTS_GRATE, &filter, &tr);

                if (tr.did_hit() && tr.ent && tr.ent->is_player()) {
                    auto p = (c_player*)tr.ent;

                    if (p->team() == csgo->local_player->team())
                        csgo->goes_thru[p->index()] = true;
                }
            }

            v->origin = tr.end;
        }
    }
}