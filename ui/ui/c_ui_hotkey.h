//
// Created by ruppet on 23.01.2020.
//

#ifndef uc-sflying-MENU_C_UI_HOTKEY_H
#define uc-sflying-MENU_C_UI_HOTKEY_H

#include "c_ui_element.h"
class c_ui_hotkey : public c_ui_element {
public:
    void setup(std::string n) override { setup(n, new int(0), new int(0)); }
    void setup(std::string n, int* v) { setup(n, v, nullptr); }
    void setup(std::string n, int* v, int* b);
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_HOTKEY; }

    bool get_state();

    int get_value() { return val ? *val : 0; }
    void set_value(int v) { if (val) *val = v; }

    int get_behaviour() { return beh ? *beh : 0; }
    void set_behaviour(int v) { if (beh) *beh = v; }

    void set_var(int* v) { val = v; }
    void set_beh(int* v) { beh = v; }

private:
    int* val = nullptr;
    int* beh = nullptr;

    ImGuiID beh_id = 0;

    bool clear = false;

    ImColor outline;
    bool applied = false;
};


#endif //uc-sflying-MENU_C_UI_HOTKEY_H
