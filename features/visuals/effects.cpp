#include "../../cheat.h"

template<typename T>
bool arr_comp(T* a, T* b, unsigned int size) {
	auto cmp = true;
	for (auto i = 0; i < size; i++) {
		if (a[i] != b[i])
			cmp = false;
	}
	return cmp;
}

void c_effects::smoke() {
	static auto smoke_count = *(int*)(utils->scan(str("client_panorama.dll"), str("55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0")) + 8);

	static std::vector<std::string> smoke_materials =
	{
		str("particle/beam_smoke_01"),
		str("particle/particle_smokegrenade"),
		str("particle/particle_smokegrenade1"),
		str("particle/particle_smokegrenade2"),
		str("particle/particle_smokegrenade3"),
		str("particle/particle_smokegrenade_sc"),
		str("particle/smoke1/smoke1"),
		str("particle/smoke1/smoke1_ash"),
		str("particle/smoke1/smoke1_nearcull"),
		str("particle/smoke1/smoke1_nearcull2"),
		str("particle/smoke1/smoke1_snow"),
		str("particle/smokesprites_0001"),
		str("particle/smokestack"),
		str("particle/vistasmokev1/vistasmokev1"),
		str("particle/vistasmokev1/vistasmokev1_emods"),
		str("particle/vistasmokev1/vistasmokev1_emods_impactdust"),
		str("particle/vistasmokev1/vistasmokev1_fire"),
		str("particle/vistasmokev1/vistasmokev1_nearcull"),
		str("particle/vistasmokev1/vistasmokev1_nearcull_fog"),
		str("particle/vistasmokev1/vistasmokev1_nearcull_nodepth"),
		str("particle/vistasmokev1/vistasmokev1_smokegrenade"),
		str("particle/vistasmokev1/vistasmokev4_emods_nocull"),
		str("particle/vistasmokev1/vistasmokev4_nearcull"),
		str("particle/vistasmokev1/vistasmokev4_nocull")
	};

	static auto o_smoke = config->m.removals[1];
	if (o_smoke != config->m.removals[1] || core->reset_fsn) {
		for (auto mat : smoke_materials) {
			auto m = csgo->materialsystem->find_material(mat.c_str(), str("Other textures"));
			m->set_flag(MATERIAL_VAR_NO_DRAW, config->m.removals[1]);
		}

		o_smoke = config->m.removals[1];
	}

	if (config->m.removals[1])
		*(int*)smoke_count = 0;
}

void c_effects::modulation() {
	static char val[3];

	static bool world_mod = config->b.vis_worldmod;
	static bool prop_mod = config->b.vis_worldmod;

	static float world[4] = { 0 };
	static float prop[4] = { 0 };

	static bool _init = false;
	if (!_init) {
		memcpy(world, config->c.world, sizeof(float) * 4);
		memcpy(prop, config->c.prop, sizeof(float) * 4);

		strcpy(val, str("0"));

		_init = true;
	}

	static auto v = csgo->cvar->find_var(str("r_drawspecificstaticprop"));
	v->set(0);

	if (world_mod != config->b.vis_worldmod || prop_mod != config->b.vis_propmod || 
		!arr_comp(world, config->c.world, 4) || !arr_comp(prop, config->c.prop, 4) || core->reset_fsn) {

		for (
			auto i = csgo->materialsystem->first_material();
			i != csgo->materialsystem->invalid_material();
			i = csgo->materialsystem->next_material(i)) {
			auto material = csgo->materialsystem->get_material(i);

			if (strstr(material->get_texture_group_name(), str("World"))) {
				if (config->b.vis_worldmod) {
					auto wcol = config->c.world;
					material->color_modulate(wcol[0], wcol[1], wcol[2]);
					material->alpha_modulate(wcol[3]);
				}
				else {
					material->color_modulate(1.f, 1.f, 1.f);
					material->alpha_modulate(1.f);
				}
			}

			if (strstr(material->get_texture_group_name(), str("StaticProp"))) {
				if (config->b.vis_propmod) {
					auto pcol = config->c.prop;
					material->color_modulate(pcol[0], pcol[1], pcol[2]);
					material->alpha_modulate(pcol[3]);
				}
				else {
					material->color_modulate(1.f, 1.f, 1.f);
					material->alpha_modulate(1.f);
				}
			}
		}

		world_mod = config->b.vis_worldmod;
		prop_mod = config->b.vis_propmod;

		memcpy(world, config->c.world, sizeof(float) * 4);
		memcpy(prop, config->c.prop, sizeof(float) * 4);
	}
}

void c_effects::sky() {
	using fnLoadNamedSky = void(__fastcall*)(const char*);
	static auto load_named_sky = (fnLoadNamedSky)utils->scan(str("engine.dll"), str("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
	static auto sv_skyname = csgo->cvar->find_var(str("sv_skyname"));

	static std::vector<std::string> skies = {
		str("sky_csgo_night02")
	};

	static auto sky = config->i.vis_sky;
	if (sky != config->i.vis_sky || core->reset_fsn) {
		if (config->i.vis_sky == 0)
			load_named_sky(sv_skyname->default_value);
		else
			load_named_sky(skies.at(config->i.vis_sky - 1).c_str());

		sky = config->i.vis_sky;
	}
}