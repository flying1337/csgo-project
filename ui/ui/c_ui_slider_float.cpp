//
// Created by ruppet on 22.01.2020.
//

#include "c_ui.h"

void c_ui_slider_float::setup(std::string n) {
    setup(n, new float(0.f), 0, 100);
}

void c_ui_slider_float::setup(std::string n, float* v, float mi, float ma, std::string f) {
    name = std::move(n);
    fmt = std::move(f);

    val = v;
    min = mi;
    max = ma;

    size = ImVec2(195.f, 40.f);

    id = ImHashStr(name.c_str(), name.size(), rand());

    minus = ImHashStr((name + "__MINUS").c_str(), (name + "__MINUS").size(), rand());
    plus = ImHashStr((name + "__PLUS").c_str(), (name + "__PLUS").size(), rand());
}

void c_ui_slider_float::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    if (draw_name)
        size.y = 40.f;
    else
        size.y = 20.f;

    ui->set_current_id(id);

    if (val)
        *val = std::clamp(*val, min, max);

    // UPDATE
    auto text_area = ImRect(pos + ImVec2(30.f, 4.f), pos + size - ImVec2(0.f, 20.f));
    auto slider_area = ImRect(pos + ImVec2(30.f, draw_name ? 20.f : 0.f), pos + size);

    auto old_value = val ? *val : 0.f;

    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_held()) {
            if (ui->is_hovered(slider_area))
                ui->active_id = id;

            if (pm && val && ui->active_id != id) {
                ui->set_current_id(minus);
                if (ui->is_hovered(ImRect(slider_area.Min - ImVec2(10.f, 0.f), slider_area.Min - ImVec2(1.f, -20.f))))
                    ui->active_id = minus;
                ui->set_current_id(plus);
                if (ui->is_hovered(ImRect(slider_area.Max + ImVec2(1.f, -20.f), slider_area.Max + ImVec2(10.f, 0.f))))
                    ui->active_id = plus;
            }
        } else {
            if (ui->active_id == id)
                ui->active_id = 0;

            if (pm && val) {
                ui->set_current_id(minus);
                if (!ui->is_held()) {
                    ui->set_current_id(plus);
                    if (!ui->is_held()) {
                        if (ui->active_id == minus) {
                            (*val)--;
                            ui->active_id = 0;
                        }

                        if (ui->active_id == plus) {
                            (*val)++;
                            ui->active_id = 0;
                        }

                        *val = std::clamp(*val, min, max);
                    }
                }
            }
        }
    }

    if (ui->active_id == id && val && ui->is_held()) {
        auto m_on_track = std::clamp(ImGui::GetIO().MousePos.x - slider_area.Min.x, 0.f, 165.f);

        *val = m_on_track / 165.f * ((float)max - (float)min) + min;
        *val = std::clamp(*val, min, max);
    } else {
        if (ui->active_id == id)
            ui->active_id = 0;
    }

    if (val && old_value != *val && callback)
        callback(this);

    // DRAW
    auto d = ui->get_painting();

    if (ui->is_hovered(slider_area))
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f);
    else
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f, true);

    if (draw_name)
        d->AddText(text_area.Min, is_active ? ui->style.text_normal : ui->style.text_disabled, name.c_str());

    auto slider_w = (slider_area.Max - slider_area.Min).x;
    auto slider_h = (slider_area.Max - slider_area.Min).y;

    d->AddRectFilled(slider_area.Min + ImVec2(0.f, 5.f), slider_area.Max - ImVec2(0.f, 5.f), ui->style.bottom);

    if (val)
        d->AddRectFilled(slider_area.Min + ImVec2(0.f, 5.f),
                         slider_area.Min + ImVec2(((float)(*val) - (float)min) / ((float)max - (float)min) * slider_w, slider_h - 5.f), ui->style.accent);

    d->AddRect(slider_area.Min + ImVec2(0.f, 5.f), slider_area.Max - ImVec2(0.f, 5.f), outline);
    d->AddRect(slider_area.Min + ImVec2(1.f, 6.f), slider_area.Max - ImVec2(1.f, 6.f), ui->style.shadow);

    if (pm && val) {
        if (*val != min)
            d->AddText(slider_area.Min - ImVec2(9.f, -3.f), ui->style.text_disabled, "-");
        if (*val != max)
            d->AddText(slider_area.Max + ImVec2(2.f, -17.f), ui->style.text_disabled, "+");
    }

    if (val) {
        c_ui::set_font(ui->fonts.main_bold);

        char buffer[256] = {};
        ImFormatString(buffer, 256, fmt.c_str(), *val);

        auto text_size = c_ui::text_size(buffer);
        auto text_pos_x = ((float)(*val) - (float)min) / ((float)max - (float)min) * slider_w - text_size.x / 2;

        d->AddText(slider_area.Min + ImVec2(text_pos_x - 1, 2.f),
                   ImColor(0, 0, 0, 100), buffer);
        d->AddText(slider_area.Min + ImVec2(text_pos_x, 2.f),
                   ImColor(0, 0, 0, 100), buffer);
        d->AddText(slider_area.Min + ImVec2(text_pos_x + 1, 2.f),
                   ImColor(0, 0, 0, 100), buffer);
        d->AddText(slider_area.Min + ImVec2(text_pos_x - 1, 4.f),
                   ImColor(0, 0, 0, 100), buffer);
        d->AddText(slider_area.Min + ImVec2(text_pos_x, 4.f),
                   ImColor(0, 0, 0, 100), buffer);
        d->AddText(slider_area.Min + ImVec2(text_pos_x + 1, 4.f),
                   ImColor(0, 0, 0, 100), buffer);

        d->AddText(slider_area.Min + ImVec2(text_pos_x, 3.f),
                   ui->style.text_normal, buffer);

        c_ui::set_font(nullptr);
    }

    ui->set_current_id(0);

    draw_desc(ui);
}
