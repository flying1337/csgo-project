//
// Created by ruppet on 21.01.2020.
//

#include "c_ui.h"

void c_ui::setup() {
    painting = &ImGui::GetCurrentContext()->BackgroundDrawList;

    auto io = &ImGui::GetIO();

    ImFontConfig cfg;
    io->Fonts->AddFontFromFileTTF("c:/windows/fonts/verdana.ttf", 12.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());
    io->Fonts->AddFontFromFileTTF("c:/windows/fonts/verdanab.ttf", 12.f, &cfg, io->Fonts->GetGlyphRangesCyrillic());

    auto imstyle = &ImGui::GetStyle();
    imstyle->AntiAliasedFill = false;
    imstyle->AntiAliasedLines = false;

    style.background        = ImColor(24, 24, 24);
    style.background_hover  = ImColor(19, 19, 19);
    style.bottom            = ImColor(13, 13, 13);
    style.bottom_hover      = ImColor(9, 9, 9);
    style.outline           = ImColor(48, 48, 48);
    style.outline_hover     = ImColor(96, 96, 96);
    style.accent            = ImColor(239, 9, 95);
    style.accent_hover      = ImColor(204, 6, 80);
    style.shadow            = ImColor(6, 6, 6);
    style.shadow_hover      = ImColor(24, 24, 24, 0);
    style.text_normal       = ImColor(235, 235, 235);
    style.text_disabled     = ImColor(64, 64, 64);
    style.text_hovered      = ImColor(128, 128, 128);

    fonts.main          = io->Fonts->Fonts[0];
    fonts.main_bold     = io->Fonts->Fonts[1];

    is_setup = true;
}

void c_ui::draw() {
    if (!is_setup) return;

    for (auto wnd : windows) {
        wnd->set_parent(this);
        wnd->draw();
    }

    auto x_pos = ImGui::GetIO().DisplaySize.x - 10.f;
    auto y_pos = 10.f;

    for (auto msg : messages) {
        if (!msg->drawable())
            continue;

        msg->set_pos(ImVec2(x_pos, y_pos));
        msg->draw(&ImGui::GetCurrentContext()->ForegroundDrawList);

        y_pos += 50.f;
    }
}

ImDrawList *c_ui::get_painting() {
    return painting;
}

void c_ui::add(c_ui_window *wnd) {
    windows.push_back(wnd);
}

bool c_ui::is_hovered(ImRect r) {
    if (active_id && active_id != current_id) return false;

    auto mp = ImGui::GetIO().MousePos;
    return mp.x >= r.Min.x && mp.x <= r.Max.x && mp.y >= r.Min.y && mp.y <= r.Max.y;
}

bool c_ui::is_held() {
    if (active_id && active_id != current_id) return false;
    return ImGui::GetIO().MouseDown[0];
}

bool c_ui::is_clicked() {
    if (active_id && active_id != current_id) return false;
    return ImGui::GetIO().MouseClicked[0];
}

bool c_ui::is_right_clicked() {
    if (active_id && active_id != current_id) return false;
    return ImGui::GetIO().MouseClicked[1];
}

bool c_ui::is_released() {
    if (active_id && active_id != current_id) return false;
    return ImGui::GetIO().MouseReleased[0];
}

ImColor c_ui::animate_color(ImColor cur, ImColor a, ImColor b, float t, bool dec) {
    if (!dec) {
        if (cur.Value.x < b.Value.x)
            cur.Value.x += (b.Value.x - a.Value.x) / t *
                           ImGui::GetIO().DeltaTime;
        if (cur.Value.y < b.Value.y)
            cur.Value.y += (b.Value.y - a.Value.y) / t *
                           ImGui::GetIO().DeltaTime;
        if (cur.Value.z < b.Value.z)
            cur.Value.z += (b.Value.z - a.Value.z) / t *
                           ImGui::GetIO().DeltaTime;
    } else {
        if (cur.Value.x > a.Value.x)
            cur.Value.x -= (b.Value.x - a.Value.x) / t *
                           ImGui::GetIO().DeltaTime;
        if (cur.Value.y > a.Value.y)
            cur.Value.y -= (b.Value.y - a.Value.y) / t *
                           ImGui::GetIO().DeltaTime;
        if (cur.Value.z > a.Value.z)
            cur.Value.z -= (b.Value.z - a.Value.z) / t *
                           ImGui::GetIO().DeltaTime;
    }

    cur.Value.x = std::clamp(cur.Value.x, 0.f, 1.f);
    cur.Value.y = std::clamp(cur.Value.y, 0.f, 1.f);
    cur.Value.z = std::clamp(cur.Value.z, 0.f, 1.f);

    return cur;
}

void c_ui::add_message(std::string ti, std::string te) {
    if (!this) return;

    auto msg = new c_ui_message;
    msg->setup(std::move(ti), std::move(te), this);

    messages.push_back(msg);
}

bool c_ui::is_unclicked() {
    if (active_id && active_id != current_id) return false;
    return ImGui::GetIO().MouseUnclicked[0];
}

void c_ui_message::setup(std::string ti, std::string te, c_ui* u) {
    title = std::move(ti);
    text = std::move(te);

    ui = u;
    is_drawable = true;

    tick = GetTickCount();
}

void c_ui_message::draw(ImDrawList *d) {
    auto ts = c_ui::text_size(text);
    auto dp = pos - ImVec2(10.f + ts.x, 0.f);

    if (!set_x) {
        x = pos.x + 10.f;
        set_x = true;
    }

    x = std::clamp(x, pos.x - (10.f + ts.x), pos.x + 10.f);
    dp.x = x;

    d->AddRectFilled(dp, dp + ImVec2(ts.x + 10.f, 40.f), ui->style.bottom);
    d->AddRect(dp, dp + ImVec2(ts.x + 10.f, 40.f), ui->style.outline);
    d->AddRect(dp + ImVec2(1.f, 1.f), dp + ImVec2(ts.x + 10.f, 40.f) - ImVec2(1.f, 1.f), ui->style.shadow);

    c_ui::set_font(ui->fonts.main_bold);
    d->AddText(dp + ImVec2(5.f, 6.f), ui->style.text_normal, title.c_str());
    c_ui::set_font(nullptr);

    d->AddText(dp + ImVec2(5.f, 20.f), ui->style.text_normal, text.c_str());

    if (GetTickCount() - tick > 5000)
    {
        if (x < pos.x + 10.f)
            x += (10.f + ts.x) / 0.1f * ImGui::GetIO().DeltaTime;
        else
            is_drawable = false;
    } else {
        if (x > pos.x - (10.f + ts.x))
            x -= (10.f + ts.x) / 0.1f * ImGui::GetIO().DeltaTime;
    }
}
