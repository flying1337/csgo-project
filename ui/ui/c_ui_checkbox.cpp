//
// Created by ruppet on 22.01.2020.
//

#include "c_ui.h"
#include "../imgui_internal.h"
#include "c_ui_checkbox.h"


void c_ui_checkbox::setup(std::string n, bool *v) {
    name = std::move(n);
    value = v;
    id = ImHashStr(name.c_str(), name.size(), rand());
    size = ImVec2(20.f, 20.f);
}

void c_ui_checkbox::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    // UPDATE
    size = ImVec2(30.f + c_ui::text_size(name).x, 20.f);
    auto rect = ImRect(pos, pos + size);

    ui->set_current_id(id);
    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_hovered(rect)) {
            if (ui->is_held())
                ui->active_id = id;
        } else {
            if (ui->active_id == id && !ui->is_held())
                ui->active_id = 0;
        }

        if (ui->is_hovered(rect) && ui->is_clicked()) {
            if (value) {
                *value = !*value;

                if (callback)
                    callback(this);
            }
        }
    }

    // DRAW
    auto d = ui->get_painting();

    if (ui->is_hovered(rect))
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f);
    else
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f, true);

    d->AddRectFilled(pos + ImVec2(5.f, 5.f), pos + ImVec2(15.f, size.y - 5.f), value && *value ? ui->style.accent : ui->style.bottom);
    d->AddRect(pos + ImVec2(5.f, 5.f), pos + ImVec2(15.f, size.y - 5.f), outline);
    d->AddRect(pos + ImVec2(6.f, 6.f), pos + ImVec2(14.f, size.y - 6.f), ui->style.shadow);

    d->AddText(pos + ImVec2(30.f, 4.f), is_active ? ui->style.text_normal : ui->style.text_disabled, name.c_str());

    ui->set_current_id(0);

    draw_desc(ui);
}

void c_ui_checkbox::setup(std::string n) {
    name = std::move(n);
    value = new bool(false);
    id = ImHashStr(name.c_str(), 0, rand());
    size = ImVec2(20.f, 20.f);
}
