//
// Created by ruppet on 22.01.2020.
//

#ifndef uc-sflying-MENU_C_UI_CHECKBOX_H
#define uc-sflying-MENU_C_UI_CHECKBOX_H

#include "c_ui_element.h"

class c_ui_checkbox : public c_ui_element {
public:
    void setup(std::string name) override;
    void setup(std::string name, bool* v);
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_CHECKBOX; }

    void set_var(bool* v) { value = v; }

    bool get_value() { return value ? *value : false; }
    void set_value(bool v) { if (value) *value = v; }

private:
    bool* value = nullptr;

    bool applied = false;

    ImColor outline;
};


#endif //uc-sflying-MENU_C_UI_CHECKBOX_H
