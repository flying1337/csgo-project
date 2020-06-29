//
// Created by ruppet on 25.12.2019.
//

#include "../../cheat.h"
#include "../../framework/config.h"
#define HITGROUP_GENERIC    0
#define HITGROUP_HEAD        1
#define HITGROUP_CHEST        2
#define HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10
s_awinfo c_autowall::work(c_vector start, c_vector end, c_player *from, c_player *to) {
    s_awinfo rt{};

    s_awbullet bullet;
    bullet.start = start;
    bullet.end = end;
    bullet.pos = start;
    bullet.thickness = 0.f;
    bullet.walls = config->i.rage_aw_precision;
    bullet.dir = utils->calc_angle(start, end).vector();

    auto flt_player = c_tracefilter_everything_exactent();
    flt_player.entity = to;

    auto flt_self = c_tracefilter_everything();
    flt_self.skip = from;

    if (to)
        bullet.filter = &flt_player;
    else
        bullet.filter = &flt_self;

    auto wep = (c_weapon*)csgo->entities->get_entity_handle(from->weapon());
    if (!wep)
        return rt;

    auto inf = csgo->weapon_infos[wep->index()];
    if (!inf)
        return rt;

    end = start + bullet.dir * (wep->get_type() == WEAPONTYPE_KNIFE ? 45.f : inf->range);
    bullet.damage = inf->damage;

    while (bullet.damage > 0 && bullet.walls > 0) {
        rt.walls = bullet.walls;

        c_ray ray;
        ray.init(bullet.pos, end);

        c_tracefilter_everything filter;
        filter.skip = from;

        csgo->enginetrace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);
        clip_trace_to_player(bullet.pos, bullet.pos + bullet.dir * 40.f, to, MASK_SHOT | CONTENTS_GRATE, bullet.filter, &bullet.trace);

        bullet.damage *= powf(inf->range_modifier, (bullet.trace.end - start).length() / 500.f);

        if (bullet.trace.fraction == 1.f) {
            rt.damage = bullet.damage;
            rt.hitgroup = -1;
            rt.end = bullet.trace.end;
            rt.player = nullptr;
        }

        if (bullet.trace.hitGroup > 0 && bullet.trace.hitGroup <= 7) {
            if ((to && bullet.trace.ent->index() != to->index()) ||
                    ((c_player*)bullet.trace.ent)->team() == from->team()) {
                rt.damage = -1;

                return rt;
            }

            scale_damage(&bullet.trace, inf, bullet.trace.hitGroup, bullet.damage);

            rt.damage = bullet.damage;
            rt.hitgroup = bullet.trace.hitGroup;
            rt.end = bullet.trace.end;
            rt.player = (c_player*)bullet.trace.ent;

            break;
        }

        if (!this->handle_bullet_penetration(inf, bullet))
            break;

        rt.did_penetrate_wall = true;
    }

    rt.walls = bullet.walls;
    return rt;
}

void c_autowall::clip_trace_to_player(c_vector start, c_vector end, c_player* ent, unsigned int mask, c_tracefilter* filter, c_trace* trace) {
    if (!ent)
        return;

    auto mins = ent->get_collider_offset()->mins;
    auto maxs = ent->get_collider_offset()->maxs;

    c_vector dir(end - start);
    c_vector
            center = (maxs + mins) / 2,
            pos(center + ent->get_abs_origin());

    auto to = pos - start;
    auto range_along = dir.dot_product(to);

    float range;
    if (range_along < 0.f)
        range = -to.length();
    else if (range_along > dir.length())
        range = -(pos - end).length();
    else {
        auto ray(pos - (dir * range_along + start));
        range = ray.length();
    }

    if (range <= 60.f) {
        c_trace newtrace;

        c_ray ray;
        ray.init(start, end);

        csgo->enginetrace->clip_ray_to_collideable(ray, mask, ent->get_collider_offset(), &newtrace);

        if (trace->fraction > newtrace.fraction)
            *trace = newtrace;
    }
}

void c_autowall::scale_damage(c_trace* trace, c_weaponinfo* inf, int& hitgroup, float& damage) {
    auto weapon = features->ragebot->wep;
    bool has_heavy = false;
    int armor_value = ((c_player*)trace->ent)->armor();
    int hit_group = trace->hitGroup;

    if (!csgo->local_player)
        return;

    if (weapon->econ_index() == WEAPON_TASER || weapon->is_projectile()) // || weapon->is_knife() ДОБАВИТЬ!
        return;

    auto is_armored = [&trace]()->bool
    {
        auto* target = (c_player*)trace->ent;
        switch (trace->hitGroup)
        {
            case HITGROUP_HEAD:
                return !!target->has_helmet();
            case HITGROUP_GENERIC:
            case HITGROUP_CHEST:
            case HITGROUP_STOMACH:
            case HITGROUP_LEFTARM:
            case HITGROUP_RIGHTARM:
                return true;
            default:
                return false;
        }
    };

    switch (hit_group)
    {
        case HITGROUP_HEAD:
            damage *= 2.f;
            break;
        case HITGROUP_STOMACH:
            damage *= 1.25f;
            break;
        case HITGROUP_LEFTLEG:
        case HITGROUP_RIGHTLEG:
            damage *= 0.75f;
            break;
        default:
            break;
    }

    if (armor_value > 0 && is_armored())
    {
        float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = inf->armor_ratio / 2.f;

        if (has_heavy)
        {
            armor_bonus_ratio = 0.33f;
            armor_ratio *= 0.5f;
            bonus_value = 0.33f;
        }

        auto new_damage = damage * armor_ratio;

        if (((damage - (damage * armor_ratio)) * (bonus_value * armor_bonus_ratio)) > armor_value)
        {
            new_damage = damage - (armor_value / armor_bonus_ratio);
        }

        damage = new_damage;
    }

}

bool c_autowall::handle_bullet_penetration(c_weaponinfo* inf, s_awbullet& bullet) {
    c_trace trace;

    auto sdata = csgo->physics->get_surface_data(bullet.trace.surface.surface_props);
    auto mat = sdata->game.material;

    auto sp_mod = sdata->game.flPenetrationModifier; //*(float*)((uintptr_t)sdata + 88);
    auto dmg_mod = sdata->game.flDamageModifier;
    auto pnt_mod = 0.f;

    auto solid_surf = (bullet.trace.contents >> 3) & CONTENTS_SOLID;
    auto light_surf = (bullet.trace.surface.flags >> 7) & SURF_LIGHT;

    if (bullet.walls <= 0
        || (!bullet.walls && !light_surf && !solid_surf && mat != CHAR_TEX_GLASS && mat != CHAR_TEX_GRATE)
        || inf->penetration <= 0.f
        || (!this->trace_to_exit(&bullet.trace, bullet.trace.end, bullet.dir, &trace)
           && !(csgo->enginetrace->get_point_contents(bullet.trace.end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL) & (MASK_SHOT_HULL | CONTENTS_HITBOX))))
        return false;

    auto e_sdata = csgo->physics->get_surface_data(trace.surface.surface_props);
    auto e_mat = e_sdata->game.material;
    auto e_sp_mod = e_sdata->game.flPenetrationModifier;

    if (mat == CHAR_TEX_GRATE || mat == CHAR_TEX_GLASS)
    {
        pnt_mod = 3.f;
        dmg_mod = 0.05f;
    }
    else if (light_surf || solid_surf)
    {
        pnt_mod = 1.f;
        dmg_mod = 0.16f;
    }
    else if (mat == CHAR_TEX_FLESH)
    {
        pnt_mod = 1.f;
        dmg_mod = 0.16f;
    }
    else
    {
        pnt_mod = (sp_mod + e_sp_mod) / 2.f;
        dmg_mod = 0.16f;
    }

    if (mat == e_mat)
    {
        if (e_mat == CHAR_TEX_CARDBOARD || e_mat == CHAR_TEX_WOOD)
            pnt_mod = 3.f;
        else if (e_mat == CHAR_TEX_PLASTIC)
            pnt_mod = 2.f;
    }

    auto thickness = (trace.end - bullet.trace.end).length_sqr();
    auto modifier = fmaxf(0.f, 1.f / pnt_mod);

    auto lost_damage = fmaxf(
            ((modifier * thickness) / 24.f)
            + ((bullet.damage * dmg_mod)
               + (fmaxf(3.75f / inf->penetration, 0.f) * 3.f * modifier)), 0.f);

    if (lost_damage > bullet.damage)
        return false;

    if (lost_damage > 0.f)
        bullet.damage -= lost_damage;

    if (bullet.damage < 1.f)
        return false;

    bullet.pos = trace.end;
    bullet.walls--;

    return true;
}

bool c_autowall::trace_to_exit(c_trace* enter_trace, c_vector& start, c_vector& dir, c_trace* exit_trace) {
    c_vector end;
    auto distance = 0.f;
    auto distance_check = 23;
    auto first_contents = 0;

    do {
        distance += 4.f;
        end = start + dir * distance;

        if (!first_contents)
            first_contents = csgo->enginetrace->get_point_contents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

        auto point_contents = csgo->enginetrace->get_point_contents(end, MASK_SHOT | CONTENTS_GRATE, NULL);
        if (!(point_contents & (MASK_SHOT_HULL /*| CONTENTS_HITBOX*/)) || (point_contents & CONTENTS_HITBOX && point_contents != first_contents)) {
            auto new_end = end - (dir * 4.f);

            c_ray ray;
            ray.init(end, new_end);

            csgo->enginetrace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

            if (exit_trace->startSolid && exit_trace->surface.flags & SURF_HITBOX) {
                c_ray ray1;
                ray1.init(start, end);

                c_tracefilter_everything filter;
                filter.skip = exit_trace->ent;

                csgo->enginetrace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, exit_trace);

                if (exit_trace->did_hit() && !exit_trace->startSolid)
                    return true;

                continue;
            }

            if (exit_trace->did_hit() && !exit_trace->startSolid) {
                if (enter_trace->surface.flags & SURF_NODRAW || !(exit_trace->surface.flags & SURF_NODRAW)) {
                    if (exit_trace->plane.normal.dot_product(dir) <= 1.f)
                        return true;

                    continue;
                }

                if (this->is_breakable((c_player*)enter_trace->ent) && this->is_breakable((c_player*)exit_trace->ent))
                    return true;

                continue;
            }

            if (exit_trace->surface.flags & SURF_NODRAW) {
                if (this->is_breakable((c_player*)enter_trace->ent) && this->is_breakable((c_player*)exit_trace->ent))
                    return true;
                else if (!(enter_trace->surface.flags & SURF_NODRAW))
                    continue;
            }

            if ((!enter_trace->ent || enter_trace->ent->index() == 0) && this->is_breakable((c_player*)enter_trace->ent)) {
                exit_trace = enter_trace;
                exit_trace->end = start + dir;
                return true;
            }

            continue;
        }

        distance_check--;
    } while (distance_check);

    return false;
}

bool c_autowall::is_breakable(c_player* e) {
    using func = bool(__fastcall*)(c_player*);
    static auto fn = reinterpret_cast<func>(utils->scan(str("client_panorama.dll"), str("55 8B EC 51 56 8B F1 85 F6 74 68 83 BE")));

    if (!e || !e->index())
        return false;

    auto take_damage{ (char*)((uintptr_t)e + *(size_t*)((uintptr_t)fn + 38)) };
    auto take_damage_backup{ *take_damage };

    auto cclass = csgo->hl->get_all_classes();

    if ((cclass->name[1]) != 'F'
        || (cclass->name[4]) != 'c'
        || (cclass->name[5]) != 'B'
        || (cclass->name[9]) != 'h')
        * take_damage = 2;

    auto breakable = fn(e);
    *take_damage = take_damage_backup;

    return breakable;
}

float c_autowall::get_point_damage(c_vector point, c_player* e) {
    return this->work(csgo->local_player->origin() + csgo->local_player->view_offset(), point, csgo->local_player, e).damage;
}

float c_autowall::get_approx_point_damage(c_vector point) {
    if (!csgo->local_player)
        return -1.f;

    s_awbullet bullet;
    auto filter = c_tracefilter_everything();
    filter.skip = csgo->local_player;

    bullet.filter = &filter;
    bullet.start = csgo->local_player->origin() + csgo->local_player->view_offset();
    bullet.end = point;
    bullet.pos = csgo->local_player->origin() + csgo->local_player->view_offset();

    bullet.dir = utils->calc_angle(bullet.start, point).vector();
    bullet.trace.start = bullet.start;
    bullet.trace.end = point;

    auto wep = (c_weapon*)csgo->entities->get_entity_handle(csgo->local_player->weapon());
    if (!wep)
        return -2.f;

    bullet.walls = 1;
    bullet.thickness = 0.f;

    auto inf = csgo->weapon_infos[wep->index()];
    if (!inf)
        return -3.f;

    bullet.damage = inf->damage;

    c_ray ray;
    ray.init(bullet.start, bullet.end);

    csgo->enginetrace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);

    if (bullet.trace.fraction == 1.f)
        return -4.f;

    if (this->handle_bullet_penetration(inf, bullet))
        return bullet.damage;

    return -5.f;
}