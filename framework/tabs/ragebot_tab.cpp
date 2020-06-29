//
// Created by ruppet on 3/14/2020.
//

#include "../../cheat.h"
#include "../config.h"

void c_menu::ragebot_tab(c_ui_tab *ragebot) {
    static auto rage_general = new c_ui_group;
    rage_general->setup(str("General"));
    rage_general->set_size(ImVec2(240.f, 420.f));
    {
        static auto enable = new c_ui_checkbox;
        enable->setup(str("Enable"), &config->b.rage_enable);

        static auto key = new c_ui_hotkey;
        key->setup(str("Enable hotkey"), &config->i.rage_key, &config->i.rage_key_style);
        key->set_draw_name(false);



        static auto walls = new c_ui_slider_int;
        walls->setup(str("Penetration precision"), &config->i.rage_aw_precision, 1, 4);
        walls->set_desc(str("How many walls to test for penetrating. Lower value = better performance"));

        static auto cock = new c_ui_checkbox;
        cock->setup(str("Auto cock revolver"), &config->b.rage_autorevolver);

        static auto hitscan = new c_ui_dropdown;
        hitscan->setup(str("Hitscan behaviour"), &config->i.rage_behaviour);
        hitscan->set_desc(str("Defines how aimbot should scan enemies"));
        hitscan->add(str("Default"));
        hitscan->add(str("1 per tick"));

        static auto view = new c_ui_checkbox;
        view->setup(str("Override view"), &config->b.rage_overrideview);
        view->set_desc(str("Overrides your view so you can see where aimbot shoots"));
        static auto fov = new c_ui_slider_int;
        fov->setup(str("FOV"), &config->i.rage_fov, 0, 180, str(u8"%d"));
        static auto targets = new c_ui_dropdown;
        targets->setup(str("Target selection"), &config->i.rage_sort);
        targets->add(str("Cyclic"));
        targets->add(str("Lowest health"));
        targets->add(str("Closest to you"));
        targets->add(str("Closest to crosshair"));

        targets->set_callback([&](c_ui_element*) {
            if(config->i.rage_sort == 3)
            fov->set_hidden(false);
            else
                fov->set_hidden(true);
        });

        static auto limbsmoving = new c_ui_checkbox;
        limbsmoving->setup(str("Avoid limbs if moving"), &config->b.rage_avoidlimbs_move);

        static auto limbsjumping = new c_ui_checkbox;
        limbsjumping->setup(str("Avoid limbs if jumping"), &config->b.rage_avoidlimbs_jump);

        static auto baimafter = new c_ui_slider_int;
        baimafter->setup(str("Force body hitbox after"), &config->i.rage_baim_after, 0, 10, str("Off"));
        baimafter->set_callback([&](c_ui_element*) {
            if (baimafter->get_value() > 0)
                baimafter->set_format(str("%d bullets"));
            else
                baimafter->set_format(str("Off"));
        });

        static auto aimpoints = new c_ui_checkbox;
        aimpoints->setup(str("Show aimpoints"), &config->b.rage_aimpoints);

        static auto aimpoints_color = new c_ui_colorpicker;
        aimpoints_color->setup(str("Show aimpoints color"), config->c.aimpoints);
        aimpoints_color->set_draw_name(false);

        rage_general->add(enable);
        rage_general->add(key);
        rage_general->add(walls);
        rage_general->add(cock);
        rage_general->add(limbsmoving);
        rage_general->add(limbsjumping);
        rage_general->add(hitscan);
        rage_general->add(view);
        rage_general->add(targets);
        rage_general->add(fov);
        rage_general->add(baimafter);
        rage_general->add(aimpoints);
        rage_general->add(aimpoints_color);
    }

    static auto rage_weapons = new c_ui_group;
    rage_weapons->setup(str("Weapons"));
    rage_weapons->set_size(ImVec2(240.f, 420.f));
    rage_weapons->set_pos(ImVec2(260.f, 0.f));
    {
        static auto aimbot = new c_ui_checkbox;
        aimbot->setup(str("Aimbot"), &config->b.rage_aim_weapon[config->i.rage_weapon]);

        static auto hitboxes = new c_ui_multiselect;
        hitboxes->setup(str("Hitboxes"), config->m.rage_aim_hitboxes[config->i.rage_weapon]);
        hitboxes->add(str("Head"));
        hitboxes->add(str("Neck"));
        hitboxes->add(str("Upper chest"));
        hitboxes->add(str("Chest"));
        hitboxes->add(str("Body"));
        hitboxes->add(str("Pelvis"));
        hitboxes->add(str("Arms"));
        hitboxes->add(str("Legs"));
        hitboxes->add(str("Feet"));

        static auto pref = new c_ui_dropdown;
        pref->setup(str("Preference"), &config->i.rage_preference[config->i.rage_weapon]);
        pref->add(str("None"));
        pref->add(str("Head"));
        pref->add(str("Chest"));
        pref->add(str("Body"));

        static auto head = new c_ui_slider_int;
        head->setup(str("Head point scale"), &config->i.rage_pointscale_head[config->i.rage_weapon], 1, 100, str("%d%%"));
        head->set_pm_active(true);

        static auto body = new c_ui_slider_int;
        body->setup(str("Body point scale"), &config->i.rage_pointscale_body[config->i.rage_weapon], 1, 100, str("%d%%"));
        body->set_pm_active(true);


        static auto autofire = new c_ui_checkbox;
        autofire->setup(str("Auto fire"), &config->b.rage_autofire[config->i.rage_weapon]);

        static auto hc = new c_ui_slider_int;
        hc->setup(str("Hitchance"), &config->i.rage_aim_hitchance[config->i.rage_weapon], 0, 100, str("Off"));
        hc->set_pm_active(true);
        hc->set_callback([&](c_ui_element*) {
            if (hc->get_value() == 0)
                hc->set_format(str("Off"));
            else
                hc->set_format(str("%d%%"));
        });

        static auto mindamage = new c_ui_slider_int;
        mindamage->setup(str("Minimum damage"), &config->i.rage_aim_mindamage[config->i.rage_weapon], 0, 101, str("Auto"));
        mindamage->set_pm_active(true);
        mindamage->set_callback([&](c_ui_element*) {
            auto v = mindamage->get_value();
            if (v == 0)
                mindamage->set_format(str("Auto"));
            else if (v == 101)
                mindamage->set_format(str("Health"));
            else
                mindamage->set_format(str("%dhp"));
        });

        static auto awdamage = new c_ui_slider_int;
        awdamage->setup(str("Penetration damage"), &config->i.rage_aim_awmindamage[config->i.rage_weapon], 0, 101, str("Auto"));
        awdamage->set_pm_active(true);
        awdamage->set_callback([&](c_ui_element*) {
            auto v = awdamage->get_value();
            if (v == 0)
                awdamage->set_format(str("Auto"));
            else if (v == 101)
                awdamage->set_format(str("Health"));
            else
                awdamage->set_format(str("%dhp"));
        });

        static auto bodyaim_lethal = new c_ui_checkbox;
        bodyaim_lethal->setup(str("Bodyaim if lethal"), &config->b.ragebot_baim_when_lethal[config->i.rage_weapon]);

        static auto autostop = new c_ui_checkbox;
        autostop->setup(str("Auto stop"), &config->b.rage_quickstop[config->i.rage_weapon]);

        static auto move_between_shots = new c_ui_checkbox;
        move_between_shots->setup(str("Move between shots"), &config->b.rage_movebetween[config->i.rage_weapon]);

        static auto autoscope = new c_ui_checkbox;
        autoscope->setup(str("Auto scope"), &config->b.rage_autoscope[config->i.rage_weapon]);

        static auto weapon = new c_ui_dropdown;
        weapon->setup(str("Weapon dropdown"), &config->i.rage_weapon);
        weapon->set_draw_name(false);
        weapon->add(str("Default"));
        weapon->add(str("Pistols"));
        weapon->add(str("Heavy pistols"));
        weapon->add(str("SMGs"));
        weapon->add(str("Heavies"));
        weapon->add(str("Rifles"));
        weapon->add(str("Auto snipers"));
        weapon->add(str("SSG-08"));
        weapon->add(str("AWP"));
        weapon->set_callback([&](c_ui_element*) {
            aimbot->set_var(&config->b.rage_aim_weapon[config->i.rage_weapon]);
            hitboxes->set_var(config->m.rage_aim_hitboxes[config->i.rage_weapon]);
            pref->set_var(&config->i.rage_preference[config->i.rage_weapon]);
            head->set_var(&config->i.rage_pointscale_head[config->i.rage_weapon]);
            body->set_var(&config->i.rage_pointscale_body[config->i.rage_weapon]);
            autofire->set_var(&config->b.rage_autofire[config->i.rage_weapon]);
            hc->set_var(&config->i.rage_aim_hitchance[config->i.rage_weapon]);
            mindamage->set_var(&config->i.rage_aim_mindamage[config->i.rage_weapon]);
            awdamage->set_var(&config->i.rage_aim_awmindamage[config->i.rage_weapon]);
            bodyaim_lethal->set_var(&config->b.ragebot_baim_when_lethal[config->i.rage_weapon]);
            autostop->set_var(&config->b.rage_quickstop[config->i.rage_weapon]);
            move_between_shots->set_var(&config->b.rage_movebetween[config->i.rage_weapon]);
            autoscope->set_var(&config->b.rage_autoscope[config->i.rage_weapon]);

            if (config->i.rage_weapon == 1 || config->i.rage_weapon == 2 || config->i.rage_weapon == 3 || config->i.rage_weapon == 4)
                autoscope->set_hidden(true);
            else
                autoscope->set_hidden(false);

            hc->call_callback();
            mindamage->call_callback();
            awdamage->call_callback();
        });

        rage_weapons->add(weapon);
        rage_weapons->add(aimbot);
        rage_weapons->add(hitboxes);
        rage_weapons->add(pref);
        rage_weapons->add(head);
        rage_weapons->add(body);
        rage_weapons->add(autofire);
        rage_weapons->add(hc);
        rage_weapons->add(mindamage);
        rage_weapons->add(awdamage);
        rage_weapons->add(bodyaim_lethal);
        rage_weapons->add(autostop);
        rage_weapons->add(move_between_shots);
        rage_weapons->add(autoscope);
    }

    static auto rage_other = new c_ui_group;
    rage_other->setup(str("Other"));
    rage_other->set_size(ImVec2(240.f, 420.f));
    rage_other->set_pos(ImVec2(520.f, 0.f));
    {
        static auto norecoil = new c_ui_checkbox;
        norecoil->setup(str("Remove recoil"), &config->b.rage_norecoil);

        static auto forcebody = new c_ui_hotkey;
        forcebody->setup(str("Force body hitbox"), &config->i.rage_aim_body, &config->i.rage_aim_body_style);


        static auto resolveonwrong = new c_ui_checkbox;
        resolveonwrong->setup(str("Resolve if hit wrong hitbox"), &config->b.rage_resolveonwrong);
        resolveonwrong->set_hidden(true);

        static auto delayshot = new c_ui_checkbox;
        delayshot->setup(str("Delay shot"), &config->b.rage_delayshot);

        static auto xp = new c_ui_checkbox;
        xp->setup(str("Extrapolate"), &config->b.rage_extrapolation);

        static auto resolver = new c_ui_checkbox;
        resolver->setup(str("Resolver"), &config->b.rage_resolver);
        resolver->set_callback([&](c_ui_element*) {
            resolveonwrong->set_hidden(!resolver->get_value());
        });


        static auto rapidfire = new c_ui_checkbox;
        static auto rapidfire_key = new c_ui_hotkey;
        static auto rapidfire_speed = new c_ui_slider_int;

        rapidfire->setup(str("Rapid fire"), &config->b.rage_rapidfire);
        rapidfire->set_callback([&](c_ui_element*) {
            rapidfire_key->set_hidden(!rapidfire->get_value());
            rapidfire_speed->set_hidden(!rapidfire->get_value());
        });


        rapidfire_key->setup(str("Rapid fire key"), &config->i.rage_rapidfire_key, &config->i.rage_rapidfire_keystyle);
        rapidfire_key->set_draw_name(false);
        rapidfire_key->set_hidden(true);

        rapidfire_speed->setup(str("Speed"), &config->i.rage_rapidfire_speed, 1, 2, str("%d%"));
        rapidfire_speed->set_hidden(true);
        rapidfire_speed->set_desc(str("Higher speed = Less consistent"));
        rapidfire_speed->set_callback([&](c_ui_element*) {
            auto v = rapidfire_speed->get_value();
            if (v == 2)
                rapidfire_speed->set_format(str("Instant"));
            else if (v == 1)
                rapidfire_speed->set_format(str("Fast"));
        });
        static auto lc = new c_ui_checkbox;
        lc->setup(str("Lag compensation"), &config->b.rage_backtrack);

        static auto lcmp = new c_ui_checkbox;
        lcmp->setup(str("Allow multipoint on LC"), &config->b.rage_btmps);
        lcmp->set_desc(str("Extremely resource intensive, not recommended to use on low-end machines"));

        rage_other->add(norecoil);
        rage_other->add(forcebody);
        rage_other->add(delayshot);
        rage_other->add(xp);
        rage_other->add(resolver);
        rage_other->add(resolveonwrong);
        rage_other->add(rapidfire);
        rage_other->add(rapidfire_key);
        rage_other->add(rapidfire_speed);
        rage_other->add(lc);
        rage_other->add(lcmp);
    }

    ragebot->add(rage_general);
    ragebot->add(rage_weapons);
    ragebot->add(rage_other);
}
