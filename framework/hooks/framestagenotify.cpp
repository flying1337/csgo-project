//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"
#include "../config.h"

extern std::string hitgroup_to_name(int hg);

void __stdcall hkFrameStageNotify(int stage) {
    if (!core->is_fully_loaded || !csgo->local_player || core->hide_from_obs || !csgo->cmd) {
        oFrameStageNotify(stage);
        return;
    }

    static auto postprocess = csgo->cvar->find_var(str("mat_postprocess_enable"));
    postprocess->set(!config->b.vis_nopostprocess);

    static auto jigglewiggle = csgo->cvar->find_var(str("r_jiggle_bones"));
    jigglewiggle->set(0);

    csgo->local_player->max_flash_alpha() = config->m.removals[0] ? 0.f : 255.f;

    static auto o_agent = csgo->local_player->agent();
    if (config->i.skins_agent)
        csgo->local_player->agent() = config->i.skins_agent;
    else
        csgo->local_player->agent() = o_agent;

    features->effects->smoke();
    features->effects->modulation();
    features->effects->sky();

    if (csgo->local_player->health() <= 0) {
        auto mode = csgo->local_player->spec_mode();
        if (mode != 0) {
            if (mode == 4 && config->b.vis_thirdperson_dead)
                csgo->local_player->spec_mode() = 5;
        }
    }

    if (keymanager->check(config->i.vis_thirdperson, config->i.vis_thirdperson_style) && config->b.vis_thirdperson && csgo->local_player->health() > 0) {
        if (!csgo->is_in_tp) {
            csgo->engine->client_cmd_unrestricted(str("thirdperson"));
            csgo->is_in_tp = true;
        }
    }
    else {
        if (csgo->is_in_tp) {
            csgo->engine->client_cmd_unrestricted(str("firstperson"));
            csgo->is_in_tp = false;
        }
    }

    c_angle view, aim;
    if (config->m.removals[2] && csgo->local_player->health() > 0) {
        view = csgo->local_player->viewpunch();
        aim = csgo->local_player->aimpunch();

        csgo->local_player->viewpunch() = c_angle();
        csgo->local_player->aimpunch() = c_angle();
    }

    if(stage == 2)
    {
        features->misc->clantag();
    }


    if(stage == 2 && csgo->local_player->health() > 0)
    {
        static auto old_flNextCmdTime = 0.0f;
        static auto old_iLastCommandAck = 0;

        auto iLastCommandAck = csgo->clientstate->last_command_ack;
        auto flNextCmdTime = csgo->clientstate->next_cmd_time;

        if(iLastCommandAck != old_iLastCommandAck || old_flNextCmdTime != flNextCmdTime)
        {
            auto velocity_modifier = csgo->local_player->velocity_mod();
            if(csgo->last_vel_mod != velocity_modifier)
            {
                *(bool*)((DWORD)csgo->prediction + 0x24) = true;
                csgo->last_vel_mod = velocity_modifier;
                csgo->last_vel_mod_tick = csgo->clientstate->command_ack;
            }
            old_flNextCmdTime = flNextCmdTime;
            old_iLastCommandAck = iLastCommandAck;
        }
    }


    static auto pp = new float[24]{};
    static auto layers = new c_animlayer[13]{};
    static int tickcount = csgo->globals->tickcount;

    if (stage == 5 && csgo->local_player && csgo->local_player->health() > 0 && csgo->local_player->poseparams() && csgo->local_player->animlayers() && csgo->cmd) {
        features->antiaims->update_body();

        auto as = csgo->local_player->get_anim_state();
        as->feet_rate = 0.f;
        if (tickcount != csgo->globals->tickcount) {
            memcpy(layers, csgo->local_player->animlayers(), sizeof(c_animlayer) * 13);

            csgo->allow_anims = false;
            as->last_anim_frame = 0;

            csgo->local_player->update_anim_state((c_csgo_animstate *) as, csgo->cmd->viewangles);

            if (csgo->last_allow_sp) {
                csgo->proper_abs = as->abs_yaw;

                memcpy(pp, csgo->local_player->poseparams(), sizeof(float) * 24);
            }

            tickcount = csgo->globals->tickcount;
        } else
            as->last_anim_frame = csgo->globals->framecount;

        csgo->allow_anims = true;

        csgo->local_player->set_abs_angles(c_angle(0, csgo->proper_abs, 0));


        memcpy(csgo->local_player->animlayers(), layers, sizeof(c_animlayer) * 13);
        memcpy(csgo->local_player->poseparams(), pp, sizeof(float) * 24);
    }
    if (stage == 3){
        for (auto i = 0; i < csgo->globals->maxclients; i++) {
            auto p = (c_player *) csgo->find_cached(i);
            if(p && p->is_player() && p->index() != csgo->local_player->index())
                features->resolve->work(p);
        }
    }
    static auto sptc = csgo->globals->tickcount;
    if (stage == 4 && csgo->local_player && sptc != csgo->globals->tickcount) {
        if (csgo->local_player->health() > 0) {
            for (auto i = 0; i < 65; i++) {
                auto p = (c_player *) csgo->find_cached(i);
                if (p && p->index() != csgo->local_player->index() && p->team() != csgo->local_player->team() &&
                    p->health() > 0 && p->is_player()) {
                    if (!p->is_dormant())
                        features->lc->populate(p);
                    else
                        features->lc->clear(i);

                    if (config->b.vis_radar)
                        p->spotted() = true;
                } else {
                    features->lc->clear(i);
                    csgo->brute_stage[i] = 0;
                    csgo->baim_stage[i] = 0;
                }
            }
        } else
            features->lc->clear();

        sptc = csgo->globals->tickcount;
    }

    oFrameStageNotify(stage);



    if (config->m.removals[2] && csgo->local_player->health() > 0) {
        csgo->local_player->viewpunch() = view;
        csgo->local_player->aimpunch() = aim;
    }

    if (stage == 5) {
        csgo->allow_anims = true;
    }

    if (stage == 4) {
        if (config->b.vis_bullettracer) {
            for (auto i = 0; i < 256; i++) {
                auto& a = features->bullettracer->data[i];
                if (a.size() > 0) {
                    auto p = (c_player*)csgo->find_cached(i);
                    if (p)
                        features->bullettracer->add(p->origin() + p->view_offset(), a.back());

                    a.clear();
                }
            }
        }

        if (csgo->cmd && csgo->shot.tick == csgo->cmd->tickcount && csgo->shot.player) {
            if (!csgo->shot.hit && !csgo->shot.hurt) {
                if(!config->b.misc_debuglogs) {
                    utils->log(str("Missed shot due to spread"));
                }
                else{
                    utils->log(str("Missed shot due to spread / d: %i / s: %i / b: %i"), csgo->shot.damage, csgo->shot.side, csgo->baim_stage[csgo->shot.player->index()]);
                }

                if (config->b.rage_resolver) {
                    csgo->brute_stage[csgo->shot.player->index()]++;
                }

                if (config->i.rage_baim_after)
                    csgo->baim_stage[csgo->shot.player->index()]++;
            }

            if (csgo->shot.hit && !csgo->shot.hurt && !csgo->shot.player->get_player_info().is_bot) {
                if(!config->b.misc_debuglogs) {
                    utils->log(str("Missed shot due to unknown"));
                }
                else{

                    utils->log(str("Missed shot due to unknown / d: %i / s: %i / b: %i"), csgo->shot.damage, csgo->shot.side, csgo->baim_stage[csgo->shot.player->index()]);
                }
                if (config->b.rage_resolver) {
                    csgo->brute_stage[csgo->shot.player->index()]++;

                }

                if (config->i.rage_baim_after)
                    csgo->baim_stage[csgo->shot.player->index()]++;
            }
            if (csgo->shot.hurt) {
                if (csgo->shot.hitgroup != csgo->shot.hurtgroup && config->b.rage_resolveonwrong && config->b.rage_resolver) {
                    csgo->brute_stage[csgo->shot.player->index()]++;
                }
            }

            csgo->shot.tick = -1;
        }
    }
    core->reset_fsn = false;
}