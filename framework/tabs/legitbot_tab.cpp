//
// Created by ruppet on 3/14/2020.
//

#include "../../cheat.h"

void c_menu::legitbot_tab(c_ui_tab* legitbot) {
    static auto general = new c_ui_group;
    general->setup(str("General"));
    general->set_size(ImVec2(240.f, 420.f));
    {
        static auto enable = new c_ui_checkbox;
        enable->setup(str("Enable"), &config->b.lb_enable);

        static auto flash = new c_ui_checkbox;
        flash->setup(str("Disable if flashed"), &config->b.lb_flash);

        static auto smoke = new c_ui_checkbox;
        smoke->setup(str("Disable if looking at smoke"), &config->b.lb_smoke);

        static auto smokechams = new c_ui_checkbox;
        smokechams->setup(str("Disable colored models in smoke"), &config->b.lb_nochams);

        general->add(enable);
        general->add(flash);
        general->add(smoke);
        general->add(smokechams);
    }

    static auto weapons = new c_ui_group;
    weapons->setup(str("Weapons"));
    weapons->set_pos(ImVec2(260.f, 0.f));
    weapons->set_size(ImVec2(240.f, 420.f));
    {
        static auto aim = new c_ui_checkbox;
        aim->setup(str("Enable"), &config->b.lb_aim_enable[config->i.lb_aim_weapon]);

        static auto key = new c_ui_hotkey;
        key->setup(str("Enable key"), &config->i.lb_aim_key[config->i.lb_aim_weapon], &config->i.lb_aim_keystyle[config->i.lb_aim_weapon]);
        key->set_draw_name(false);

        static auto weapon = new c_ui_dropdown;
        weapon->setup(str("Weapon dropdown"), &config->i.lb_aim_weapon);
        weapon->set_draw_name(false);
        weapon->add(str("Default"));
        weapon->add(str("Pistols"));
        weapon->add(str("SMGs"));
        weapon->add(str("Heavies"));
        weapon->add(str("Rifles"));
        weapon->add(str("Auto snipers"));
        weapon->add(str("Bolt action snipers"));
        weapon->set_callback([&](c_ui_element*) {
            aim->set_var(&config->b.lb_aim_enable[config->i.lb_aim_weapon]);
            key->set_var(&config->i.lb_aim_key[config->i.lb_aim_weapon]);
            key->set_beh(&config->i.lb_aim_key[config->i.lb_aim_weapon]);
        });

        weapons->add(weapon);
        weapons->add(aim);
        weapons->add(key);
    }

    static auto trigger = new c_ui_group;
    trigger->setup(str("Triggerbot"));
    trigger->set_pos(ImVec2(520.f, 0.f));
    trigger->set_size(ImVec2(240.f, 420.f));
    {
        static auto weapon = new c_ui_dropdown;
        weapon->setup(str("Triggerbot dropdown"), &config->i.lb_trg_weapon);
        weapon->set_draw_name(false);
        weapon->add(str("Default"));
        weapon->add(str("Pistols"));
        weapon->add(str("SMGs"));
        weapon->add(str("Heavies"));
        weapon->add(str("Rifles"));
        weapon->add(str("Auto snipers"));
        weapon->add(str("Bolt action snipers"));

        trigger->add(weapon);
    }

    legitbot->add(general);
    legitbot->add(weapons);
    legitbot->add(trigger);
}