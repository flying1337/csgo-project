//
// Created by ruppet on 22.01.2020.
//

#ifndef uc-sflying-MENU_C_UI_LABEL_H
#define uc-sflying-MENU_C_UI_LABEL_H

#include "c_ui_element.h"

class c_ui_label : public c_ui_element {
public:
    void setup(std::string name) override;
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_LABEL; }
};


#endif //uc-sflying-MENU_C_UI_LABEL_H
