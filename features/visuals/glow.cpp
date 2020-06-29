#include "../../cheat.h"

void c_glow::work() {
    if (!csgo->local_player) return;

	for (auto i = 1; i < csgo->glowmanager->numobjects; i++) {
		auto o = &csgo->glowmanager->objects[i];
		if (!o || o->is_unused() || !o->entity) continue;

		auto ent = o->entity;
		if (ent->is_player()) {
			auto p = (c_player*)ent;

			auto is_self = p->index() == csgo->local_player->index();
			auto is_teammate = p->team() == csgo->local_player->team();

			if (config->i.vis_glow == 1 && !is_teammate)
			    set_glow(o, config->c.glow);
			if (config->i.vis_glow_team == 1 && is_teammate && !is_self)
			    set_glow(o, config->c.glow_team);
            if (config->i.vis_glow_self == 1 && is_self)
                set_glow(o, config->c.glow_self);
			continue;
		}

		if (ent->is_weapon()) {
			if (config->b.vis_droppedglow) {
				auto w = (c_weapon*)ent;
				if (csgo->entities->get_entity_handle(w->owner())) continue;

				set_glow(o, config->c.droppedglow);
			}
			else
				set_glow(o, new float[4] {0, 0, 0, 0});

			continue;
		}

		if (ent->is_projectile() && config->b.vis_projectilesglow) {
			set_glow(o, config->c.projectilesglow);
			continue;
		}
	}
}

void c_glow::set_glow(s_glowobject* o, float col[4]) {
	o->r = col[0];
	o->g = col[1];
	o->b = col[2];
	o->a = col[3];

	o->glow_type = 0;
	o->render_occluded = true;
	o->render_unoccluded = false;
}