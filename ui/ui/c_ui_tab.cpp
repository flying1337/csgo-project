//
// Created by ruppet on 21.01.2020.
//

#include "c_ui.h"
#include "c_ui_tab.h"

void c_ui_tab::setup(std::string n) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());
    is_active = false;
}

void c_ui_tab::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    if (!set_color) {
        text_color = ui->style.text_disabled;
        set_color = true;
    }

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
    }

    // UPDATE
    pos = ImFloor(pos);
    size = ImFloor(size);

    auto w_pos = wnd->get_pos();
    auto t_size = wnd->get_size().x / (float)tab_max;

    pos = w_pos + ImVec2(t_size * (float)tab_id, 20.f);\
    size = ImVec2(t_size, 35.f);

    auto rect = ImRect(pos, pos + size);

    ui->set_current_id(id);
    if (ui->is_hovered(rect)) {
        if (ui->is_held())
            ui->active_id = id;
    }

    if (ui->active_id == id && !ui->is_held()) {
        ui->active_id = 0;

        for (auto e : wnd->elements) {
            if (e->is_tab())
                ((c_ui_tab*)e)->set_active(false);
        }

        is_active = true;
    }

    // DRAW
    auto d = ui->get_painting();

    if (is_active) {
        text_color = ui->style.text_normal;
        d->AddText(pos + size / 2 - c_ui::text_size(name) / 2, text_color, name.c_str());
    }
    else {
        if (ui->is_hovered(rect)) {
            if (height < 35.f)
                height += 35.f / 0.1f * ImGui::GetIO().DeltaTime;
            height = std::clamp(height, 0.f, 35.f);

            if (text_color == ui->style.text_normal)
                text_color = ui->style.text_hovered;
            else
                text_color = c_ui::animate_color(text_color, ui->style.text_disabled, ui->style.text_hovered, 0.1f);

            d->AddRectFilled(pos, pos + size, ui->style.bottom);
            d->AddRectFilled(pos + ImVec2(0.f, size.y), pos + ImVec2(size.x, size.y - height), ui->style.background_hover);
            d->AddText(pos + size / 2 - c_ui::text_size(name) / 2, text_color, name.c_str());
        } else {
            if (height > 0.f)
                height -= 35.f / 0.1f * ImGui::GetIO().DeltaTime;
            height = std::clamp(height, 0.f, 35.f);

            if (text_color == ui->style.text_normal)
                text_color = ui->style.text_disabled;
            else
                text_color = c_ui::animate_color(text_color, ui->style.text_disabled, ui->style.text_hovered, 0.1f, true);

            d->AddRectFilled(pos, pos + size, ui->style.bottom);
            d->AddRectFilled(pos + ImVec2(0.f, size.y), pos + ImVec2(size.x, size.y - height), ui->style.background_hover);
            d->AddText(pos + size / 2 - c_ui::text_size(name) / 2, text_color, name.c_str());
        }
    }

    ui->set_current_id(0);

    if (is_active) {
        for (auto e : elements) {
            if (e->get_hidden())
                continue;

            e->draw(wnd);
        }
    }
}
