//
// Created by ruppet on 27.12.2019.
//

#include "../../cheat.h"
#include "../../framework/config.h"

void c_indicators::draw(ImDrawList* d) {
    if (!csgo->engine->is_in_game() || !csgo->engine->is_connected() || !csgo->local_player || csgo->local_player->health() <= 0)
        return;

    std::vector<s_indicator> indicators;

    auto fps = 1.f / csgo->globals->absframetime;
    if ((fps < 96.f || csgo->mitigate_fps) && config->b.low_fps_warning) {
        s_indicator ind;
        ind.text = str("FPS");
        ind.color = features->visuals->percent_to_color(utils->map(std::clamp(fps, 64.f, 128.f), 64.f, 128.f, 0, 100));

        indicators.push_back(ind);
    }

    if (config->b.rage_rapidfire && keymanager->check(config->i.rage_rapidfire_key, config->i.rage_rapidfire_keystyle)) {
        s_indicator ind;
        ind.text = str("RF");
        ind.color = ImColor(255, 255, 255);

        ind.bar.state = true;

        if (features->future->await <= 0) {
            if (features->future->ready)
                ind.bar.color = features->visuals->percent_to_color(100);
            else {
                if (!features->future->enable)
                    ind.bar.color = features->visuals->percent_to_color(0);
                else
                    ind.bar.color = features->visuals->percent_to_color(50);
            }
        } else
            ind.bar.color = ImColor(255, 255, 255);

        ind.bar.min = 0;
        ind.bar.max = 50;

        if (features->future->await <= 0) {
            if (features->future->ready)
                ind.bar.value = 50;
            else
                ind.bar.value = 50 - std::clamp(features->future->recharge, 0, 50);
        }
        else
            ind.bar.value = features->future->await;

        indicators.push_back(ind);
    }

    if (keymanager->check(config->i.rage_aim_body, config->i.rage_aim_body_style)) {
        s_indicator ind;
        ind.text = str("BODY");
        ind.color = features->visuals->percent_to_color(100.f);

        indicators.push_back(ind);
    }

    if (csgo->aa_desync) {
        s_indicator ind;
        ind.text = str("FAKE");
        ind.color = features->visuals->percent_to_color(fabsf(csgo->gfy_delta) / 58.f * 100.f);

        ind.side.state = true;
        ind.side.color = ImColor(255, 255, 255);
        if (keymanager->check(config->i.aa_invert, config->i.aa_invert_style))
            ind.side.text = str("R");
        else
            ind.side.text = str("L");

        if (config->b.aa_manual && config->b.aa_manual_indicator) {
            ind.desc.state = true;
            ind.desc.color = ImColor(255, 255, 255);

            switch (features->antiaims->override) {
                case 0: ind.desc.text = str("NONE"); break;
                case 1: ind.desc.text = str("RIGHT"); break;
                case 2: ind.desc.text = str("LEFT"); break;
                case 3: ind.desc.text = str("OPPOSITE"); break;
            }
        }

        indicators.push_back(ind);
    }

    if (keymanager->check(config->i.aa_fakeduck, config->i.aa_fakeduck_style)) {
        s_indicator ind;
        ind.text = str("DUCK");
        ind.color = ImColor(255, 255, 255);

        ind.bar.state = true;
        ind.bar.color = ImColor(255, 255, 255);
        ind.bar.min = 48.f;
        ind.bar.max = 64.f;
        ind.bar.value = std::clamp(csgo->local_player->view_offset().z, 48.f, 64.f);

        indicators.push_back(ind);
    }

    for (auto ei : ext_indicators)
        indicators.push_back(ei);
    ext_indicators.clear();

    auto w = 0, h = 0;
    csgo->engine->screen_size(w, h); h -= 50;

    auto y = 0;
    for (auto &e : indicators) {
        y += 26;

        if (e.bar.state)
            y += 4;
        if (e.desc.state)
            y += 5;
    }

    for (auto &e : indicators) {
        auto addition = 0;

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[6]);
        auto base = ImGui::CalcTextSize(e.text.c_str());

        d->AddTextSoftShadow(ImVec2(25, h - y), e.color, e.text.c_str());
        ImGui::PopFont();

        addition = base.y;

        if (e.bar.state) {
            auto add = &e.bar;

            if (!add->large) {
                d->AddRectFilled(ImVec2(25, h - y + addition), ImVec2(25 + base.x, h - y + addition + 4),
                                 ImColor(0, 0, 0, 150));

                auto fill = (base.x - 2) / (add->max - add->min) * (add->value - add->min);
                d->AddRectFilled(ImVec2(26, h - y + addition + 1), ImVec2(26 + fill, h - y + addition + 3), add->color);
            } else {
                d->AddRectFilled(ImVec2(25, h - y + addition - 2), ImVec2(25 + base.x, h - y + addition + 3),
                                 ImColor(0, 0, 0, 150));

                auto fill = (base.x - 2) / (add->max - add->min) * (add->value - add->min);
                d->AddRectFilled(ImVec2(26, h - y + addition - 1), ImVec2(26 + fill, h - y + addition + 2), add->color);
            }

            addition += 6 + (e.desc.state ? 4 : 0);
        }

        if (e.desc.state) {
            auto add = &e.desc;

            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
            d->AddTextShadow(ImVec2(26, h - y + addition - 4), add->color, add->text.c_str());

            addition += ImGui::CalcTextSize(add->text.c_str()).y / 2;
            ImGui::PopFont();
        }

        if (e.side.state) {
            auto add = &e.side;

            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[6]);
            d->AddTextSoftShadow(ImVec2(30 + base.x, h - y), add->color, add->text.c_str());
            ImGui::PopFont();
        }

        y -= addition + 2;
    }
}

void c_indicators::add(s_indicator ind) {
    if (!csgo->engine->is_in_game() || !csgo->engine->is_connected() || !csgo->local_player || csgo->local_player->health() <= 0)
        return;

    ext_indicators.push_back(ind);
    //ext_indicators.push_back({ text, ImColor(color), false });
}

void c_indicators::add_with_bar(std::string text, ImU32 color, ImU32 barcolor, float max, float value) {
    //ext_indicators.push_back({ text, ImColor(color), true, ImColor(barcolor), max, value });
}