#include "../cheat.h"
#include "csgo.h"


void c_csgo::cache_entities() {
	auto lp_index = engine->get_local_player();

	c_player* nlp = 0;
	c_tonemap_controller* ntc = 0;
	c_bomb* nb = 0;
	c_gamerules* ng = 0;
	c_playerresources* np = 0;
	c_fogcontroller* nfc = 0;

	for (auto i = 0; i < 4096; i++) {
		auto ent = entities->get_entity(i);
		if (ent) {
			if (ent->is_player()) {
				auto p = (c_player*)ent;
				if (p->index() == lp_index)
					nlp = p;

				continue;
			}

			if (ent->is_weapon()) {
				auto w = (c_weapon*)ent;
				weapon_infos[i] = w->get_info();
				continue;
			}

			auto c = ent->get_client_class();
			if (c) {
				auto id = c->class_id;
				if (id == DT_EnvTonemapController)
					ntc = (c_tonemap_controller*)ent;
				if (id == DT_PlantedC4)
					nb = (c_bomb*)ent;
				if (id == DT_CSGameRulesProxy)
					ng = (c_gamerules*)ent;
				if (id == DT_CSPlayerResource)
					np = (c_playerresources*)ent;
				if (id == DT_FogController)
				    nfc = (c_fogcontroller*)ent;
			}
		}
	}

	if (nlp) local_player = nlp; else local_player = 0;
	if (ntc) tonemap_controller = ntc; else tonemap_controller = 0;
	if (nb) bomb = nb; else bomb = 0;
	if (ng) gamerules = ng; else gamerules = 0;
	if (np) playerresources = np; else playerresources = 0;
	if (nfc) fog_controller = nfc; else fog_controller = 0;
}

c_entity* c_csgo::find_cached(int num) {
	if (num < 0) return 0;
	return entities->get_entity(num);
}

c_entity* c_csgo::find_cached(c_ehandle& const h) {
	if (h.index == 0) return 0;
	return entities->get_entity_handle(h);
}