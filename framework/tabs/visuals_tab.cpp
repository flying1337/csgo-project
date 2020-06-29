//
// Created by ruppet on 3/14/2020.
//

#include "../../cheat.h"

void c_menu::visuals_tab(c_ui_tab* visuals) {
    static auto vis_players = new c_ui_group;
    vis_players->setup(str("Players"));
    vis_players->set_size(ImVec2(240.f, 205.f));
    {
        static auto enable = new c_ui_checkbox;
        enable->setup(str("Enable"), &config->b.vis_esp);

        static auto noanim = new c_ui_checkbox;
        noanim->setup(str("Disable animations"), &config->b.vis_noanim);
        noanim->set_desc(str("Disables ESP animations such as health descrease, dormancy in/out, etc"));

        static auto showteam = new c_ui_checkbox;
        showteam->setup(str("Draw teammates"), &config->b.vis_teammates);

        static auto dormant = new c_ui_checkbox;
        dormant->setup(str("Dormant"), &config->b.vis_dormant);
        dormant->set_desc(str("Draw ESP even if game does not receive any info about player"));

        static auto boundingbox = new c_ui_checkbox;
        boundingbox->setup(str("Bounding box"), &config->b.vis_boundingbox);

        static auto boundingbox_color = new c_ui_colorpicker;
        boundingbox_color->setup(str("Bounding box color"), config->c.boundingbox);
        boundingbox_color->set_draw_name(false);

        static auto healthbar = new c_ui_checkbox;
        healthbar->setup(str("Health"), &config->b.vis_healthbar);

        static auto name = new c_ui_checkbox;
        name->setup(str("Name"), &config->b.vis_name);

        static auto name_color = new c_ui_colorpicker;
        name_color->setup(str("Name color"), config->c.name);
        name_color->set_draw_name(false);

        static auto info = new c_ui_multiselect;
        info->setup(str("Info"), config->m.vis_info);
        info->add(str("Armor"));
        info->add(str("Money"));
        info->add(str("Zoom"));
        info->add(str("Fake duck"));
        info->add(str("Latency"));
        info->add(str("Distance"));
        info->add(str("Bomb"));

        static auto weapon = new c_ui_dropdown;
        weapon->setup(str("Weapon"), &config->i.vis_activeweapon);
        weapon->add(str("Disabled"));
        weapon->add(str("Icon"));
        weapon->add(str("Text"));

        static auto weapon_color = new c_ui_colorpicker;
        weapon_color->setup(str("Weapon color"), config->c.weapon);
        weapon_color->set_draw_name(false);

        static auto ammo = new c_ui_checkbox;
        ammo->setup(str("Ammo"), &config->b.vis_ammo);

        static auto ammo_color = new c_ui_colorpicker;
        ammo_color->setup(str("Ammo color"), config->c.ammo);
        ammo_color->set_draw_name(false);

        vis_players->add(enable);
        vis_players->add(noanim);
        vis_players->add(showteam);
        vis_players->add(dormant);
        vis_players->add(boundingbox);
        vis_players->add(boundingbox_color);
        vis_players->add(healthbar);
        vis_players->add(name);
        vis_players->add(name_color);
        vis_players->add(info);
        vis_players->add(weapon);
        vis_players->add(weapon_color);
        vis_players->add(ammo);
        vis_players->add(ammo_color);
    }

    static auto vis_effects = new c_ui_group;
    vis_effects->setup(str("Effects"));
    vis_effects->set_pos(ImVec2(0.f, 215.f));
    vis_effects->set_size(ImVec2(240.f, 205.f));
    {
        static auto bloom_scale = new c_ui_slider_float;
        bloom_scale->setup(str("Bloom slider"), &config->f.vis_bloom_scale, 0.01f, 10.f, str("%.2f"));
        bloom_scale->set_draw_name(false);
        bloom_scale->set_hidden(true);

        static auto bloom = new c_ui_checkbox;
        bloom->setup(str("Bloom"), &config->b.vis_bloom);
        bloom->set_callback([&](c_ui_element*) {
            bloom_scale->set_hidden(!bloom->get_value());
        });

        static auto ae_min = new c_ui_slider_float;
        ae_min->setup(str("Auto exposure slider 1"), &config->f.vis_ae_min, 0.001f, 2.f, str("%.3f"));
        ae_min->set_draw_name(false);
        ae_min->set_hidden(true);

        static auto ae_max = new c_ui_slider_float;
        ae_max->setup(str("Auto exposure slider 2"), &config->f.vis_ae_max, 0.001f, 2.f, str("%.3f"));
        ae_max->set_draw_name(false);
        ae_max->set_hidden(true);

        static auto ae = new c_ui_checkbox;
        ae->setup(str("Auto exposure"), &config->b.vis_autoexposure);
        ae->set_callback([&](c_ui_element*) {
            ae_min->set_hidden(!ae->get_value());
            ae_max->set_hidden(!ae->get_value());
        });

        static auto nopostprocess = new c_ui_checkbox;
        nopostprocess->setup(str("Disable post processing"), &config->b.vis_nopostprocess);
        nopostprocess->set_callback([&](c_ui_element*) {
            auto v = nopostprocess->get_value();
            if (v) {
                bloom_scale->set_hidden(true);
                bloom->set_hidden(true);
                ae_min->set_hidden(true);
                ae_max->set_hidden(true);
                ae->set_hidden(true);
            } else {
                bloom->set_hidden(false);
                bloom->call_callback();

                ae->set_hidden(false);
                ae->call_callback();
            }
        });

        static auto killeffect_time = new c_ui_slider_float;
        killeffect_time->setup(str("Kill effect slider"), &config->f.vis_kf_length, 0.1f, 5.f, str("%.1f"));
        killeffect_time->set_draw_name(false);
        killeffect_time->set_hidden(true);

        static auto killeffect = new c_ui_checkbox;
        killeffect->setup(str("Kill effect"), &config->b.vis_killeffect);
        killeffect->set_callback([&](c_ui_element*) {
            killeffect_time->set_hidden(!killeffect->get_value());
        });

        static auto removals = new c_ui_multiselect;
        removals->setup(str("Remove"), config->m.removals);
        removals->add(str("Flashbang"));
        removals->add(str("Smoke"));
        removals->add(str("Recoil"));
        removals->add(str("Scope overlay"));


        static auto vis_zoom_sens = new c_ui_checkbox;
        vis_zoom_sens->setup(str("Fix zoom sensivity"), &config->b.vis_fix_zoom_sensivity);

        static auto worldmod = new c_ui_checkbox;
        worldmod->setup(str("World modulation"), &config->b.vis_worldmod);

        static auto worldmod_color = new c_ui_colorpicker;
        worldmod_color->setup(str("World modulation color"), config->c.world);
        worldmod_color->set_draw_name(false);

        static auto propsmod = new c_ui_checkbox;
        propsmod->setup(str("Props modulation"), &config->b.vis_propmod);

        static auto propsmod_color = new c_ui_colorpicker;
        propsmod_color->setup(str("Props modulation color"), config->c.prop);
        propsmod_color->set_draw_name(false);

        static auto sky = new c_ui_dropdown;
        sky->setup(str("Sky"), &config->i.vis_sky);
        sky->add(str("Default"));
        sky->add(str("sky_csgo_night02"));

        static auto bulletimpacts = new c_ui_checkbox;
        bulletimpacts->setup(str("Bullet impacts"), &config->b.vis_impacts);

        static auto disablefog = new c_ui_checkbox;
        disablefog->setup(str("Disable fog"), &config->b.vis_disablefog);
        static auto fogprim = new c_ui_colorpicker;
        fogprim->setup(str("Fog primary color"), config->c.fogprimary);
        fogprim->set_alphabar(false);

        static auto fogstart = new c_ui_slider_float;
        fogstart->setup(str("Fog start"), &config->f.vis_fog_start, 0.f, 16384.f, str("%.0fu"));

        static auto fogend = new c_ui_slider_float;
        fogend->setup(str("Fog end"), &config->f.vis_fog_end, 0.f, 16384.f, str("%.0fu"));

        static auto fogdensity = new c_ui_slider_float;
        fogdensity->setup(str("Fog density"), &config->f.vis_fog_density, 0.f, 1.f, str("%.2f"));

        disablefog->set_callback([&](c_ui_element*) {
            auto disable_fog_s = disablefog->get_value();

            if (disable_fog_s) {
                fogprim->set_hidden(true);
                fogstart->set_hidden(true);
                fogend->set_hidden(true);
                fogdensity->set_hidden(true);
            } else{
                fogprim->set_hidden(false);
                fogstart->set_hidden(false);
                fogend->set_hidden(false);
                fogdensity->set_hidden(false);
            }
        });

        vis_effects->add(bloom);
        vis_effects->add(bloom_scale);
        vis_effects->add(ae);
        vis_effects->add(ae_min);
        vis_effects->add(ae_max);
        vis_effects->add(nopostprocess);
        vis_effects->add(killeffect);
        vis_effects->add(killeffect_time);
        vis_effects->add(removals);
        vis_effects->add(vis_zoom_sens);
        vis_effects->add(worldmod);
        vis_effects->add(worldmod_color);
        vis_effects->add(propsmod);
        vis_effects->add(propsmod_color);
        vis_effects->add(sky);
        vis_effects->add(bulletimpacts);
        vis_effects->add(disablefog);
        vis_effects->add(fogprim);
        vis_effects->add(fogstart);
        vis_effects->add(fogend);
        vis_effects->add(fogdensity);
    }

    static auto vis_models = new c_ui_group;
    vis_models->setup(str("Models"));
    vis_models->set_pos(ImVec2(260.f, 0.f));
    vis_models->set_size(ImVec2(240.f, 420.f));
    {
        static auto glow = new c_ui_dropdown;
        glow->setup(str("Glow"), &config->i.vis_glow);
        glow->add(str("Disabled"));
        glow->add(str("Default"));
        glow->add(str("Shaded"));

        static auto glow_color = new c_ui_colorpicker;
        glow_color->setup(str("Glow color"), config->c.glow);
        glow_color->set_draw_name(false);

        static auto glow_team = new c_ui_dropdown;
        glow_team->setup(str("Glow (team)"), &config->i.vis_glow_team);
        glow_team->add(str("Disabled"));
        glow_team->add(str("Default"));
        glow_team->add(str("Shaded"));

        static auto glow_team_color = new c_ui_colorpicker;
        glow_team_color->setup(str("Glow (team) color"), config->c.glow_team);
        glow_team_color->set_draw_name(false);

        static auto glow_self = new c_ui_dropdown;
        glow_self->setup(str("Glow (self)"), &config->i.vis_glow_self);
        glow_self->add(str("Disabled"));
        glow_self->add(str("Default"));
        glow_self->add(str("Shaded"));

        static auto glow_self_color = new c_ui_colorpicker;
        glow_self_color->setup(str("Glow (self) color"), config->c.glow_self);
        glow_self_color->set_draw_name(false);

        static auto glow_weapons = new c_ui_checkbox;
        glow_weapons->setup(str("Glow (dropped weapons)"), &config->b.vis_droppedglow);

        static auto glow_weapons_color = new c_ui_colorpicker;
        glow_weapons_color->setup(str("Glow (dropped weapons) color"), config->c.droppedglow);
        glow_weapons_color->set_draw_name(false);

        static auto glow_nades = new c_ui_checkbox;
        glow_nades->setup(str("Glow (projectiles)"), &config->b.vis_projectilesglow);

        static auto glow_nades_color = new c_ui_colorpicker;
        glow_nades_color->setup(str("Glow (projectiles) color"), config->c.projectilesglow);
        glow_nades_color->set_draw_name(false);

        static auto players = new c_ui_checkbox;
        players->setup(str("Enemies"), &config->b.vis_cm_players);
        static auto player_type = new c_ui_dropdown;
        player_type->setup(str("Enemy model type"), &config->i.vis_cm_type);
        player_type->add(str("Textured"));
        player_type->add(str("Shaded"));
        player_type->add(str("Reflective"));
        player_type->add(str("Cel Shaded"));

        static auto player_color = new c_ui_colorpicker;
        player_color->setup(str("Enemies color"), config->c.cm_players);
        player_color->set_draw_name(false);

        static auto players_walls = new c_ui_checkbox;
        players_walls->setup(str("Enemies behind walls"), &config->b.vis_cm_players_xqz);

        static auto player_walls_color = new c_ui_colorpicker;
        player_walls_color->setup(str("Enemies behind walls color"), config->c.cm_players_xqz);
        player_walls_color->set_draw_name(false);

        static auto backtrack = new c_ui_checkbox;
        backtrack->setup(str("LC"), &config->b.vis_cm_backtrack);

        static auto backtrack_type = new c_ui_dropdown;
        backtrack_type->setup(str("LC model type"), &config->i.vis_cm_backtrack);
        backtrack_type->add(str("Textured"));
        backtrack_type->add(str("Shaded"));
        backtrack_type->add(str("Reflective"));
        backtrack_type->add(str("Cel Shaded"));



        static auto backtrack_color = new c_ui_colorpicker;
        backtrack_color->setup(str("LC color"), config->c.cm_backtrack);
        backtrack_color->set_draw_name(false);

        static auto team = new c_ui_checkbox;
        team->setup(str("Team"), &config->b.vis_cm_team);

        static auto team_type = new c_ui_dropdown;
        team_type->setup(str("Team model type"), &config->i.vis_cm_team_type);
        team_type->add(str("Textured"));
        team_type->add(str("Shaded"));
        team_type->add(str("Reflective"));
        team_type->add(str("Cel Shaded"));

        static auto team_color = new c_ui_colorpicker;
        team_color->setup(str("Team color"), config->c.cm_team);
        team_color->set_draw_name(false);

        static auto self = new c_ui_checkbox;
        self->setup(str("Self"), &config->b.vis_cm_self);

        static auto self_type = new c_ui_dropdown;
        self_type->setup(str("Self model type"), &config->i.vis_cm_self_type);
        self_type->add(str("Textured"));
        self_type->add(str("Shaded"));
        self_type->add(str("Reflective"));
        self_type->add(str("Cel Shaded"));


        static auto self_color = new c_ui_colorpicker;
        self_color->setup(str("Self color"), config->c.self);
        self_color->set_draw_name(false);

        static auto fake = new c_ui_checkbox;
        fake->setup(str("Fake"), &config->b.vis_cm_fake);

        static auto fake_type = new c_ui_dropdown;
        fake_type->setup(str("Fake model type"), &config->i.vis_cm_fake_type);
        fake_type->add(str("Textured"));
        fake_type->add(str("Shaded"));
        fake_type->add(str("Reflective"));
        fake_type->add(str("Animated"));
        fake_type->add(str("Cel Shaded"));
        fake_type->add(str("Glow"));


        static auto fake_color = new c_ui_colorpicker;
        fake_color->setup(str("Fake color"), config->c.fake);
        fake_color->set_draw_name(false);

        static auto interpolate = new c_ui_checkbox;
        interpolate->setup(str("Interpolate"), &config->b.vis_cm_nolagpos);
        interpolate->set_desc(str("Interpolates fake model and makes it appear where real model is"));

        static auto overlay = new c_ui_checkbox;
        overlay->setup(str("Overlay"), &config->b.vis_cm_fake_overlayed);
        overlay->set_desc(str("Draws fake model after real one so it overlays real model"));

        static auto nooccl = new c_ui_checkbox;
        nooccl->setup(str("Disable model occlusion"), &config->b.vis_noocclusion);

        vis_models->add(glow);
        vis_models->add(glow_color);
        vis_models->add(glow_team);
        vis_models->add(glow_team_color);
        vis_models->add(glow_self);
        vis_models->add(glow_self_color);
        vis_models->add(glow_weapons);
        vis_models->add(glow_weapons_color);
        vis_models->add(glow_nades);
        vis_models->add(glow_nades_color);
        vis_models->add(players);
        vis_models->add(player_type);
        vis_models->add(player_color);
        vis_models->add(players_walls);
        vis_models->add(player_walls_color);
        vis_models->add(backtrack);
        vis_models->add(backtrack_type);
        vis_models->add(backtrack_color);
        vis_models->add(team);
        vis_models->add(team_type);
        vis_models->add(team_color);
        vis_models->add(self);
        vis_models->add(self_type);
        vis_models->add(self_color);
        vis_models->add(fake);
        vis_models->add(fake_type);
        vis_models->add(fake_color);
        vis_models->add(interpolate);
        vis_models->add(overlay);
        vis_models->add(nooccl);
    }

    static auto vis_other = new c_ui_group;
    vis_other->setup(str("Other"));
    vis_other->set_pos(ImVec2(520.f, 0.f));
    vis_other->set_size(ImVec2(240.f, 420.f));
    {
        static auto offset = new c_ui_slider_int;
        offset->setup(str("Show offscreen players slider 1"), &config->i.vis_offscreen_offset, 0, 480, str("%dpx"));
        offset->set_draw_name(false);
        offset->set_pm_active(true);
        offset->set_hidden(true);
        offset->set_desc(str("Offset from center"));

        static auto size = new c_ui_slider_int;
        size->setup(str("Show offscreen players slider 2"), &config->i.vis_offscreen_size, 15, 45, str("%dpx"));
        size->set_draw_name(false);
        size->set_hidden(true);
        size->set_desc(str("Size of arrow"));


        static auto col = new c_ui_colorpicker;
        col->setup(str("Show offscreen players color"), config->c.offscreen);
        col->set_draw_name(false);
        col->set_hidden(true);

        static auto offscreen = new c_ui_checkbox;
        offscreen->setup(str("Show offscreen players"), &config->b.vis_offscreen);
        offscreen->set_callback([&](c_ui_element*) {
            offset->set_hidden(!offscreen->get_value());
            size->set_hidden(!offscreen->get_value());
            col->set_hidden(!offscreen->get_value());
        });

        static auto hitmarker = new c_ui_checkbox;
        hitmarker->setup(str("Hitmarker"), &config->b.vis_hitmarker);

        static auto hitsound = new c_ui_checkbox;
        hitsound->setup(str("Hitsound"), &config->b.vis_hitsound);



        static auto width = new c_ui_slider_int;
        width->setup(str("Bullet tracers slider 1"), &config->i.vis_tracerwidth, 1, 15);
        width->set_draw_name(false);
        width->set_hidden(true);
        width->set_desc(str("Width"));


        static auto tracers_color = new c_ui_colorpicker;

        static auto tracers = new c_ui_checkbox;
        tracers->setup(str("Bullet tracers"), &config->b.vis_bullettracer);
        tracers->set_callback([&](c_ui_element*) {
            width->set_hidden(!tracers->get_value());
            tracers_color->set_hidden(!tracers->get_value());
        });


        tracers_color->setup(str("Bullet tracers color"), config->c.tracer);
        tracers_color->set_draw_name(false);
        tracers_color->set_hidden(true);

        static auto sound_esp = new c_ui_checkbox;
        static auto sound_esp_col = new c_ui_colorpicker;
        sound_esp->setup(str("Sound ESP"), &config->b.vis_sound);
        sound_esp->set_callback([&](c_ui_element*) {
            sound_esp_col->set_hidden(!sound_esp->get_value());
        });

        sound_esp_col->setup(str("Sound ESP Color"), config->c.soundcolor);
        sound_esp_col->set_draw_name(false);
        sound_esp_col->set_hidden(true);

        static auto thirdperson = new c_ui_checkbox;
        thirdperson->setup(str("Thirdperson"), &config->b.vis_thirdperson);

        static auto thirdperson_key = new c_ui_hotkey;
        thirdperson_key->setup(str("Thirdperson hotkey"), &config->i.vis_thirdperson, &config->i.vis_thirdperson_style);
        thirdperson_key->set_draw_name(false);

        static auto thirdperson_dist = new c_ui_slider_int;
        thirdperson_dist->setup(str("Thirdperson slider"), &config->i.vis_thirdperson_dist, 50, 200);
        thirdperson_dist->set_draw_name(false);

        static auto thirdperson_dead = new c_ui_checkbox;
        thirdperson_dead->setup(str("Thirdperson while dead"), &config->b.vis_thirdperson_dead);


        static auto specs = new c_ui_checkbox;
        specs->setup(str("Spectators"), &config->b.vis_speclist);

        static auto bomb = new c_ui_checkbox;
        bomb->setup(str("Bomb timer"), &config->b.vis_bombtimer);

        static auto grenade_preview = new c_ui_checkbox;
        grenade_preview->setup(str("Grenade prediction"), &config->b.vis_grenade_preview);


        static auto dropped_name = new c_ui_dropdown;
        dropped_name->setup(str("Name (weapons)"), &config->i.vis_droppedaw);
        dropped_name->add(str("Disabled"));
        dropped_name->add(str("Icon"));
        dropped_name->add(str("Text"));
        dropped_name->set_hidden(true);

        static auto dropped_name_color = new c_ui_colorpicker;
        dropped_name_color->setup(str("Name (weapons) color"), config->c.droppedaw);
        dropped_name_color->set_draw_name(false);
        dropped_name_color->set_hidden(true);

        static auto dropped_ammo = new c_ui_checkbox;
        dropped_ammo->setup(str("Ammo"), &config->b.vis_droppedam);
        dropped_ammo->set_hidden(true);

        static auto dropped_ammo_color = new c_ui_colorpicker;
        dropped_ammo_color->setup(str("Ammo color"), config->c.droppedam);
        dropped_ammo_color->set_draw_name(false);
        dropped_ammo_color->set_hidden(true);

        static auto dropped_dist = new c_ui_checkbox;
        dropped_dist->setup(str("Distance"), &config->b.vis_droppeddist);
        dropped_dist->set_hidden(true);

        static auto dropped_dd = new c_ui_slider_int;
        dropped_dd->setup(str("Draw distance"), &config->i.vis_droppeddd, 3, 26, str("%dm"));
        dropped_dd->set_callback([&](c_ui_element*) {
            if (dropped_dd->get_value() == 26)
                dropped_dd->set_format(str("Infinite"));
            else
                dropped_dd->set_format(str("%dm"));
        });
        dropped_dd->set_hidden(true);

        static auto dropped = new c_ui_checkbox;
        dropped->setup(str("Dropped weapons"), &config->b.vis_droppedesp);
        dropped->set_callback([&](c_ui_element*) {
            auto h = !dropped->get_value();
            dropped_name->set_hidden(h);
            dropped_name_color->set_hidden(h);
            dropped_ammo->set_hidden(h);
            dropped_ammo_color->set_hidden(h);
            dropped_dist->set_hidden(h);
            dropped_dd->set_hidden(h);
        });


        static auto nades_name = new c_ui_dropdown;
        nades_name->setup(str("Name (projectiles)"), &config->i.vis_projectilesna);
        nades_name->add(str("Disabled"));
        nades_name->add(str("Icon"));
        nades_name->add(str("Text"));
        nades_name->set_hidden(true);

        static auto nades_name_color = new c_ui_colorpicker;
        nades_name_color->setup(str("Name (projectiles) color"), config->c.projectilesna);
        nades_name_color->set_draw_name(false);
        nades_name_color->set_hidden(true);

        static auto nades = new c_ui_checkbox;
        nades->setup(str("Projectiles"), &config->b.vis_projectiles);
        nades->set_callback([&](c_ui_element*) {
            auto h = !nades->get_value();
            nades_name->set_hidden(h);
            nades_name_color->set_hidden(h);
        });



        static auto awcross_col = new c_ui_colorpicker;
        awcross_col->setup(str("Damage color"), config->c.damagecolor);
        nades_name_color->set_draw_name(false);
        nades_name_color->set_hidden(true);

        static auto awcross_nograd = new c_ui_checkbox;
        awcross_nograd->setup(str("Show damage"), &config->b.vis_pipiska);
        awcross_nograd->set_desc(str("Shows possible damage that can be inflicted through wall"));
        awcross_nograd->set_hidden(true);
        awcross_nograd->set_callback([&](c_ui_element*){
            awcross_col->set_hidden(!awcross_nograd->get_value());
        });
        static auto awcross = new c_ui_checkbox;
        awcross->setup(str("Penetration crosshair"), &config->b.vis_penecross);
        awcross->set_callback([&](c_ui_element*) {
            awcross_nograd->set_hidden(!awcross->get_value());
        });

        static auto hostages = new c_ui_checkbox;
        hostages->setup(str("Hostages"), &config->b.vis_hostages);

        static auto hostages_col = new c_ui_colorpicker;
        hostages_col->set_draw_name(false);
        hostages_col->setup(str("Hostages color"), config->c.hostages);

        static auto radar = new c_ui_checkbox;
        radar->setup(str("Radar"), &config->b.vis_radar);

        vis_other->add(offscreen);
        vis_other->add(col);
        vis_other->add(offset);
        vis_other->add(size);
        vis_other->add(hitmarker);
        vis_other->add(hitsound);
        vis_other->add(tracers);
        vis_other->add(tracers_color);
        vis_other->add(width);
        vis_other->add(sound_esp);
        vis_other->add(sound_esp_col);
        vis_other->add(thirdperson);
        vis_other->add(thirdperson_key);
        vis_other->add(thirdperson_dist);
        vis_other->add(thirdperson_dead);
        vis_other->add(grenade_preview);
        vis_other->add(specs);
        vis_other->add(bomb);
        vis_other->add(dropped);
        vis_other->add(dropped_name);
        vis_other->add(dropped_name_color);
        vis_other->add(dropped_ammo);
        vis_other->add(dropped_ammo_color);
        vis_other->add(dropped_dist);
        vis_other->add(dropped_dd);
        vis_other->add(nades);
        vis_other->add(nades_name);
        vis_other->add(nades_name_color);
        vis_other->add(awcross);
        vis_other->add(awcross_nograd);
        vis_other->add(awcross_col);
        vis_other->add(hostages);
        vis_other->add(hostages_col);
        vis_other->add(radar);
    }

    visuals->add(vis_players);
    visuals->add(vis_effects);
    visuals->add(vis_models);
    visuals->add(vis_other);
}