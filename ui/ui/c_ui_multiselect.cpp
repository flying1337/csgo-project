//
// Created by ruppet on 23.01.2020.
//

#include "c_ui.h"

void c_ui_multiselect::setup(std::string n) {
    setup(n, new bool[16]{false});
}

void c_ui_multiselect::setup(std::string n, bool *v) {
    name = std::move(n);
    id = ImHashStr(name.c_str(), name.size(), rand());

    val = v;
    size = ImVec2(195.f, 40.f);
}

void c_ui_multiselect::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();
    ui->set_current_id(id);

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    // UPDATE
    if (clear) {
        ui->active_id = 0;
        clear = false;
    }

    if (draw_name)
        size.y = 40.f;
    else
        size.y = 20.f;

    auto label_rect = ImRect(pos + ImVec2(30.f, 4.f), pos + size - ImVec2(0.f, 20.f));
    auto control_rect = ImRect(pos + ImVec2(30.f, draw_name ? 20.f : 0.f), pos + size);
    auto select_rect = ImRect(
            control_rect.Max - ImVec2(165.f, 0.f) + ImVec2(0.f, 2.f),
            control_rect.Max + ImVec2(0.f, 20.f * dd_elements.size() + 2.f));

    auto old_value = val ? *val : 0;
    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_hovered(control_rect)) {
            if (ui->is_clicked())
                ui->active_id = ui->active_id == id ? 0 : id;
        }
    }

    if (ui->active_id == id && ui->is_clicked() && !ui->is_hovered(control_rect)) {
        auto hovered_any = false;

        auto i = 0;
        for (const auto& dd : dd_elements) {
            auto rect = ImRect(select_rect.Min + ImVec2(0.f, 20.f * i), select_rect.Min + ImVec2(165.f, 20.f * i + 19.f));

            if (ui->is_hovered(rect)) {
                val[i] = !val[i];
                hovered_any = true;
            }

            i++;
        }

        if (!hovered_any)
            clear = true;
    }

    if (val && old_value != *val && callback)
        callback(this);

    // DRAW
    auto d = ui->get_painting();

    if (draw_name) {
        d->PushClipRect(label_rect.Min, label_rect.Max, true);
        d->AddText(label_rect.Min, is_active ? ui->style.text_normal : ui->style.text_disabled, name.c_str());
        d->PopClipRect();
    }

    if (ui->is_hovered(control_rect))
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f);
    else
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f, true);

    d->AddRectFilled(control_rect.Min, control_rect.Max, ui->style.bottom);
    d->AddRect(control_rect.Min, control_rect.Max, outline);
    d->AddRect(control_rect.Min + ImVec2(1.f, 1.f), control_rect.Max - ImVec2(1.f, 1.f), ui->style.shadow);

    d->PushClipRect(control_rect.Min, control_rect.Max - ImVec2(12.f, 0.f), true);
    if (!val || dd_elements.empty())
        d->AddText(control_rect.Min + ImVec2(5.f, 4.f), ui->style.text_disabled, "-");
    else {
        std::string sel;
        for (auto i = 0; i < 16; i++) {
            if (val[i])
                sel += (sel == "" ? dd_elements.at(i) : (", " + dd_elements.at(i)));
        }

        auto bottom_noalpha = ui->style.bottom;
        bottom_noalpha.Value.w = 0.f;

        d->AddText(control_rect.Min + ImVec2(5.f, 4.f), ui->style.text_normal, sel.c_str());
        d->AddRectFilledMultiColor(control_rect.Max - ImVec2(52.f, control_rect.GetHeight() - 2), control_rect.Max - ImVec2(12.f, 2.f),
                                   bottom_noalpha, ui->style.bottom, ui->style.bottom, bottom_noalpha);
    }
    d->PopClipRect();

    if (dd_elements.size() > 1) {
        auto t_base = control_rect.Max - ImVec2(10.f, 9.f);

        if (ui->active_id != id)
            d->AddTriangleFilled(t_base - ImVec2(0.f, 3.f), t_base + ImVec2(2.f, 1.f), t_base + ImVec2(4.f, -3.f), ui->style.text_normal);
        else
            d->AddTriangleFilled(t_base - ImVec2(0.f, -1.f), t_base + ImVec2(2.f, -3.f), t_base + ImVec2(4.f, 1.f), ui->style.text_normal);
    }

    if (ui->active_id == id && val && !clear) {
        auto f = &ImGui::GetCurrentContext()->ForegroundDrawList;

        f->PushClipRect(select_rect.Min - ImVec2(1.f, 1.f), select_rect.Max, true);
        f->AddRectFilled(select_rect.Min, select_rect.Max, ui->style.bottom);

        auto i = 0;
        for (const auto& dd : dd_elements) {
            auto rect = ImRect(select_rect.Min + ImVec2(0.f, 20.f * i), select_rect.Min + ImVec2(165.f, 20.f * i + 19.f));

            if (ui->is_hovered(rect))
                f->AddRectFilled(rect.Min, rect.Max, ui->style.background_hover);

            if (val[i]) {
                c_ui::set_font(ui->fonts.main_bold);
                f->AddText(rect.Min + ImVec2(5.f, 4.f), ui->style.accent, dd.c_str());
                c_ui::set_font(nullptr);
            } else
                f->AddText(rect.Min + ImVec2(5.f, 4.f), ui->style.text_normal, dd.c_str());

            i++;
        }

        f->AddRect(select_rect.Min, select_rect.Max, ui->style.outline);
        f->AddRect(select_rect.Min + ImVec2(1.f, 1.f), select_rect.Max - ImVec2(1.f, 1.f), ui->style.shadow);
        f->PopClipRect();
    }

    ui->set_current_id(0);

    draw_desc(ui);
}
