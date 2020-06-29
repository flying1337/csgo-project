//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"
#include "../config.h"

void __fastcall hkPaintTraverse(void* o, void*, unsigned int a, bool b, bool c) {
    static auto is_connected = false;
    if (!csgo->engine->is_connected() || !csgo->engine->is_in_game()) {
        csgo->local_player = 0;
        csgo->tonemap_controller = 0;
        csgo->bomb = 0;
        csgo->gamerules = 0;
        csgo->playerresources = 0;
        csgo->next_sidemove = 0;
        csgo->next_shift = 0;

        features->speclist->clear();
        features->visuals->clear();
        features->lc->clear();

        features->entlistener->unhook();

        for (auto i = 0; i < 65; i++) {
            csgo->brute_stage[i] = 0;
            csgo->baim_stage[i] = 0;
            csgo->resolver_mode[i] = 0;
        }

        if (is_connected && features->discord->is_initialized) {
            DiscordRichPresence rpc;
            memset(&rpc, 0, sizeof(rpc));
            rpc.details = str("In main menu");

            features->discord->set_activity(rpc);

            is_connected = false;
        }

        static auto cl_interp = csgo->cvar->find_var(str("cl_interp"));
        cl_interp->set(1.f / 64.f);
    }
    else {
        if (!is_connected && csgo->gamerules && features->discord->is_initialized) {
            DiscordRichPresence rpc;
            memset(&rpc, 0, sizeof(rpc));

            if (csgo->gamerules->is_valve_ds())
                rpc.details = str("In casual");
            else if (csgo->gamerules->is_matchmaking())
                rpc.details = str("In matchmaking");
            else
                rpc.details = str("In game");

            features->discord->set_activity(rpc);

            is_connected = true;
        }



        if (csgo->fog_controller && !core->hide_from_obs && csgo->local_player) {
            static auto zoom_sensitivity_ratio_mouse = csgo->cvar->find_var(str("zoom_sensitivity_ratio_mouse"));
            if (config->b.vis_fix_zoom_sensivity)
                zoom_sensitivity_ratio_mouse->set(0);

            auto fc = csgo->fog_controller;
            fc->enable() = !config->b.vis_disablefog;
            fc->color_primary() = ImColor(
                    config->c.fogprimary[0],
                    config->c.fogprimary[1],
                    config->c.fogprimary[2],
                    config->c.fogprimary[3]);
            fc->start() = config->f.vis_fog_start;
            fc->end() = config->f.vis_fog_end;
            fc->maxdensity() = config->f.vis_fog_density;

            csgo->local_player->fog_enabled() = !config->b.vis_disablefog;
            csgo->local_player->fog_color() = ImColor(
                    config->c.fogprimary[0],
                    config->c.fogprimary[1],
                    config->c.fogprimary[2],
                    config->c.fogprimary[3]);
            csgo->local_player->fog_start() = config->f.vis_fog_start;
            csgo->local_player->fog_end() = config->f.vis_fog_end;
            csgo->local_player->fog_maxdensity() = config->f.vis_fog_density;
        }

        if (csgo->tonemap_controller && !core->hide_from_obs) {
            static auto bloom = config->b.vis_bloom;
            static auto bscale = config->f.vis_bloom_scale;

            if (bscale != config->f.vis_bloom_scale || bloom != config->b.vis_bloom || core->reset_bloom) {
                csgo->tonemap_controller->custom_bloom_scale() = config->b.vis_bloom ? config->f.vis_bloom_scale : 0.001f;
                bscale = config->f.vis_bloom_scale;
            }

            if (bloom != config->b.vis_bloom || core->reset_bloom) {
                csgo->tonemap_controller->use_custom_bloom_scale() = true;
                bloom = config->b.vis_bloom;
            }

            static auto ae = config->b.vis_autoexposure;
            static auto ae_min = config->f.vis_ae_min;
            static auto ae_max = config->f.vis_ae_max;

            if (ae_min != config->f.vis_ae_min || ae != config->b.vis_autoexposure || core->reset_bloom) {
                csgo->tonemap_controller->custom_autoexposure_min() = ae ? config->f.vis_ae_min : 1.f;
                ae_min = config->f.vis_ae_min;
            }

            if (ae_max != config->f.vis_ae_min || ae != config->b.vis_autoexposure || core->reset_bloom) {
                csgo->tonemap_controller->custom_autoexposure_max() = ae ? config->f.vis_ae_max : 1.f;
                ae_max = config->f.vis_ae_max;
            }

            if (ae != config->b.vis_autoexposure || core->reset_bloom) {
                csgo->tonemap_controller->use_custom_autoexposure_min() = config->b.vis_autoexposure;
                csgo->tonemap_controller->use_custom_autoexposure_max() = config->b.vis_autoexposure;
                ae = config->b.vis_autoexposure;
            }

            core->reset_bloom = false;
        }
    }

    auto panel_name = g_vfunc(const char*(__thiscall*)(void*, unsigned int), csgo->panel, 36)(csgo->panel, a);
    if (!strstr(panel_name, str("HudZoom")) || core->hide_from_obs)
        oPaintTraverse(o, a, b, c);
    else {
        if (!config->m.removals[3])
            oPaintTraverse(o, a, b, c);
    }
}