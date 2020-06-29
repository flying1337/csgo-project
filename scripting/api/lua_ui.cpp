//
// Created by ruppet on 3/18/2020.
//

#include "../../cheat.h"

#define _s base_state

void c_scripting::ns_ui() {
    auto ui_element = _s.new_usertype<c_ui_element>(str("ui_element"), sol::no_constructor);
    ui_element.set_function(str("set_name"), &c_ui_element::set_name);
    ui_element.set_function(str("get_name"), &c_ui_element::get_name);
    ui_element.set_function(str("set_desc"), &c_ui_element::set_desc);
    //ui_element.set_function(str("get_desc"), &c_ui_element::get_desc);
    ui_element.set_function(str("set_size"), [](c_ui_element* self, float w, float h) {
        self->set_size(ImVec2(w, h));
    });
    //ui_element.set_function(str("get_size"), &c_ui_element::get_size);
    ui_element.set_function(str("set_hidden"), &c_ui_element::set_hidden);
    //ui_element.set_function(str("get_hidden"), &c_ui_element::get_hidden);
    ui_element.set_function(str("set_active"), &c_ui_element::set_active);
    ui_element.set_function(str("get_active"), &c_ui_element::get_active);
    ui_element.set_function(str("set_draw_name"), &c_ui_element::set_draw_name);
    ui_element.set_function(str("set_callback"), &c_ui_element::set_callback);

    auto ui_checkbox = _s.new_usertype<c_ui_checkbox>(str("ui_checkbox"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_checkbox.set_function(str("get_value"), &c_ui_checkbox::get_value);
    ui_checkbox.set_function(str("set_value"), &c_ui_checkbox::set_value);

    auto ui_dropdown = _s.new_usertype<c_ui_dropdown>(str("ui_dropdown"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_dropdown.set_function(str("get_value"), &c_ui_dropdown::get_value);
    ui_dropdown.set_function(str("set_value"), &c_ui_dropdown::set_value);

    auto ui_hotkey = _s.new_usertype<c_ui_hotkey>(str("ui_hotkey"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_hotkey.set_function(str("get_value"), &c_ui_hotkey::get_value);
    ui_hotkey.set_function(str("set_value"), &c_ui_hotkey::set_value);
    ui_hotkey.set_function(str("get_behavior"), &c_ui_hotkey::get_behaviour);
    ui_hotkey.set_function(str("set_behavior"), &c_ui_hotkey::set_behaviour);

    auto ui_listbox = _s.new_usertype<c_ui_listbox>(str("ui_listbox"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_listbox.set_function(str("get_value"), &c_ui_listbox::get_value);
    ui_listbox.set_function(str("set_value"), &c_ui_listbox::set_value);

    auto ui_slider_float = _s.new_usertype<c_ui_slider_float>(str("ui_slider_float"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_slider_float.set_function(str("get_value"), &c_ui_slider_float::get_value);
    ui_slider_float.set_function(str("set_value"), &c_ui_slider_float::set_value);
    ui_slider_float.set_function(str("get_format"), &c_ui_slider_float::get_format);
    ui_slider_float.set_function(str("set_format"), &c_ui_slider_float::set_format);

    auto ui_slider_int = _s.new_usertype<c_ui_slider_int>(str("ui_slider_int"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_slider_int.set_function(str("get_value"), &c_ui_slider_int::get_value);
    ui_slider_int.set_function(str("set_value"), &c_ui_slider_int::set_value);
    ui_slider_int.set_function(str("get_format"), &c_ui_slider_int::get_format);
    ui_slider_int.set_function(str("set_format"), &c_ui_slider_int::set_format);

    auto ui_textbox = _s.new_usertype<c_ui_textbox>(str("ui_textbox"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_textbox.set_function(str("get_value"), &c_ui_textbox::get_value);
    ui_textbox.set_function(str("set_value"), &c_ui_textbox::set_value);

    auto ui_colorpicker = _s.new_usertype<c_ui_colorpicker>(str("ui_colorpicker"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_colorpicker.set_function(str("get_value"), [](c_ui_colorpicker* self) { return self->get_value(); });
    ui_colorpicker.set_function(str("set_value"), [](c_ui_colorpicker* self, uint32 v) { return self->set_value(v); });
    ui_colorpicker.set_function(str("get_alphabar"), &c_ui_colorpicker::get_alphabar);
    ui_colorpicker.set_function(str("set_alphabar"), &c_ui_colorpicker::set_alphabar);

    auto ui_multiselect = _s.new_usertype<c_ui_multiselect>(str("ui_multiselect"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    ui_multiselect.set_function(str("get_value"),
    [](sol::this_state s, c_ui_multiselect* self) {
        std::vector<std::string> result;
        for (auto i = 0; i < self->count(); i++) {
            if (self->get_value(i))
                result.push_back(self->dd_elements.at(i));
        }

        return result;
    });
    ui_multiselect.set_function(str("set_value"),
    [](c_ui_multiselect* self, sol::variadic_args v) {
        for (auto s : v) {
            if (!s.is<std::string>())
                continue;

            for (auto i = 0; i < self->count(); i++) {
                if (self->dd_elements.at(i) == s.as<std::string>())
                    self->set_value(i, !self->get_value(i));
            }
        }
    });

    _s.new_usertype<c_ui_button>(str("ui_button"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());
    _s.new_usertype<c_ui_label>(str("ui_label"), sol::no_constructor, sol::base_classes, sol::bases<c_ui_element>());

    auto ui = _s.create_table();

    ui[str("new_checkbox")] = [](sol::this_state s, g_str tab, g_str group, g_str name, std::optional<bool> def) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_checkbox;
        e->setup(name, new bool(def.value_or(false)));


        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_button")] = [](sol::this_state s, g_str tab, g_str group, g_str name) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_button;
        e->setup(name);

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_label")] = [](sol::this_state s, g_str tab, g_str group, g_str name) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_label;
        e->setup(name);

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_hotkey")] = [](sol::this_state s, g_str tab, g_str group, g_str name) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_hotkey;
        e->setup(name, new int(0), new int(1));

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_slider_int")] = [](sol::this_state s, g_str tab, g_str group, g_str name, int min, int max, std::optional<int> def) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_slider_int;
        e->setup(name, new int(def.value_or(0)), min, max);

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_slider_float")] = [](sol::this_state s, g_str tab, g_str group, g_str name, float min, float max, std::optional<float> def) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_slider_float;
        e->setup(name, new float(def.value_or(0)), min, max);

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_textbox")] = [](sol::this_state s, g_str tab, g_str group, g_str name, int size) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_textbox;
        e->setup(name, new std::string(), size);

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_colorpicker")] = [](sol::this_state s, g_str tab, g_str group, g_str name) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_colorpicker;
        e->setup(name, new float[4]{1, 1, 1, 1});

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_dropdown")] = [](sol::this_state s, g_str tab, g_str group, g_str name, sol::variadic_args va) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_dropdown;
        for (auto v : va)
            e->add(v.as<std::string>());
        e->setup(name, new int(0));

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_multiselect")] = [](sol::this_state s, g_str tab, g_str group, g_str name, sol::variadic_args va) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_multiselect;
        for (auto v : va)
            e->add(v.as<std::string>());
        e->setup(name, new bool[16]{false});

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("new_listbox")] = [](sol::this_state s, g_str tab, g_str group, g_str name, sol::variadic_args va) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto g = menu->wnd->find(tab, group);
        if (!g)
            return sol::make_object(s, sol::nil);

        auto e = new c_ui_listbox;
        for (auto v : va)
            e->add(v.as<std::string>());
        e->setup(name, new int(0));

        g->add(e);
        scripting->current_script->menu_elements.push_back(e);

        return sol::make_object(s, e);
    };

    ui[str("reference")] = [](sol::this_state s, g_str tab, g_str group, g_str name) {
        if (!scripting->current_script)
            return sol::make_object(s, sol::nil);

        auto e = menu->wnd->find(tab, group, name);
        if (!e)
            return sol::make_object(s, sol::nil);

        switch (e->get_control_type())
        {
            case CONTROL_BUTTON:
                return sol::make_object(s, (c_ui_button*)e);
            case CONTROL_CHECKBOX:
                return sol::make_object(s, (c_ui_checkbox*)e);
            case CONTROL_COLORPICKER:
                return sol::make_object(s, (c_ui_colorpicker*)e);
            case CONTROL_DROPDOWN:
                return sol::make_object(s, (c_ui_dropdown*)e);
            case CONTROL_HOTKEY:
                return sol::make_object(s, (c_ui_hotkey*)e);
            case CONTROL_LABEL:
                return sol::make_object(s, (c_ui_label*)e);
            case CONTROL_LISTBOX:
                return sol::make_object(s, (c_ui_listbox*)e);
            case CONTROL_MULTISELECT:
                return sol::make_object(s, (c_ui_multiselect*)e);
            case CONTROL_SLIDER_FLOAT:
                return sol::make_object(s, (c_ui_slider_float*)e);
            case CONTROL_SLIDER_INT:
                return sol::make_object(s, (c_ui_slider_int*)e);
            case CONTROL_TEXTBOX:
                return sol::make_object(s, (c_ui_textbox*)e);
            case CONTROL_NONE:
                return sol::make_object(s, sol::nil);
        }
    };

    _s[str("ui")] = protect(ui, scripting->allow_none);
}

#undef _s