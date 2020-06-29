//
// Created by ruppet on 3/14/2020.
//

#include "../../cheat.h"

void c_menu::antiaims_tab(c_ui_tab* antiaims) {
    static auto aa_general = new c_ui_group;
    aa_general->setup(str("General"));
    aa_general->set_size(ImVec2(240.f, 420.f));
    {
        static auto pitch = new c_ui_dropdown;
        pitch->setup(str("Pitch"), &config->i.aa_pitch[config->i.aa]);
        pitch->add(str("Disabled"));
        pitch->add(str("Down"));
        pitch->add(str("Up"));
        pitch->add(str("Jitter"));

        static auto yaw = new c_ui_dropdown;
        yaw->setup(str("Yaw"), &config->i.aa_yaw[config->i.aa]);
        yaw->add(str("Disabled"));
        yaw->add(str("Static"));
        yaw->add(str("Backward"));
        yaw->add(str("Jitter"));

        static auto yaw_offset = new c_ui_slider_int;
        yaw_offset->setup(str("Yaw slider"), &config->i.aa_yaw_offset[config->i.aa], -180, 180, str("%d°"));
        yaw_offset->set_pm_active(true);
        yaw_offset->set_draw_name(false);

        static auto desync = new c_ui_checkbox;
        desync->setup(str("Desync"), &config->b.aa_desync[config->i.aa]);

        static auto minwalk = new c_ui_slider_int;
        minwalk->setup(str("Minwalk speed"), &config->i.aa_minwalkspeed, 1, 16);
        minwalk->set_desc(str("Changes delay between minwalks. Set to odd value to make fake jitter"));

        static auto desync_type = new c_ui_dropdown;
        desync_type->setup(str("Desync dropdown"), &config->i.aa_desync_type);
        desync_type->set_draw_name(false);
        desync_type->add(str("Default"));
        desync_type->add(str("Break"));
        desync_type->add(str("Far"));
        desync_type->add(str("Custom"));
        desync_type->set_callback([&](c_ui_element*) {
            auto v = desync_type->get_value();
            if (v != 3)
                minwalk->set_hidden(true);
            else
                minwalk->set_hidden(false);

            if (v == 0) {
                config->i.aa_fakelag_mode[0] = 1;
                config->i.aa_fakelag_style[0] = 0;
                config->i.aa_fakelag_limit[0] = 1;
                config->i.aa_fakelag_var[0] = 0;
            }

            if (v == 1) {
                config->i.aa_fakelag_mode[0] = 1;
                config->i.aa_fakelag_style[0] = 1;
                config->i.aa_fakelag_limit[0] = 12;
                config->i.aa_fakelag_var[0] = 0;
            }

            if (v == 2) {
                config->i.aa_fakelag_mode[0] = 1;
                config->i.aa_fakelag_style[0] = 0;
                config->i.aa_fakelag_limit[0] = 2;
                config->i.aa_fakelag_var[0] = 0;
            }

            auto e = wnd->find(str("Anti-aims"), str("Fake lags"), str("Fake lags dropdown"));
            if (e)
                e->call_callback();
        });

        static auto aa = new c_ui_dropdown;
        aa->setup(str("General dropdown"), &config->i.aa);
        aa->add(str("Standing"));
        aa->add(str("Moving"));
        aa->add(str("Air"));
        aa->set_draw_name(false);
        aa->set_callback([&](c_ui_element*) {
            pitch->set_var(&config->i.aa_pitch[config->i.aa]);
            yaw->set_var(&config->i.aa_yaw[config->i.aa]);
            yaw_offset->set_var(&config->i.aa_yaw_offset[config->i.aa]);
            desync->set_var(&config->b.aa_desync[config->i.aa]);

            if (config->i.aa != 0) {
                desync_type->set_hidden(true);
                minwalk->set_hidden(true);
            } else {
                desync_type->set_hidden(false);
                desync_type->call_callback();
            }
        });

        static auto freezetime = new c_ui_checkbox;
        freezetime->setup(str("Disable on freezetime"), &config->b.aa_disableonfreeze);

        aa_general->add(aa);
        aa_general->add(pitch);
        aa_general->add(yaw);
        aa_general->add(yaw_offset);
        aa_general->add(desync);
        aa_general->add(desync_type);
        aa_general->add(freezetime);
        aa_general->add(minwalk);
    }

    static auto aa_fakelags = new c_ui_group;
    aa_fakelags->setup(str("Fake lags"));
    aa_fakelags->set_pos(ImVec2(260.f, 0.f));
    aa_fakelags->set_size(ImVec2(240.f, 420.f));
    {
        static auto mode = new c_ui_dropdown;
        mode->setup(str("Mode"), &config->i.aa_fakelag_mode[config->i.aa_fakelag]);
        mode->add(str("Disabled"));
        mode->add(str("Always on"));

        static auto style = new c_ui_dropdown;
        style->setup(str("Style"), &config->i.aa_fakelag_style[config->i.aa_fakelag]);
        style->add(str("Linear"));
        style->add(str("Fluctating"));
        style->add(str("Dynamic"));
        style->add(str("Random"));

        static auto limit = new c_ui_slider_int;
        limit->setup(str("Limit"), &config->i.aa_fakelag_limit[config->i.aa_fakelag], 1, 14);

        static auto variance = new c_ui_slider_int;
        variance->setup(str("Variance"), &config->i.aa_fakelag_var[config->i.aa_fakelag], 0, 100, str("%d%%"));

        static auto fl = new c_ui_dropdown;
        fl->setup(str("Fake lags dropdown"), &config->i.aa_fakelag);
        fl->add(str("Standing"));
        fl->add(str("Moving"));
        fl->add(str("Air"));
        fl->set_draw_name(false);
        fl->set_callback([&](c_ui_element*) {
            if (config->i.aa_fakelag == 0 && config->i.aa_desync_type != 3) {
                mode->set_active(false);
                style->set_active(false);
                limit->set_active(false);
                variance->set_active(false);
                mode->set_desc(str("You cannot change stand fakelag settings while using anti-aim preset."));
                style->set_desc(str("You cannot change stand fakelag settings while using anti-aim preset."));
                limit->set_desc(str("You cannot change stand fakelag settings while using anti-aim preset."));
                variance->set_desc(str("You cannot change stand fakelag settings while using anti-aim preset."));
            } else {
                mode->set_active(true);
                style->set_active(true);
                limit->set_active(true);
                variance->set_active(true);
                mode->set_desc("");
                style->set_desc("");
                limit->set_desc("");
                variance->set_desc("");
            }

            mode->set_var(&config->i.aa_fakelag_mode[config->i.aa_fakelag]);
            style->set_var(&config->i.aa_fakelag_style[config->i.aa_fakelag]);
            limit->set_var(&config->i.aa_fakelag_limit[config->i.aa_fakelag]);
            variance->set_var(&config->i.aa_fakelag_var[config->i.aa_fakelag]);
        });

        static auto nowhileshooting = new c_ui_checkbox;
        nowhileshooting->setup(str("Disable while shooting"), &config->b.aa_fl_disableshooting);

        aa_fakelags->add(fl);
        aa_fakelags->add(mode);
        aa_fakelags->add(style);
        aa_fakelags->add(limit);
        aa_fakelags->add(variance);
        aa_fakelags->add(nowhileshooting);
    }

    static auto aa_other = new c_ui_group;
    aa_other->setup(str("Other"));
    aa_other->set_pos(ImVec2(520.f, 0.f));
    aa_other->set_size(ImVec2(240.f, 420.f));
    {
        static auto legs = new c_ui_checkbox;
        legs->setup(str("Fix leg movement"), &config->b.aa_fix_legs);

        static auto slowmo = new c_ui_hotkey;
        slowmo->setup(str("Slow motion"), &config->i.aa_slowmo, &config->i.aa_slowmo_style);

        static auto speed = new c_ui_slider_int;
        speed->setup(str("Slow motion slider"), &config->i.aa_slowmo_speed, 1, 100, str("%d%%"));
        speed->set_draw_name(false);
        speed->set_pm_active(true);

        static auto mode = new c_ui_dropdown;
        mode->setup(str("Mode"), &config->i.aa_slowmo_type);
        mode->add(str("Prefer speed"));
        mode->add(str("Prefer anti-aim"));

        static auto indicate = new c_ui_checkbox;
        indicate->setup(str("Indicate"), &config->b.aa_manual_indicator);
        indicate->set_desc(str("Show AA direction in indicators"));
        indicate->set_hidden(true);

        static auto left = new c_ui_hotkey;
        left->setup(str("Left"), &config->i.aa_manual_negative);
        left->set_hidden(true);

        static auto right = new c_ui_hotkey;
        right->setup(str("Right"), &config->i.aa_manual_positive);
        right->set_hidden(true);

        static auto opposite = new c_ui_hotkey;
        opposite->setup(str("Opposite"), &config->i.aa_manual_opposite);
        opposite->set_hidden(true);

        static auto manual = new c_ui_checkbox;
        manual->setup(str("Manual override"), &config->b.aa_manual);
        manual->set_callback([&](c_ui_element*) {
            auto v = !manual->get_value();
            indicate->set_hidden(v);
            left->set_hidden(v);
            right->set_hidden(v);
            opposite->set_hidden(v);
        });

        static auto invert = new c_ui_hotkey;
        invert->setup(str("Invert desync"), &config->i.aa_invert, &config->i.aa_invert_style);

        static auto fakeduck = new c_ui_hotkey;
        fakeduck->setup(str("Fake duck"), &config->i.aa_fakeduck, &config->i.aa_fakeduck_style);

        aa_other->add(legs);
        aa_other->add(slowmo);
        aa_other->add(speed);
        aa_other->add(mode);
        aa_other->add(manual);
        aa_other->add(indicate);
        aa_other->add(left);
        aa_other->add(right);
        aa_other->add(opposite);
        aa_other->add(invert);
        aa_other->add(fakeduck);
    }

    antiaims->add(aa_general);
    antiaims->add(aa_fakelags);
    antiaims->add(aa_other);
}