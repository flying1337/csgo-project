//
// Created by ruppet on 3/14/2020.
//

#include "../../cheat.h"

void c_menu::extensions_tab(c_ui_tab* extensions) {
    static auto ext_loader = new c_ui_group;
    ext_loader->setup(str("Loader"));
    ext_loader->set_size(ImVec2(240.f, 420.f));
    {
        static auto selected_script = 0;

        static auto toggle = new c_ui_button;
        toggle->setup(str("Load"));
        toggle->set_callback([&](c_ui_element*) {
            if (scripting->scripts.empty()) return;

            auto s = &scripting->scripts.at(selected_script);
            scripting->load_queue.push_back(s->name);
        });

        static auto unload = new c_ui_button;
        unload->setup(str("Unload"));
        unload->set_callback([&](c_ui_element*) {
            if (scripting->scripts.empty()) return;

            auto s = &scripting->scripts.at(selected_script);
            scripting->unload(*s);
        });

        static auto scripts = new c_ui_listbox;
        scripts->setup(str("Loader listbox"), &selected_script);
        scripts->set_size(ImVec2(195.f, 160.f));

        static auto refresh = new c_ui_button;
        refresh->setup(str("Refresh"));
        refresh->set_callback([&](c_ui_element*) {
            scripting->refresh();
            scripts->remove_all();

            for (auto& s : scripting->scripts)
                scripts->add(s.name);
        });

        ext_loader->add(scripts);
        ext_loader->add(refresh);
        ext_loader->add(toggle);
        ext_loader->add(unload);
    }

    static auto ext_1 = new c_ui_group;
    ext_1->setup(str("Elements 1"));
    ext_1->set_size(ImVec2(240.f, 420.f));
    ext_1->set_pos(ImVec2(260.f, 0.f));
    {

    }

    static auto ext_2 = new c_ui_group;
    ext_2->setup(str("Elements 2"));
    ext_2->set_size(ImVec2(240.f, 420.f));
    ext_2->set_pos(ImVec2(520.f, 0.f));
    {

    }

    extensions->add(ext_loader);
    extensions->add(ext_1);
    extensions->add(ext_2);
}