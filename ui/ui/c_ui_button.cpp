//
// Created by ruppet on 23.01.2020.
//

#include "c_ui.h"
#include "../imgui_internal.h"

void c_ui_button::setup(std::string n) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());

    size = ImVec2(195.f, 25.f);
}

void c_ui_button::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();
    ui->set_current_id(id);

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    // UPDATE
    auto rect = ImRect(pos + ImVec2(30.f, 5.f), pos + size);

    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_held()) {
            if (ui->is_hovered(rect))
                ui->active_id = id;
        } else {
            if (ui->active_id == id) {
                if (callback)
                    callback(this);

                ui->active_id = 0;
            }
        }
    }

    // DRAW
    auto d = ui->get_painting();

    if (ui->is_hovered(rect))
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f);
    else
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f, true);

    d->AddRectFilled(rect.Min, rect.Max, ui->active_id == id ? ui->style.accent : ui->style.bottom);
    d->AddRect(rect.Min, rect.Max, outline);
    d->AddRect(rect.Min + ImVec2(1.f, 1.f), rect.Max - ImVec2(1.f, 1.f), ui->style.shadow);

    d->PushClipRect(rect.Min, rect.Max, true);
    d->AddText(rect.Min + ImVec2(165.f / 2, 20.f / 2) - c_ui::text_size(name) / 2,
            is_active ? ui->style.text_normal : ui->style.text_disabled, name.c_str());
    d->PopClipRect();

    ui->set_current_id(0);

    draw_desc(ui);
}
