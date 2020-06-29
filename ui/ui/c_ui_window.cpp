//
// Created by ruppet on 21.01.2020.
//

#include "c_ui.h"
#include "c_ui_window.h"

void c_ui_window::set_parent(c_ui *ui) {
    parent = ui;
}

void c_ui_window::setup(std::string n) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());
    cb_id = ImHashStr((name + "__CB").c_str(), (name + "__CB").size(), rand());
}

void c_ui_window::draw() {
    if (!is_active)
    {
        parent->active_id = 0;
        return;
    }

    std::vector<c_ui_element*> rm;
    for (auto e : elements) {
        if (e->gone)
            rm.push_back(e);
    }

    for (auto r : rm)
        elements.erase(std::find(elements.begin(), elements.end(), r));

    // UPDATE
    pos = ImFloor(pos);
    size = ImFloor(size);

    cb_rect = ImRect(pos + ImVec2(size.x - 20.f, 0.f), pos + ImVec2(size.x, 20.f));

    parent->set_current_id(cb_id);
    if (parent->is_hovered(cb_rect)) {
        if (parent->active_id == cb_id && !parent->is_held()) {
            parent->active_id = 0;
            is_active = false;
        } else {
            if (parent->is_held())
                parent->active_id = cb_id;
        }
    } else {
        if (parent->active_id == cb_id)
            parent->active_id = 0;
    }
    parent->set_current_id(0);

    // DRAW
    auto d = parent->get_painting();

    d->PushClipRect(pos, pos + size, true);
    d->AddRectFilled(pos, pos + size, parent->style.background);

    d->AddRectFilled(pos, pos + ImVec2(size.x, 20.f), parent->style.bottom);
    d->AddText(pos + ImVec2(5.f, 4.f), parent->style.text_normal, name.c_str());

    auto cb_col = parent->style.text_disabled;
    if (parent->is_hovered(cb_rect)) {
        if (parent->is_held())
            cb_col = parent->style.text_normal;
        else
            cb_col = parent->style.text_hovered;
    }

    d->AddLine(cb_rect.Min + ImVec2(5.f, 5.f), cb_rect.Max - ImVec2(5.f, 5.f), cb_col, 2.f);
    d->AddLine(cb_rect.Min + ImVec2(15.f, 5.f), cb_rect.Max - ImVec2(15.f, 5.f), cb_col, 2.f);

    for (auto e : elements) {
        if (e->get_hidden())
            continue;

        e->draw(this);
    }

    d->PopClipRect();

    // POST UPDATE
    parent->set_current_id(id);
    if (parent->active_id == id && is_movable) {
        pos += ImGui::GetIO().MouseDelta;
        if (!parent->is_held())
            parent->active_id = 0;
    }
    else {
        if (parent->is_held()) {
            auto rect = ImRect(pos, pos + size);

            if (!parent->is_hovered(rect))
                is_movable = false;
            else {
                if (is_movable)
                    parent->active_id = id;
            }
        } else
            is_movable = true;
    }
    parent->set_current_id(0);
}

extern bool cmp_strings_ci(const char* a, const char* b);

c_ui_element* find_recursive(const std::string& name, const std::vector<c_ui_element*>& elements) {
    for (auto e : elements) {
        if (e->get_type() == ELEMENT_TYPE_CONTAINER)
            return find_recursive(name, e->elements);
        else {
            if (cmp_strings_ci(e->get_name().c_str(), name.c_str()))
                return e;
        }
    }

    return nullptr;
}

c_ui_element *c_ui_window::find(const std::string& n) {
    return find_recursive(n, elements);
}

c_ui_element *c_ui_window::find(const std::string &tab, const std::string &group, const std::string &n) {
    for (auto t : elements) {
        if (t->is_tab() && cmp_strings_ci(t->get_name().c_str(), tab.c_str())) {
            for (auto g : t->elements) {
                if (g->get_type() == ELEMENT_TYPE_CONTAINER && cmp_strings_ci(g->get_name().c_str(), group.c_str())) {
                    for (auto e : g->elements) {
                        if (cmp_strings_ci(e->get_name().c_str(), n.c_str()))
                            return e;
                    }
                }
            }
        }
    }

    return nullptr;
}

c_ui_element *c_ui_window::find(const std::string &n, const std::string &group) {
    for (auto t : elements) {
        if (t->is_tab() && cmp_strings_ci(t->get_name().c_str(), n.c_str())) {
            for (auto g : t->elements) {
                if (g->get_type() == ELEMENT_TYPE_CONTAINER && cmp_strings_ci(g->get_name().c_str(), group.c_str()))
                    return g;
            }
        }
    }

    return nullptr;
}
