//
// Created by ruppet on 3/14/2020.
//

#include "../../cheat.h"

using namespace std::chrono_literals;

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

void c_menu::misc_tab(c_ui_tab* misc) {
    static auto misc_other = new c_ui_group;
    misc_other->setup(str("Miscellaneous"));
    misc_other->set_size(ImVec2(240.f, 420.f));
    {
        static auto override_fov = new c_ui_slider_int;
        override_fov->setup(str("Override FOV"), &config->i.misc_overridefov, -60, 60, str("%d°"));
        override_fov->set_pm_active(true);

        static auto override_viewmodel_fov = new c_ui_slider_int;
        override_viewmodel_fov->setup(str("Override viewmodel FOV"), &config->i.misc_viewmodelfov, -60, 60,
                                      str("%d°"));
        override_viewmodel_fov->set_pm_active(true);

        static auto override_zoom = new c_ui_slider_int;
        override_zoom->setup(str("Override zoom FOV"), &config->i.misc_zoom_fov, 0, 100, str("%d%%"));
        override_zoom->set_pm_active(true);

        static auto bunnyhop = new c_ui_checkbox;
        bunnyhop->setup(str("Auto jump"), &config->b.misc_autojump);

        static auto wasdstrafe = new c_ui_checkbox;
        wasdstrafe->setup(str("Directional"), &config->b.misc_wasdmove);
        wasdstrafe->set_desc(str("Hold WASD keys to change strafe direction without changing angle"));
        wasdstrafe->set_hidden(true);

        static auto autostrafe = new c_ui_checkbox;
        autostrafe->setup(str("Auto strafe"), &config->b.misc_airstrafe);
        autostrafe->set_desc(str("Automatically strafes in air. Hold SHIFT to disable (helps when jumpscouting)"));
        autostrafe->set_callback([&](c_ui_element*) {
            wasdstrafe->set_hidden(!autostrafe->get_value());
        });

        static auto infiniteduck = new c_ui_checkbox;
        infiniteduck->setup(str("Infinite duck"), &config->b.misc_infduck);

        static auto events = new c_ui_multiselect;
        events->setup(str("Log events"), config->m.events);
        events->add("Hit");
        events->add("Hurt");
        events->add("Buy");

        static auto clantagspammer = new c_ui_checkbox;
        clantagspammer->setup(str("Clantag spammer"), &config->b.misc_clantag);

        static auto customratio = new c_ui_slider_int;
        customratio->setup(str("Aspect ratio slider"), &config->i.misc_aspectratio, 1, 200, str("%d%%"));
        customratio->set_draw_name(false);
        customratio->set_pm_active(true);
        customratio->set_hidden(true);

        static auto aspectratio = new c_ui_dropdown;
        aspectratio->setup(str("Aspect ratio"), &config->i.misc_aspectratiomd);
        aspectratio->add(str("Disabled"));
        aspectratio->add(str("1:1"));
        aspectratio->add(str("5:4"));
        aspectratio->add(str("4:3"));
        aspectratio->add(str("3:2"));
        aspectratio->add(str("14:9"));
        aspectratio->add(str("16:10"));
        aspectratio->add(str("16:9"));
        aspectratio->add(str("21:9"));
        aspectratio->add(str("11:4"));
        aspectratio->add(str("64:45"));
        aspectratio->add(str("Custom"));
        aspectratio->set_callback([&](c_ui_element*) {
            customratio->set_hidden(aspectratio->get_value() != 11);
        });

        static auto svpure = new c_ui_checkbox;
        svpure->setup(str("Bypass sv_pure"), &config->b.misc_svpurebypass);

        misc_other->add(override_fov);
        misc_other->add(override_viewmodel_fov);
        misc_other->add(override_zoom);
        misc_other->add(bunnyhop);
        misc_other->add(autostrafe);
        misc_other->add(wasdstrafe);
        misc_other->add(infiniteduck);
        misc_other->add(events);
        misc_other->add(clantagspammer);
        misc_other->add(aspectratio);
        misc_other->add(customratio);
        misc_other->add(svpure);
    }

    static auto misc_cheat = new c_ui_group;
    misc_cheat->setup(str("Cheat settings"));
    misc_cheat->set_pos(ImVec2(260.f, 0.f));
    misc_cheat->set_size(ImVec2(240.f, 420.f));
    {
        static auto menukey = new c_ui_hotkey;
        menukey->setup(str("Menu key"), &config->i.menukey);
        menukey->set_desc(str("Menu toggle key"));

        static auto accent = new c_ui_colorpicker;
        accent->setup(str("Accent"), config->c.menu);
        accent->set_alphabar(false);

        static auto antiuntrusted = new c_ui_checkbox;
        antiuntrusted->setup(str("Anti untrusted"), &config->b.anti_untrusted);
        antiuntrusted->set_desc(str("Disables some features that may cause untrusted ban"));

        static auto low_fps = new c_ui_checkbox;
        low_fps->setup(str("Low FPS warning"), &config->b.low_fps_warning);

        static auto debug_logs = new c_ui_checkbox;
        debug_logs->setup(str("Debug logs"), &config->b.misc_debuglogs);
        debug_logs->set_desc(str("Show debug information in event logs"));

        static auto rpc = new c_ui_button;
        rpc->setup(str("Enable discord RPC"));
        rpc->set_callback([&](c_ui_element*) {
            if (!features->discord->is_initializing) {
                if (!features->discord->is_initialized) {
                    rpc->set_name(str("Disable discord RPC"));
                    std::thread([]() { features->discord->initialize(); }).detach();
                } else {
                    rpc->set_name(str("Enable discord RPC"));
                    std::thread([]() { features->discord->release(); }).detach();
                }
            }
        });

        static auto debug1 = new c_ui_button;
        debug1->setup(str("debug"));
        debug1->set_callback([&](c_ui_element*) {
            core->loadanim_text = str("processing");
            core->is_loadanim_shown = true;

            std::thread([&]() {
                core->dump_netvars();
                core->is_loadanim_shown = false;
            }).detach();
        });

        static auto debug2 = new c_ui_button;
        debug2->setup(str("debug"));
        debug2->set_callback([&](c_ui_element*) {
            auto td = csgo->local_player->get_datamap();

            utils->log(str("%s (%i) ->"), td->dataClassName, td->dataNumFields);
            for (auto i = 0; i < td->dataNumFields; i++) {
                auto f = &td->dataDesc[i];
                if (f->fieldType == FIELD_VOID) continue;

                utils->log(str("\t%s [0x%X]"), f->fieldName, f->fieldOffset[TD_OFFSET_NORMAL]);
            }

            if (td->baseMap) {
                td = td->baseMap;

                utils->log(str("\t%s (%i) ->"), td->dataClassName, td->dataNumFields);
                for (auto i = 0; i < td->dataNumFields; i++) {
                    auto f = &td->dataDesc[i];
                    if (f->fieldType == FIELD_VOID) continue;

                    utils->log(str("\t\t%s [0x%X]"), f->fieldName, f->fieldOffset[TD_OFFSET_NORMAL]);
                }
            }
        });

        misc_cheat->add(menukey);
        misc_cheat->add(accent);
        misc_cheat->add(antiuntrusted);
        misc_cheat->add(low_fps);
        if(CHEAT_ID != 1) {
            misc_cheat->add(debug_logs);
        }
        misc_cheat->add(rpc);
        //misc_cheat->add(debug1);
        //misc_cheat->add(debug2);
    }

    static auto misc_presets = new c_ui_group;
    misc_presets->setup(str("Presets"));
    misc_presets->set_pos(ImVec2(520.f, 0.f));
    misc_presets->set_size(ImVec2(240.f, 420.f));
    {
        static auto presets = new c_ui_listbox;
        presets->setup(str("Presets"), &config->selected_slot);
        presets->set_size(ImVec2(195.f, 160.f));

        static std::string cfg;

        static auto config_name = new c_ui_textbox;
        config_name->setup(str("Config name"), &cfg, 24);
        config_name->set_draw_name(false);

        static auto create = new c_ui_button;
        create->setup(str("Create"));
        create->set_callback([&](c_ui_element*) {
            trim(cfg);
            if (cfg == "")
                return;

            std::ofstream nc(str("c:/uc-sflying/cfg/csgo_") + cfg + str(".dat"));
            nc.close();

            config->refresh();

            ui->add_message(str("Config created"), str("Config '") + cfg + str("' has been created"));
        });

        static auto remove = new c_ui_button;
        remove->setup(str("Remove"));
        remove->set_callback([&](c_ui_element*) {
            auto cfg_name = config->slots.at(config->selected_slot);
            ::remove((str("c:/uc-sflying/cfg/csgo_") + cfg_name + str(".dat")).c_str());

            config->refresh();

            ui->add_message(str("Config removed"), str("Config '") + cfg_name + str("' has been removed"));
        });

        static auto load = new c_ui_button;
        load->setup(str("Load"));
        load->set_callback([&](c_ui_element*) {
            std::thread([]() -> void { config->load(); }).detach();
        });

        static auto save = new c_ui_button;
        save->setup(str("Save"));
        save->set_callback([&](c_ui_element*) {
            std::thread([]() -> void { config->save(); }).detach();
        });

        static auto reset = new c_ui_button;
        reset->setup(str("Reset"));
        reset->set_callback([&](c_ui_element*) {
            std::thread([]() -> void { config->reset(); }).detach();
        });

        static auto refresh = new c_ui_button;
        refresh->setup(str("Refresh"));
        refresh->set_callback([&](c_ui_element*) {
            config->refresh();
        });

        static auto settings_folder = new c_ui_button;
        settings_folder->setup(str("Open settings folder"));
        settings_folder->set_callback([&](c_ui_element*) {
            std::thread([]() -> void {
                core->loadanim_text = str("Opening settings folder...");
                core->is_loadanim_shown = true;

                std::this_thread::sleep_for(1s);
                ShellExecuteA(0, str("open"), str("c:/uc-sflying/cfg"), NULL, NULL, SW_NORMAL);

                core->is_loadanim_shown = false;
            }).detach();
        });

        presets->set_callback([&](c_ui_element*) {
            if (presets->count() <= 0) {
                load->set_active(false);
                save->set_active(false);
                remove->set_active(false);
            } else {
                load->set_active(true);
                save->set_active(true);
                remove->set_active(true);
            }
        });

        for (auto s : config->slots)
            presets->add(s);

        presets->call_callback();

        misc_presets->add(presets);
        misc_presets->add(config_name);
        misc_presets->add(create);
        misc_presets->add(refresh);
        misc_presets->add(remove);
        misc_presets->add(load);
        misc_presets->add(save);
        misc_presets->add(reset);
        misc_presets->add(settings_folder);
    }

    misc->add(misc_other);
    misc->add(misc_cheat);
    misc->add(misc_presets);
}