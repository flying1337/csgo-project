//
// Created by ruppet on 2/5/2020.
//

#ifndef uc-sflying-LOADER_C_UI_PRODUCTS_H
#define uc-sflying-LOADER_C_UI_PRODUCTS_H

#include "c_ui_element.h"
#include "../../core/subscription.h"

class c_ui_products : public c_ui_element {
public:
    void setup(std::string n) override;
    void setup(std::string n, int* v, IDirect3DDevice9* device);
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_LISTBOX; }

    void add(const c_subscription& t) { dd_elements.push_back(t); }
    void remove_all() { dd_elements.clear(); }
    int count() { return dd_elements.size(); }

    int get_value() { return val ? *val : 0; }
    void set_value(int v) { if (val) *val = v; }

    void set_var(int* v) { val = v; }

private:
    int* val = nullptr;

    float scroll = 0.f;

    std::vector<c_subscription> dd_elements;
    std::unordered_map<int, IDirect3DTexture9*> icons;
};


#endif //uc-sflying-LOADER_C_UI_PRODUCTS_H
