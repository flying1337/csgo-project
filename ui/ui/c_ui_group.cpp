//
// Created by ruppet on 22.01.2020.
//

#include "c_ui.h"
#include "c_ui_group.h"
#include "../imgui_internal.h"

void c_ui_group::setup(std::string n) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());
    scroll_id = ImHashStr((name + "__SCROLL").c_str(), (name + "__SCROLL").size(), rand());
}

void c_ui_group::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    ui->set_current_id(id);

    std::vector<c_ui_element*> rm;
    for (auto e : elements) {
        if (e->gone)
            rm.push_back(e);
    }

    for (auto r : rm)
    {
        for (auto i = 0; i < elements.size(); i++) {
            if (elements.at(i)->get_name() == r->get_name()) {
                elements.erase(elements.begin() + i);
                break;
            }
        }

        delete r;
    }

    // UPDATE
    pos = ImFloor(pos);
    size = ImFloor(size);

    auto any_tab = false;
    for (auto e : wnd->elements) {
        if (e->is_tab() && !e->get_hidden())
            any_tab = true;
    }

    auto real_pos = wnd->get_pos() + pos + (any_tab ? ImVec2(20.f, 65.f) : ImVec2(10.f, 30.f));
    auto real_size = size;

    auto rect = ImRect(real_pos, real_pos + real_size);

    auto y_size = 40.f;
    for (auto e : elements) {
        if (e->get_hidden())
            continue;
        y_size += e->get_size().y + 2.f;
    }

    auto sb_needed = false;
    if (y_size > real_size.y) {
        y_size -= real_size.y;
        sb_needed = true;
    }

    auto scroll_size = std::clamp(size.y - y_size - 20.f, 20.f, size.y - 20.f);
    auto scroll_size_real = size.y - y_size - 20.f;

    auto scroll_pos = scroll / y_size * (size.y - 40.f);
    if (scroll_size_real >= 20.f)
        scroll_pos = scroll;

    auto scroll_rect = ImRect(rect.Min + ImVec2(size.x - 6.f, 21.f - scroll_pos),
                              rect.Min + ImVec2(size.x - 1.f, 19.f + scroll_size - scroll_pos));

    if (sb_needed) {
        ui->set_current_id(scroll_id);
        if (ui->is_held()) {
            if (ui->is_hovered(scroll_rect))
                ui->active_id = scroll_id;

            if (ui->active_id == scroll_id) {
                if (scroll_size_real >= 20.f)
                    scroll -= ImGui::GetIO().MouseDelta.y;
                else
                    scroll -= ImGui::GetIO().MouseDelta.y * (y_size / (size.y - 40.f));

                scroll = std::clamp(scroll, -y_size, 0.f);
            }
        } else {
            if (ui->active_id == scroll_id)
                ui->active_id = 0;
        }
        ui->set_current_id(id);
    }

    if (ui->is_hovered(rect)) {
        if (sb_needed) {
            scroll += ImGui::GetIO().MouseWheel * 20.f;
            scroll = std::clamp(scroll, -y_size, 0.f);
        } else
            scroll = 0.f;
    }

    auto y = real_pos.y + 30.f + scroll;
    auto x = real_pos.x + 10.f;

    for (auto e : elements) {
        if (e->get_hidden())
            continue;

        e->set_pos(ImVec2(x, y));
        y += e->get_size().y + 2.f;
    }

    // DRAW
    auto d = ui->get_painting();

    d->AddRect(rect.Min, rect.Max, ui->style.outline);
    d->AddRect(rect.Min - ImVec2(1.f, 1.f), rect.Max + ImVec2(1.f, 1.f), ui->style.shadow);
    d->AddRectFilled(rect.Min, rect.Min + ImVec2(real_size.x, 20.f), ui->style.outline);

    c_ui::set_font(ui->fonts.main_bold);
    d->AddText(rect.Min + ImVec2(5.f, 4.f), ui->style.text_normal, name.c_str());
    c_ui::set_font(nullptr);

    d->PushClipRect(rect.Min + ImVec2(2.f, 21.f), rect.Max - ImVec2(2.f, 2.f), true);

    auto o_pos = pos, o_size = size;
    pos = real_pos + ImVec2(0.f, 20.f);
    size = real_size - ImVec2(0.f, 20.f);

    for (auto e : elements) {
        if (e->get_hidden())
            continue;

        e->set_parent(this);
        e->draw(wnd);
    }

    pos = o_pos;
    size = o_size;

    d->PopClipRect();

    d->AddRectFilledMultiColor(rect.Min + ImVec2(1.f, 20.f), rect.Min + ImVec2(rect.GetWidth(), 8.f) - ImVec2(1.f, -20.f),
            ui->style.background, ui->style.background,
            ui->style.shadow_hover, ui->style.shadow_hover);
    d->AddRectFilledMultiColor(rect.Max - ImVec2(rect.GetWidth(), 8.f) - ImVec2(-1.f, 1.f), rect.Max - ImVec2(1.f, 1.f),
                               ui->style.shadow_hover, ui->style.shadow_hover,
                               ui->style.background, ui->style.background);

    if (sb_needed) {
        d->AddRectFilled(rect.Min + ImVec2(size.x - 7.f, 20.f), rect.Max - ImVec2(1.f, 1.f), ui->style.outline);

        d->AddRectFilled(rect.Min + ImVec2(size.x - 5.f, 22.f - scroll_pos),
                         rect.Min + ImVec2(size.x - 2.f, 18.f + scroll_size - scroll_pos), ui->style.outline_hover);
    }

    ui->set_current_id(0);
}

