#pragma once
#define cfg_bool(n, d) bool n = d
#define cfg_int(n, d) int n = d
#define cfg_float(n, d) float n = d
#define cfg_color(n, r, g, b, a) float n[4] = {r, g, b, a}
#define cfg_map(n) bool n[16] = {false}
#define cfg_bool_arr(n, s, d) bool n[s] = {d}
#define cfg_int_arr(n, s, d) int n[s] = {d}
#define cfg_float_arr(n, s, d) float n[s] = {d}
#define cfg_color_arr(n, s, r, g, b, a) float n[s][4] = {{r, g, b, a}}
#define cfg_map_arr(n, s) bool n[s][16] = {{false}}

class c_config_paintkit {
public:
    short item_id;
    char team;
    int paintkit_id;
    bool is_stattrack;
    int stattrack_amt;
    float wear;
    char nametag[128];
};

class c_config {
public:
	struct {
		bool _ = false;

		cfg_bool(show_desc, true); // 0
		cfg_bool(anti_untrusted, true);
		cfg_bool(disable_loader, false);
		cfg_bool(vis_esp, false);
		cfg_bool(vis_teammates, false);
		cfg_bool(vis_dormant, false); // 5
		cfg_bool(vis_boundingbox, false);
		cfg_bool(vis_healthbar, false);
		cfg_bool(vis_name, false);
		cfg_bool(vis_offscreen, false);
		cfg_bool(vis_ammo, false); // 10
		cfg_bool(vis_hitmarker, false);
		cfg_bool(vis_hitsound, false);
		cfg_bool(vis_noanim, false);
		cfg_bool(vis_glow, false);
		cfg_bool(vis_cm_players, false); // 15
		cfg_bool(vis_cm_players_xqz, false);
		cfg_bool(vis_cm_team, false);
		cfg_bool(vis_bloom, false);
		cfg_bool(vis_autoexposure, false);
		cfg_bool(vis_killeffect, false); // 20
		cfg_bool(vis_nopostprocess, false);

		cfg_bool(vis_worldmod, false);
		cfg_bool(vis_propmod, false); // 25
		cfg_bool(vis_impacts, false);
		cfg_bool(vis_bullettracer, false);
		cfg_bool(vis_thirdperson, false);
		cfg_bool(vis_thirdperson_dead, false);
		cfg_bool(vis_speclist, false);
		cfg_bool(vis_bombtimer, false);
		cfg_bool(vis_noocclusion, false);
		cfg_bool(vis_droppedesp, false);
		cfg_bool(vis_droppedam, false);
		cfg_bool(vis_droppeddist, false);
		cfg_bool(vis_droppedglow, false);
		cfg_bool(vis_projectilesglow, false);
		cfg_bool(vis_projectiles, false); // 40
		cfg_bool(misc_autojump, false);
		cfg_bool(misc_airstrafe, false);
		cfg_bool(misc_infduck, false);
		cfg_bool(misc_wasdmove, false); // 45
		cfg_bool(rage_enable, false);
		cfg_bool_arr(rage_aim_weapon, 9, false);
		cfg_bool(aa_fix_legs, false); // 55
		cfg_bool(vis_penecross, false);
		cfg_bool(rage_norecoil, false);
		cfg_bool_arr(rage_autofire, 9, false); // 58
		cfg_bool(aa_fl_disableshooting, false); // 67
		cfg_bool_arr(aa_desync, 3, false); // 68
        cfg_bool(vis_cm_fake, false); // 71
        cfg_bool(aa_manual, false);
        cfg_bool(aa_manual_indicator, false);
        cfg_bool(rage_autorevolver, false);
        cfg_bool_arr(rage_quickstop, 9, false); // 75
        cfg_bool_arr(rage_autoscope, 9, false); // 84
        cfg_bool(vis_cm_nolagpos, false); // 93
        cfg_bool(vis_cm_self, false);
        cfg_bool(rage_resolver, false); // 95
        cfg_bool(misc_svpurebypass, false);
        cfg_bool(aa_disableonfreeze, false);
        cfg_bool(vis_cm_fake_overlayed, false);
        cfg_bool(rage_preferbaim, false);
        cfg_bool(aa_sway, false); // 100
        cfg_bool(rage_overrideview, false);
        cfg_bool(rage_rapidfire, false);
        cfg_bool(vis_pipiska, false);
        cfg_bool(rage_delayshot, false);
        cfg_bool(vis_disablefog, false);
        cfg_bool(vis_hostages, false);
        cfg_bool(vis_radar, false);
        cfg_bool(rage_avoidlimbs_move, false);
        cfg_bool(rage_avoidlimbs_jump, false);
        cfg_bool(rage_aimpoints, false);
        cfg_bool(rage_aimpoints_scopeonly, false);
        cfg_bool(rage_silent, false);
        cfg_bool(rage_resolveonwrong, false);
        cfg_bool(vis_hitlog, false);

        cfg_bool(vis_cm_backtrack, false);
        cfg_bool(rage_backtrack, false);
        cfg_bool(rage_btmps, false);
        cfg_bool(lb_enable, false);
        cfg_bool(lb_flash, false);
        cfg_bool(lb_smoke, false);
        cfg_bool(lb_nochams, false);
        cfg_bool_arr(lb_aim_enable, 7, false);
        cfg_bool(rage_extrapolation, false);
        cfg_bool(vis_fix_zoom_sensivity, false);
        cfg_bool_arr(rage_movebetween, 9,false);
        cfg_bool(misc_clantag, false);
        cfg_bool(misc_debuglogs, false);
        cfg_bool_arr(ragebot_baim_when_lethal, 9, false);
        cfg_bool(vis_sound, false);
        cfg_bool(vis_grenade_preview, false);
        cfg_bool(low_fps_warning, false);
	} b;

	struct {
		int _ = 0;

		cfg_int(menukey, VK_INSERT); // 0
		cfg_int(vis_distance, 100);
		cfg_int(vis_offscreen_size, 30);
		cfg_int(vis_offscreen_offset, 200);
		cfg_int(vis_activeweapon, 0); // 5
		cfg_int(vis_cm_type, 0);
		cfg_int(vis_cm_team_type, 0);
		cfg_int(vis_sky, 0);
		cfg_int(vis_tracerwidth, 3);
		cfg_int(vis_tracerlife, 2);
		cfg_int(vis_thirdperson, 0);
		cfg_int(vis_thirdperson_style, 0); // 15
		cfg_int(vis_speclist_x, 25);
		cfg_int(vis_speclist_y, 700);
		cfg_int(vis_droppedaw, 0);
		cfg_int(vis_droppeddd, 15); // 20
		cfg_int(vis_projectilesna, 0);
		cfg_int(misc_overridefov, 0);
		cfg_int(misc_aspectratio, 0);
		cfg_int(misc_aspectratiomd, 0);
		cfg_int(rage_weapon, 0); // 25
		cfg_int(aa_fakelag, 0);
		cfg_int_arr(aa_fakelag_mode, 3, 0);
		cfg_int_arr(aa_fakelag_style, 3, 0); // 30
		cfg_int_arr(aa_fakelag_limit, 3, 1);
		cfg_int_arr(aa_fakelag_var, 3, 0);
        cfg_int(aa, 0); // 36
        cfg_int_arr(aa_pitch, 3, 0);
        cfg_int_arr(aa_yaw, 3, 0); // 40
        cfg_int_arr(aa_yaw_offset, 3, 0);
        cfg_int(rage_aw_precision, 4); // 44
        cfg_int_arr(rage_aim_mindamage, 9, 0); // 54
        cfg_int_arr(rage_aim_awmindamage, 9, 0); // 63
        cfg_int(rage_aim_body, 0); // 72
        cfg_int(rage_aim_body_style, 1);
        cfg_int(aa_slowmo, 0);
        cfg_int(aa_slowmo_style, 1); // 75
        cfg_int(aa_slowmo_speed, 100);
        cfg_int(vis_cm_fake_type, 0);
        cfg_int(misc_viewmodelfov, 0);
        cfg_int(aa_manual_positive, 0);
        cfg_int(aa_manual_negative, 0); // 80
        cfg_int(aa_manual_opposite, 0);
        cfg_int(aa_invert, 0);
        cfg_int(aa_invert_style, 0);
        cfg_int(aa_fakeduck, 0);
        cfg_int(aa_fakeduck_style, 1); // 85
        cfg_int_arr(rage_pointscale_head, 9, 1); // 86
        cfg_int_arr(rage_pointscale_body, 9, 1); // 95
        cfg_int_arr(rage_pointscale_legs, 9, 1); // 104
        cfg_int(vis_cm_self_type, 0); // 113
        cfg_int(vis_glow, 0);
        cfg_int(vis_glow_team, 0); // 115
        cfg_int(vis_glow_self, 0);
        cfg_int(skins_weapon, 0);
        cfg_int_arr(skins_weapon_paintkit, 560, 0);
        cfg_int(rage_behaviour, 0); // 677
        cfg_int(rage_maxmisses, 0);
        cfg_int(aa_minwalkspeed, 0);
        cfg_int_arr(rage_preference, 9, 0);
        cfg_int(rage_sort, 0);
        cfg_int(aa_desync_type, 0);
        cfg_int(aa_slowmo_type, 0);
        cfg_int(vis_thirdperson_dist, 150);
        cfg_int(vis_fog_blend, 0);
        cfg_int(rage_baim_after, 0);
        cfg_int(rage_key, 0);
        cfg_int(rage_key_style, 0);
        cfg_int(rage_fov, 360);
        cfg_int(misc_zoom_fov, 100);
        cfg_int(skins_agent, 0);
        cfg_int(rage_rapidfire_key, 0);
        cfg_int(rage_rapidfire_keystyle, 0);
        cfg_int(vis_cm_backtrack, 0);
        cfg_int(lb_aim_weapon, 0);
        cfg_int(lb_trg_weapon, 0);
        cfg_int_arr(lb_aim_key, 7, 0);
        cfg_int_arr(lb_aim_keystyle, 7, 1);
        cfg_int_arr(rage_aim_hitchance, 9, 0); // 45
        cfg_int(rage_rapidfire_speed, 1);
	} i;

	struct {
		float _ = 0;

		cfg_float(vis_bloom_scale, 0.01f); // 0
		cfg_float(vis_ae_min, 1.f);
		cfg_float(vis_ae_max, 1.f);
		cfg_float(vis_kf_length, 1.f);
		cfg_float(aa_sidemove_speed, 1.5f); // 5
		cfg_float(vis_fog_start, 0.f);
		cfg_float(vis_fog_end, 16384.f);
		cfg_float(vis_fog_density, 0.45f);
        cfg_float_arr(rage_aim_hitchance, 9, 0.f); // 45

    } f;

	struct {
		float _[4] = { 0, 0, 0, 0 };

		cfg_color(menu, 0.937f, 0.035f, 0.373f, 1.f); // 0
		cfg_color(darkbg, 0.05f, 0.05f, 0.05f, 1.f);
		cfg_color(lightbg, 0.07f, 0.07f, 0.07f, 1.f);
		cfg_color(shadow, 0.03f, 0.03f, 0.03f, 1.f);
		cfg_color(outline, 0.1f, 0.1f, 0.1f, 1.f);
		cfg_color(boundingbox, 1.f, 1.f, 1.f, 0.5f); // 5
		cfg_color(name, 1.f, 1.f, 1.f, 1.f);
		cfg_color(offscreen, 1.f, 1.f, 1.f, 1.f);
		cfg_color(weapon, 1.f, 1.f, 1.f, 1.f);
		cfg_color(ammo, 1.f, 1.f, 1.f, 1.f);
		cfg_color(glow, 1.f, 0.f, 0.667f, 0.5f); // 10
		cfg_color(cm_players, 0.678f, 0.956f, 0.019f, 1.f);
		cfg_color(cm_players_xqz, 0.f, 0.667f, 1.f, 1.f);
		cfg_color(cm_team, 1.f, 1.f, 1.f, 1.f);
		cfg_color(world, 1.f, 1.f, 1.f, 1.f);
		cfg_color(prop, 1.f, 1.f, 1.f, 1.f); // 15
		cfg_color(tracer, 1.f, 1.f, 1.f, 1.f);
		cfg_color(droppedaw, 1.f, 1.f, 1.f, 1.f);
		cfg_color(droppedam, 1.f, 1.f, 1.f, 1.f);
		cfg_color(droppedglow, 1.f, 1.f, 1.f, 0.5f); // 20
		cfg_color(projectilesglow, 1.f, 1.f, 1.f, 0.5f);
		cfg_color(projectilesna, 1.f, 1.f, 1.f, 1.f);
        cfg_color(fake, 1.f, 1.f, 1.f, 0.5f);
        cfg_color(self, 1.f, 1.f, 1.f, 1.f); // 25
        cfg_color(glow_team, 1.f, 1.f, 1.f, 0.5f);
        cfg_color(glow_self, 1.f, 1.f, 1.f, 0.5f);
        cfg_color(fogprimary, 1.f, 1.f, 1.f, 1.f);
        cfg_color(fogsecondary, 0.5f, 0.5f, 0.5f, 1.f);
        cfg_color(hostages, 1.f, 1.f, 1.f, 1.f);
        cfg_color(aimpoints, 1.f, 1.f, 1.f, 1.f);
        cfg_color(cm_backtrack, 1.f, 1.f, 1.f, 1.f);
        cfg_color(damagecolor, 1.f, 1.f, 1.f, 1.f);
        cfg_color(soundcolor, 1.f, 1.f, 1.f, 1.f);
	} c;

	struct {
		bool _[16] = { false };

		cfg_map(vis_info);
		cfg_map_arr(rage_aim_hitboxes, 9);
		cfg_map(removals);
		cfg_map(events);
	} m;

    std::unordered_map<int, c_config_paintkit> paintkits;
	std::vector<std::string> slots;
	int selected_slot = 0;

	const int cfg_version = 1;

	void init();
	void save();
	void load();
	void reset();
	void refresh();

	void save_skins();
	void load_skins();
	void reset_skins();

	bool is_anything_selected(bool smth[16]) { for (auto k = 0; k < 16; k++) if (smth[k]) return true; return false; }

private:
	void* o_b = 0;
	void* o_i = 0;
	void* o_f = 0;
	void* o_c = 0;
	void* o_m = 0;
};