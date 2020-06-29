//
// Created by ruppet on 23.12.2019.
//

#include "../../cheat.h"
#include "../../framework/config.h"
#include "ragebot.h"
#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define M_PI 3.14159265358979323846

void rotate_movement(c_usercmd* cmd, float yaw)
{
    c_angle viewangles;
    csgo->engine->get_view_angles(&viewangles);

    float rotation = DEG2RAD(viewangles.y - yaw);

    float cos_rot = cos(rotation);
    float sin_rot = sin(rotation);

    float new_forwardmove = cos_rot * cmd->forwardmove - sin_rot * cmd->sidemove;
    float new_sidemove = sin_rot * cmd->forwardmove + cos_rot * cmd->sidemove;

    cmd->forwardmove = new_forwardmove;
    cmd->sidemove = new_sidemove;
}
void c_ragebot::work(c_usercmd* c) {
    csgo->aimpoints.clear();

    having_revolver = false;
    if (!config->b.rage_enable || !csgo->local_player || !csgo->engine->is_connected() || !csgo->engine->is_in_game() ||
        !keymanager->check(config->i.rage_key, config->i.rage_key_style) || delay_shot) return;

    wep = (c_weapon*)csgo->entities->get_entity_handle(csgo->local_player->weapon());
    if (!wep) return;

    wep->update_spread();

    auto type = wep->get_type();
    auto idx = wep->econ_index();

    if (type == WEAPONTYPE_GRENADE)
        return;
    if (type == WEAPONTYPE_KNIFE) {
        return;
    }
    if (type == WEAPONTYPE_PISTOL) {
        if (!config->b.rage_aim_weapon[0] && !config->b.rage_aim_weapon[1]) return; else if (config->b.rage_aim_weapon[1]) wep_cfg = 1;
        if (!config->b.rage_aim_weapon[1]) use_def = true; else use_def = false; }
    if (type == WEAPONTYPE_HEAVYPISTOL) {
        if (!config->b.rage_aim_weapon[0] && !config->b.rage_aim_weapon[2]) return; else if (config->b.rage_aim_weapon[2]) wep_cfg = 2;
        if (!config->b.rage_aim_weapon[2]) use_def = true; else use_def = false;}
    if (type == WEAPONTYPE_SUBMACHINEGUN) {
        if (!config->b.rage_aim_weapon[0] && !config->b.rage_aim_weapon[3]) return; else if (config->b.rage_aim_weapon[3]) wep_cfg = 3;
        if (!config->b.rage_aim_weapon[3]) use_def = true; else use_def = false;}
    if (type == WEAPONTYPE_SHOTGUN || type == WEAPONTYPE_MACHINEGUN) {
        if (!config->b.rage_aim_weapon[0] && !config->b.rage_aim_weapon[4]) return; else if (config->b.rage_aim_weapon[4]) wep_cfg = 4;
        if (!config->b.rage_aim_weapon[4]) use_def = true; else use_def = false;}
    if (type == WEAPONTYPE_RIFLE) {
        if (!config->b.rage_aim_weapon[0] && !config->b.rage_aim_weapon[5]) return; else if (config->b.rage_aim_weapon[5]) wep_cfg = 5;
        if (!config->b.rage_aim_weapon[5]) use_def = true; else use_def = false;}
    if (type == WEAPONTYPE_SNIPER_RIFLE) {
        if (!config->b.rage_aim_weapon[0] && !config->b.rage_aim_weapon[6]) return; else if (config->b.rage_aim_weapon[6]) wep_cfg = 6;
        if (!config->b.rage_aim_weapon[6]) use_def = true; else use_def = false;}
    if (type == WEAPONTYPE_BOLTACTION) {
        if (idx == WEAPON_SSG08) {
            if (!config->b.rage_aim_weapon[0] && !config->b.rage_aim_weapon[7]) return; else if (config->b.rage_aim_weapon[7]) wep_cfg = 7;
            if (!config->b.rage_aim_weapon[7]) use_def = true; else use_def = false;
        }
        if (idx == WEAPON_AWP) {
            if (!config->b.rage_aim_weapon[0] && !config->b.rage_aim_weapon[8]) return; else if (config->b.rage_aim_weapon[8]) wep_cfg = 8;
            if (!config->b.rage_aim_weapon[8]) use_def = true; else use_def = false;
        }
    }

    if (idx == WEAPON_REVOLVER && wep->clip() > 0) {
        having_revolver = true;
        fire_time = wep->fire_time();
    }

    cmd = c;
    auto players = get_players();

    if (config->i.rage_sort != 0) {
        std::vector<c_player*> sorted;
        c_player* best = 0;
        float best_fov = 360.f;

        c_angle va;
        csgo->engine->get_view_angles(&va);

        for (auto& p : players) {
            if (!best)
                best = p;
            else {
                if (config->i.rage_sort == 1) {
                    if (best->health() > p->health())
                        best = p;
                }

                if (config->i.rage_sort == 2) {
                    if (best->origin().dist(csgo->local_player->origin()) > p->origin().dist(csgo->local_player->origin()))
                        best = p;
                }

                if (config->i.rage_sort == 3) {
                    auto fov = utils->calc_fov(va, utils->calc_angle(p->origin(), csgo->local_player->origin()));
                    if (best_fov > fov && fov <= (config->i.rage_fov * 2)) {
                        best_fov = fov;
                        best = p;
                    }
                }
            }
        }

        for (auto& p : players) {
            if (p != best)
                sorted.push_back(p);
        }

        if (best)
            sorted.push_back(best);

        players = sorted;
    }

    c_angle va;
    csgo->engine->get_view_angles(&va);

    s_bullet best_bullet{};

    if (config->i.rage_behaviour == 0 ) {
        for (auto p : players) {
            auto bi = scan(p);
            if (bi.hittable && bi.damage > best_bullet.damage)
                best_bullet = bi;
        }
    }

    if ((config->i.rage_behaviour == 1 || csgo->mitigate_fps) && players.size() > 0) {
        auto bi = scan(players.at(c->tickcount % players.size()));
        if (bi.hittable)
            best_bullet = bi;
    }

    if (best_bullet.hittable)
        aim(best_bullet);

    if (c->buttons & IN_ATTACK && config->b.rage_norecoil)
        c->viewangles -= csgo->local_player->aimpunch() * 2.f;
}

void c_ragebot::aim(s_bullet bi) {
    auto ang = utils->calc_angle(csgo->local_player->origin() + csgo->local_player->view_offset(), bi.point);

    bool can_shoot = is_able_to_shoot();
    bool can_shoot2 = false;
    auto good_chance = hitchance(ang, bi.player, bi);

    if(features->future->ready)
        can_shoot2 = true;
    else
        can_shoot2 = is_able_to_shoot();

    if (can_shoot2) {
        auto sc_enabled = false;
        if (use_def) sc_enabled = config->b.rage_autoscope[0];
        else sc_enabled = config->b.rage_autoscope[wep_cfg];

        if (sc_enabled && !csgo->local_player->is_scoped() && (use_def ? false : (wep_cfg == 6 || wep_cfg == 7 || wep_cfg == 8))
            && !features->autojump->is_hopping)
            cmd->buttons |= IN_ATTACK2;
    }




    if (!config->b.rage_movebetween[wep_cfg])
    {
        csgo->cmd->forwardmove = 0.f;
        csgo->cmd->sidemove = 0.f;
        rotate_movement(csgo->cmd, 180.f);
    }
    else if (can_shoot)
    {
        auto as_enabled = false;
        if (use_def) as_enabled = config->b.rage_quickstop[0];
        else as_enabled = config->b.rage_quickstop[wep_cfg];
        if (as_enabled && wep->econ_index() != WEAPON_TASER) {
            auto movetype = csgo->local_player->movetype();
            if (movetype != MOVETYPE_LADDER && movetype != MOVETYPE_NOCLIP &&
                (csgo->local_player->flags() & FL_ONGROUND)) {


                if (csgo->local_player->velocity().length_2d() > 15.f) {
                    csgo->cmd->forwardmove = csgo->local_player->velocity().length_2d() > 15.0f ? 450.f : 0.0f;
                    csgo->cmd->sidemove = 0;
                    rotate_movement(csgo->cmd,utils->calc_angle(c_vector(0.0f, 0.0f, 0.0f), csgo->local_player->velocity()).y + 180.0f);
                }


            }
        }
    }

    if (!good_chance){
            return;
    }

    if (((use_def && config->b.rage_autofire[0]) || (!use_def && config->b.rage_autofire[wep_cfg])) && can_shoot) {
        if (config->b.aa_fl_disableshooting)
            features->fakelag->max_choke = 1;

        cmd->buttons |= IN_ATTACK;
    }

    if (config->b.rage_overrideview && can_shoot &&
        ((((use_def && config->b.rage_autofire[0]) || (!use_def && config->b.rage_autofire[wep_cfg])) && (cmd->buttons & IN_ATTACK)) ||
         !((use_def && config->b.rage_autofire[0]) || (!use_def && config->b.rage_autofire[wep_cfg])))) {
        csgo->engine->set_view_angles(&ang);
    }

    if (cmd->buttons & IN_ATTACK) {
        last_shot.point = bi.point;
        last_shot.player = bi.player;
        last_shot.hitgroup = bi.hitgroup;
        last_shot.damage = bi.damage;
        last_shot.abspos = bi.player->get_abs_origin();
        last_shot.absyaw = bi.player->get_abs_angles();
        last_shot.hc = true;
        last_shot.side = features->resolve->side[bi.player->index()];



        if(config->b.vis_impacts)
        csgo->debugoverlay->add_box_overlay(last_shot.point, c_vector(-2, -2, -2), c_vector(2, 2, 2),
                                            c_vector(0, 0, 0), 220, 0, 0, 125, 4.f);

        last_shot.shotpos = csgo->local_player->origin() + csgo->local_player->view_offset();
        last_shot.hitbox = bi.hitbox;

        memcpy(last_shot.bones, bi.bones, sizeof(c_matrix3x4) * 128);

        cmd->tickcount = utils->time_to_ticks(bi.lc.simtime + features->lc->get_lerptime());
        cmd->viewangles = ang;
        csgo->aimpoint = bi.point;

        csgo->real = ang;

        did_shot = true;
        wants_to_shoot = false;
    }

    if (features->fakelag->max_choke == 1 && !keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style)) {
        csgo->choke = 2;
        csgo->want_sp = csgo->choke >= features->fakelag->max_choke;
    }
}

bool c_ragebot::is_able_to_shoot() {
    if (!wep) return false;
    if (wep->econ_index() == WEAPON_C4) return true;
    if (wep->clip() == 0) return false;

    if (csgo->shift && config->b.rage_rapidfire && keymanager->check(config->i.rage_rapidfire_key, config->i.rage_rapidfire_keystyle))
    {

        auto server_time = csgo->local_player->calc_servertime();

        if (csgo->shift)
            server_time -= utils->ticks_to_time(csgo->shift) - 1;

        if (server_time < wep->next_attack()) //-V807
            return false;

        if (server_time < csgo->local_player->nextattack())
            return false;

        if (having_revolver && utils->time_to_ticks(wep->fire_time() - csgo->local_player->tickbase() * csgo->globals->tickinterval) > 4) return false;
    }
    else
    {
        auto server_time = csgo->local_player->calc_servertime();

        if (server_time <  wep->next_attack())
            return false;

        if (server_time < csgo->local_player->nextattack())
            return false;

        //if (having_revolver && utils->time_to_ticks(wep->fire_time() - csgo->local_player->tickbase() * csgo->globals->tickinterval) > 4) return false;
    }

    return true;
}

bool c_ragebot::hitchance(c_angle aimangle, c_player* p, s_bullet bullet) {
    if (use_def) {
        if (!config->i.rage_aim_hitchance[0]) return true;
    } else {
        if (!config->i.rage_aim_hitchance[wep_cfg]) return true;
    }

    auto hc = std::clamp(float(use_def ? config->i.rage_aim_hitchance[0] : config->i.rage_aim_hitchance[wep_cfg]), 1.f, 100.f);

    c_vector fw, rw, uw;
    utils->angle_vectors(aimangle, fw, rw, uw);

    int precision = 450;
    if (csgo->mitigate_fps)
        precision = 200;

    int hits = 0;
    int total = int(precision * (hc / 100.f));

    wep->update_spread();

    float cone = wep->get_spread();
    float inacc = wep->get_inaccuracy();

    auto src = csgo->local_player->origin() + csgo->local_player->view_offset();

    memcpy(csgo->scan_bones, bullet.lc.bones, sizeof(c_matrix3x4) * 128);

    auto o_count = p->cached_bones_count();
    auto o_mtx = *p->cached_bones();

    auto mins = p->get_collider_offset()->mins;
    auto maxs = p->get_collider_offset()->maxs;

    p->cached_bones_count() = 128;
    *p->cached_bones() = csgo->scan_bones;

    auto babs = p->get_abs_origin();
    p->set_abs_origin(bullet.lc.abspos);

    auto bang = p->get_abs_angles();
    p->set_abs_angles(c_angle(0, bullet.lc.absyaw, 0));

    p->get_collider_offset()->mins = bullet.lc.mins;
    p->get_collider_offset()->maxs = bullet.lc.maxs;

    auto result = false;
    for (int i = 0; i < precision; i++) {
        float a = utils->random(0.f, 1.f);
        float b = utils->random(0.f, 3.1415f * 2.f);
        float c = utils->random(0.f, 1.f);
        float d = utils->random(0.f, 3.1415f * 2.f);
        float inaccuracy = a * inacc;
        float spread = c * cone;

        if (wep->econ_index() == WEAPON_REVOLVER) {
            if (cmd->buttons & IN_ATTACK2) {
                a = 1.f - a * a;
                c = 1.f - c * c;
            }
        }

        c_vector spread_view((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0);
        c_vector direction;

        direction.x = fw.x + (spread_view.x * rw.x) + (spread_view.y * uw.x);
        direction.y = fw.y + (spread_view.x * rw.y) + (spread_view.y * uw.y);
        direction.z = fw.z + (spread_view.x * rw.z) + (spread_view.y * uw.z);

        c_angle viewangles_spread;
        c_vector view_forward;

        utils->vector_angles(direction, uw, viewangles_spread);
        viewangles_spread.normalize();

        view_forward = viewangles_spread.vector();
        view_forward = src + (view_forward * csgo->weapon_infos[wep->index()]->range);

        c_trace tr;
        c_ray ray;

        ray.init(src, view_forward);
        csgo->enginetrace->clip_ray_to_entity(ray, MASK_SHOT | CONTENTS_GRATE, p, &tr);

        if (tr.ent) {
            if (tr.ent->index() == p->index())
                hits++;
        }

        if ((float(hits) / precision) * 100.f >= hc) {
            result = true;
            break;
        }

        if ((precision - i + hits) < total)
            break;
    }

    p->cached_bones_count() = o_count;
    *p->cached_bones() = o_mtx;

    p->set_abs_origin(babs);
    p->set_abs_angles(bang);

    p->get_collider_offset()->mins = mins;
    p->get_collider_offset()->maxs = maxs;

    return result;
}

s_bullet c_ragebot::scan(c_player* p) { //WORK ON THIS
    auto v = features->lc->get(p);
    if (v.size() <= 0) return {};

    std::vector<c_lag_record> sc;
    sc.push_back(v.back());

    if (v.size() > 1 && config->b.rage_backtrack) {
        sc.push_back(v.front());
        sc.back().is_backtrack = true;
    }

    s_bullet best_bullet{};
    for (auto rec : sc) {
        if (!rec.allow_attack) return {};

        memcpy(csgo->scan_bones, rec.bones, sizeof(c_matrix3x4) * 128);

        std::vector<s_bullet> bullets{};

        auto avoidlimbs = false;
        if ((config->b.rage_avoidlimbs_move && rec.velocity.length() > 4.02f) ||
            (config->b.rage_avoidlimbs_jump && !(rec.flags & FL_ONGROUND)))
            avoidlimbs = true;

        auto forcebody = false;
        if (config->i.rage_baim_after && csgo->baim_stage[p->index()] >= config->i.rage_baim_after)
            forcebody = true;
        if (wep->econ_index() == WEAPON_TASER)
            forcebody = true;

        auto wc = use_def ? 0 : wep_cfg;
        if (config->m.rage_aim_hitboxes[wc][4] ||
            keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) || forcebody) {
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_STOMACH, csgo->scan_bones), false, p, -1, 0, HITBOX_STOMACH, true});

            if (!rec.is_backtrack || (rec.is_backtrack && config->b.rage_btmps))
                generate_multipoints(bullets, HITBOX_STOMACH, csgo->scan_bones, p);
        }
        if ((config->m.rage_aim_hitboxes[wc][5] ||
            keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) || forcebody) && !rec.is_backtrack) {
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_PELVIS, csgo->scan_bones), false, p, -1, 0, HITBOX_PELVIS, true});
            generate_multipoints(bullets, HITBOX_PELVIS, csgo->scan_bones, p);
        }
        if (config->m.rage_aim_hitboxes[wc][0] &&
            !keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) && !forcebody) {
            bullets.push_back({p->get_hitbox_pos(HITBOX_HEAD, csgo->scan_bones), false, p, -1, 1, HITBOX_HEAD, true});

            if (!rec.is_backtrack || (rec.is_backtrack && config->b.rage_btmps))
                generate_multipoints(bullets, HITBOX_HEAD, csgo->scan_bones, p);
        }
        if (config->m.rage_aim_hitboxes[wc][1] &&
            !keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) && !forcebody && !csgo->mitigate_fps && !rec.is_backtrack) {
            bullets.push_back({p->get_hitbox_pos(HITBOX_NECK, csgo->scan_bones), false, p, -1, 1, HITBOX_NECK, true});
        }
        if (config->m.rage_aim_hitboxes[wc][2] &&
            !keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) && !forcebody && !csgo->mitigate_fps && !rec.is_backtrack) {
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_UPPER_CHEST, csgo->scan_bones), false, p, -1, 2, HITBOX_UPPER_CHEST,
                     true});

            if (!rec.is_backtrack || (rec.is_backtrack && config->b.rage_btmps))
                generate_multipoints(bullets, HITBOX_UPPER_CHEST, csgo->scan_bones, p);
        }
        if (config->m.rage_aim_hitboxes[wc][3] &&
            !keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) && !forcebody) {
            bullets.push_back({p->get_hitbox_pos(HITBOX_CHEST, csgo->scan_bones), false, p, -1, 2, HITBOX_CHEST, true});

            if (!rec.is_backtrack || (rec.is_backtrack && config->b.rage_btmps))
                generate_multipoints(bullets, HITBOX_CHEST, csgo->scan_bones, p);
        }
        if (config->m.rage_aim_hitboxes[wc][6] &&
            !keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) && !avoidlimbs && !forcebody && !csgo->mitigate_fps && !rec.is_backtrack) {
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_LEFT_HAND, csgo->scan_bones), false, p, -1, 4, HITBOX_LEFT_HAND, true});
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_RIGHT_HAND, csgo->scan_bones), false, p, -1, 5, HITBOX_RIGHT_HAND, true});
        }
        if (config->m.rage_aim_hitboxes[wc][7] &&
            !keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) && !avoidlimbs && !forcebody && !rec.is_backtrack) {
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_LEFT_CALF, csgo->scan_bones), false, p, -1, 6, HITBOX_LEFT_CALF, true});
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_RIGHT_CALF, csgo->scan_bones), false, p, -1, 7, HITBOX_RIGHT_CALF, true});
        }
        if (config->m.rage_aim_hitboxes[wc][8] &&
            !keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style) && !avoidlimbs && !forcebody && !csgo->mitigate_fps) {
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_LEFT_FOOT, csgo->scan_bones), false, p, -1, 6, HITBOX_LEFT_FOOT, true});
            bullets.push_back(
                    {p->get_hitbox_pos(HITBOX_RIGHT_FOOT, csgo->scan_bones), false, p, -1, 7, HITBOX_RIGHT_FOOT, true});
        }

        // scan loop

        auto o_count = p->cached_bones_count();
        auto o_mtx = *p->cached_bones();

        auto mins = p->get_collider_offset()->mins;
        auto maxs = p->get_collider_offset()->maxs;

        p->cached_bones_count() = 128;
        *p->cached_bones() = csgo->scan_bones;

        auto babs = p->get_abs_origin();
        p->set_abs_origin(rec.abspos);

        auto bang = p->get_abs_angles();
        p->set_abs_angles(c_angle(0, rec.absyaw, 0));

        p->get_collider_offset()->mins = rec.mins;
        p->get_collider_offset()->maxs = rec.maxs;

        for (auto &b : bullets) {
            if (config->b.rage_aimpoints) {
                csgo->aimpoints.push_back({b.point});
            }

            b.damage = features->autowall->get_point_damage(b.point, p);
            if (b.damage <= 0)
                continue;

            c_ray ray;
            ray.init(csgo->local_player->origin() + csgo->local_player->view_offset(), b.point);

            c_tracefilter_everything filter;
            filter.skip = csgo->local_player;

            c_trace tr;
            csgo->enginetrace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

            if (tr.fraction > 0.97f)
                b.is_visible = true;
        }

        p->cached_bones_count() = o_count;
        *p->cached_bones() = o_mtx;

        p->set_abs_origin(babs);
        p->set_abs_angles(bang);

        p->get_collider_offset()->mins = mins;
        p->get_collider_offset()->maxs = maxs;

        auto vdmg = 0;
        auto admg = 0;

        if (use_def) {
            if(config->i.rage_aw_precision < 1)
                admg = 99999;
            else
                admg = config->i.rage_aim_awmindamage[0];
            vdmg = config->i.rage_aim_mindamage[0];
        } else {
            if(config->i.rage_aw_precision < 1)
                admg = 99999;
            else
                admg = config->i.rage_aim_awmindamage[wep_cfg];
            vdmg = config->i.rage_aim_mindamage[wep_cfg];
        }

        if (vdmg == 0)
            vdmg = max(csgo->weapon_infos[wep->index()]->damage / 2, p->health() / 2);
        if (vdmg == 101)
            vdmg = p->health() + 1;

        if (admg == 0)
            admg = min(csgo->weapon_infos[wep->index()]->damage / 3, p->health() / 4);
        if (admg == 101)
            admg = p->health() + 1;

        if (wep->econ_index() == WEAPON_TASER) {
            vdmg = 100;
            admg = 100;
        }

        auto pref = features->future->ready ? 2 : preference_to_hitgroup(use_def ? config->i.rage_preference[0] : config->i.rage_preference[wep_cfg]);


        // select loop
        for (auto &b : bullets) {
            b.hittable = b.is_visible ? (b.damage >= vdmg) : (b.damage >= admg);
            if (best_bullet.hitgroup == b.hitgroup && best_bullet.hittable && best_bullet.is_center) continue;

            auto can_baim = (
                    use_def ? config->b.ragebot_baim_when_lethal[0] : config->b.ragebot_baim_when_lethal[wep_cfg] && ( ( b.hitbox == HITBOX_CHEST || b.hitbox == HITBOX_LOWER_CHEST ||
                                                              b.hitbox == HITBOX_STOMACH || b.hitbox == HITBOX_PELVIS ) && best_bullet.damage >= p->health() )
            );
            if (can_baim && b.hittable){
                best_bullet = b;
                break;
            }
            if (b.hitgroup == 0 && b.hittable && best_bullet.damage >= p->health()) {
                best_bullet = b;
                break;
            }


            if (pref >= 0 && b.hitgroup == pref && b.hittable) {
                best_bullet = b;
                break;
            }

            if (b.damage > best_bullet.damage && b.hittable)
                best_bullet = b;
        }

        best_bullet.tick = rec.tick;
        best_bullet.lc = rec;
        memcpy(best_bullet.bones, csgo->scan_bones, sizeof(c_matrix3x4) * 128);

        if (best_bullet.hittable)
            break;
    }

    return best_bullet;
}

int c_ragebot::preference_to_hitgroup(int pref) {
    if (pref == 0) return -1;
    if (pref == 1) return 1;
    if (pref == 2) return 2;
    if (pref == 3) return 0;
    return -1;
}

void c_ragebot::generate_multipoints(std::vector<s_bullet>& points, int hitbox, c_matrix3x4 bones[128], c_player* p) {
    auto mdl = csgo->modelinfo->get_studio_model(p->get_renderable()->get_model());
    if (!mdl)
        return;

    auto set = mdl->get_hitbox_set(0);
    auto hbx = set->get_hitbox(hitbox);

    if (!hbx)
        return;

    c_vector mins, maxs;
    utils->vector_transform(hbx->bbmin, bones[hbx->bone], mins);
    utils->vector_transform(hbx->bbmax, bones[hbx->bone], maxs);

    auto center = (mins + maxs) * 0.5f;
    auto forward = utils->calc_angle(center, csgo->local_player->origin() + csgo->local_player->view_offset()).vector();

    auto right = forward.cross(c_vector(0, 0, 1));
    auto left = c_vector(-right.x, -right.y, right.z);
    auto top = c_vector(0, 0, 1);
    auto bottom = c_vector(0, 0, -1);

    auto wc = use_def ? 0 : wep_cfg;

    float adjusted_radius = 0.f;
    switch (hitbox) {
        case HITBOX_HEAD:
            adjusted_radius = hbx->radius * float(config->i.rage_pointscale_head[wc] / 125.f);

            if (adjusted_radius > 0.f) {
                points.push_back({ center + top * adjusted_radius, false, p, -1, 1, HITBOX_HEAD });
                points.push_back({ center + right * adjusted_radius, false, p, -1, 1, HITBOX_HEAD });
                points.push_back({ center + left * adjusted_radius, false, p, -1, 1, HITBOX_HEAD });
            }
            break;

        case HITBOX_CHEST:
            adjusted_radius = hbx->radius * float(config->i.rage_pointscale_body[wc] / 125.f);

            if (adjusted_radius > 0.f) {
                points.push_back({ center + right * adjusted_radius, false, p, -1, 2, HITBOX_CHEST });
                points.push_back({ center + left * adjusted_radius, false, p, -1, 2, HITBOX_CHEST });
            }
            break;

        case HITBOX_UPPER_CHEST:
            adjusted_radius = hbx->radius * float(config->i.rage_pointscale_body[wc] / 125.f);

            if (adjusted_radius > 0.f) {
                points.push_back({ center + right * adjusted_radius, false, p, -1, 2, HITBOX_UPPER_CHEST });
                points.push_back({ center + left * adjusted_radius, false, p, -1, 2, HITBOX_UPPER_CHEST });
            }
            break;

        case HITBOX_STOMACH:
            adjusted_radius = hbx->radius * float(config->i.rage_pointscale_body[wc] / 125.f);

            if (adjusted_radius > 0.f) {
                points.push_back({ center + right * adjusted_radius, false, p, -1, 0, HITBOX_STOMACH });
                points.push_back({ center + left * adjusted_radius, false, p, -1, 0, HITBOX_STOMACH });
            }
            break;

        case HITBOX_PELVIS:
            adjusted_radius = hbx->radius * float(config->i.rage_pointscale_body[wc] / 125.f);

            if (adjusted_radius > 0.f) {
                points.push_back({ center + right * adjusted_radius, false, p, -1, 0, HITBOX_PELVIS });
                points.push_back({ center + left * adjusted_radius, false, p, -1, 0, HITBOX_PELVIS });
            }
            break;
    }
}

std::vector<c_player*> c_ragebot::get_players() {
    std::vector<c_player*> players;
    for (auto i = 0; i < 65; i++) {
        auto e = (c_player*)csgo->entities->get_entity(i);
        if (!e || !e->is_player() || e->is_dormant() || e->health() <= 0) continue;
        if (e->index() == csgo->local_player->index()) continue;
        if (e->team() == csgo->local_player->team()) continue;
        if (e->flags() & FL_GODMODE) continue;
        if (e->is_immune()) continue;

        players.push_back(e);
    }

    return players;
}

void c_ragebot::cock() {
   // HWND hWnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
   // HWND activeWindow = GetForegroundWindow();
    static auto r8cock_flag = true;
    static auto r8cock_time = 0.0f;
    if (features->ragebot->having_revolver && config->b.rage_autorevolver && !(cmd->buttons & IN_ATTACK)) {
        r8cock_flag = true;

        if (r8cock_flag && is_able_to_shoot())
        {
            if (r8cock_time <= utils->ticks_to_time(csgo->local_player->tickbase()))
            {
                if (wep->next_secondary_attack() <= utils->ticks_to_time(csgo->local_player->tickbase()))
                    r8cock_time = utils->ticks_to_time(csgo->local_player->tickbase()) + 0.234375f; // 0.234375f
                else
                    cmd->buttons |= IN_ATTACK2;
            }
            else
                cmd->buttons |= IN_ATTACK;

            r8cock_flag = utils->ticks_to_time(csgo->local_player->tickbase()) > r8cock_time;

        } else{
            r8cock_flag = false;
            r8cock_time = utils->ticks_to_time(csgo->local_player->tickbase()) + 0.234375f;
            cmd->buttons &= ~IN_ATTACK;
        }
    }
}