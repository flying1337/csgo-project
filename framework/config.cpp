#include "../cheat.h"
#include "config.h"


using namespace std::chrono_literals;
namespace fs = std::filesystem;

template<typename T>
void bitwrite(std::ofstream& s, T val) {
	auto b = new char[sizeof(T)];
	memcpy(b, &val, sizeof(T));
	s.write(b, sizeof(T));
}

template<typename T>
void bitwrite_ptr(std::ofstream& s, T* val, int size) {
	auto b = new char[size];
	memcpy(b, val, size);
	s.write(b, size);
}

template<typename T>
T bitread(std::ifstream& s) {
	auto b = new char[sizeof(T)];
	s.read(b, sizeof(T));

	T val;
	memcpy(&val, b, sizeof(T));
	return val;
}

template<typename T>
void bitread_ptr(std::ifstream& s, T* ptr, int size) {
	auto b = new char[size];
	s.read(b, size);

	memcpy(ptr, b, size);
}

void c_config::init() {
	o_b = malloc(sizeof(b));
	o_i = malloc(sizeof(i));
	o_f = malloc(sizeof(f));
	o_c = malloc(sizeof(c));
	o_m = malloc(sizeof(m));

	memcpy(o_b, &b, sizeof(b));
	memcpy(o_i, &i, sizeof(i));
	memcpy(o_f, &f, sizeof(f));
	memcpy(o_c, &c, sizeof(c));
	memcpy(o_m, &m, sizeof(m));

	refresh();
}

void c_config::save() {
	if (core->is_config_working) return;
	core->is_config_working = true;

	if (!b.disable_loader) {
		core->loadanim_text = str("Saving config...");
		core->is_loadanim_shown = true;
	}

	char path[MAX_PATH];
	sprintf(path, str("c:/uc-sflying/cfg/csgo_%s.dat"), slots.at(selected_slot).c_str());

	std::ofstream s(path, std::ios::binary);
	if (!s.is_open()) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;

	    menu->wnd->get_parent()->add_message(str("Save failed"), str("Could not open file to write"));
	    return;
	}

	bitwrite(s, '\x13');
	bitwrite(s, '\x37');
	bitwrite(s, cfg_version);
	bitwrite(s, (unsigned int)sizeof(b));
	bitwrite(s, b);
	bitwrite(s, (unsigned int)sizeof(i));
	bitwrite(s, i);
	bitwrite(s, (unsigned int)sizeof(f));
	bitwrite(s, f);
	bitwrite(s, (unsigned int)sizeof(c));
	bitwrite(s, c);
	bitwrite(s, (unsigned int)sizeof(m));
	bitwrite(s, m);
	
	s.close();

	core->is_config_working = false;
	core->is_loadanim_shown = false;

    menu->wnd->get_parent()->add_message(str("Saved"), str("Config has been saved"));
}

void c_config::load() {
	if (core->is_config_working) return;
	core->is_config_working = true;

	if (!b.disable_loader) {
		core->loadanim_text = str("Loading config...");
		core->is_loadanim_shown = true;
	}

	char path[MAX_PATH];
	sprintf(path, str("c:/uc-sflying/cfg/csgo_%s.dat"), slots.at(selected_slot).c_str());

	std::ifstream s(path, std::ios::binary);
	if (!s.is_open()) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Could not open file to read"));
        return;
	}

	s.seekg(0, s.end);
	auto len = s.tellg();
	s.seekg(0, s.beg);

	if (!len || len < 6) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Config file is corrupted"));
        return;
	}

	char magic[2];
	magic[0] = bitread<char>(s);
	magic[1] = bitread<char>(s);

	if (magic[0] != '\x13' || magic[1] != '\x37') {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Config file is incompatible"));
        return;
	}

	auto version = bitread<int>(s);
	if (version > cfg_version) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Config file is made in newer version of the cheat"));
        return;
	}

	auto b_size = bitread<unsigned int>(s);
	if (b_size > sizeof(b)) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Config file is corrupted"));

        memcpy(&b, o_b, sizeof(b));
        memcpy(&i, o_i, sizeof(i));
        memcpy(&f, o_f, sizeof(f));
        memcpy(&c, o_c, sizeof(c));
        memcpy(&m, o_m, sizeof(m));
	    return;
	}

	bitread_ptr(s, &b, b_size);

	auto i_size = bitread<unsigned int>(s);
	if (i_size > sizeof(i)) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Config file is corrupted"));

        memcpy(&b, o_b, sizeof(b));
        memcpy(&i, o_i, sizeof(i));
        memcpy(&f, o_f, sizeof(f));
        memcpy(&c, o_c, sizeof(c));
        memcpy(&m, o_m, sizeof(m));
        return;
	}

    bitread_ptr(s, &i, i_size);

	auto f_size = bitread<unsigned int>(s);
	if (f_size > sizeof(f)) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Config file is corrupted"));

        memcpy(&b, o_b, sizeof(b));
        memcpy(&i, o_i, sizeof(i));
        memcpy(&f, o_f, sizeof(f));
        memcpy(&c, o_c, sizeof(c));
        memcpy(&m, o_m, sizeof(m));
        return;
	}

    bitread_ptr(s, &f, f_size);

	auto c_size = bitread<unsigned int>(s);
	if (c_size > sizeof(c)) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Config file is corrupted"));

        memcpy(&b, o_b, sizeof(b));
        memcpy(&i, o_i, sizeof(i));
        memcpy(&f, o_f, sizeof(f));
        memcpy(&c, o_c, sizeof(c));
        memcpy(&m, o_m, sizeof(m));
        return;
	}

    bitread_ptr(s, &c, c_size);

	auto m_size = bitread<unsigned int>(s);
	if (m_size > sizeof(m)) {
	    core->is_config_working = false;
	    core->is_loadanim_shown = false;
        menu->wnd->get_parent()->add_message(str("Load failed"), str("Config file is corrupted"));

        memcpy(&b, o_b, sizeof(b));
        memcpy(&i, o_i, sizeof(i));
        memcpy(&f, o_f, sizeof(f));
        memcpy(&c, o_c, sizeof(c));
        memcpy(&m, o_m, sizeof(m));
        return;
	}

    bitread_ptr(s, &m, m_size);

	s.close();

    if (menu->wnd) {
        for (auto e : menu->wnd->elements) {
            if (e->is_tab()) {
                for (auto te : e->elements) {
                    if (te->get_type() == ELEMENT_TYPE_CONTAINER) {
                        for (auto ce : te->elements) {
                            if (ce->get_control_type() != CONTROL_BUTTON)
                                ce->call_callback();
                        }
                    } else {
                        if (te->get_control_type() != CONTROL_BUTTON)
                            te->call_callback();
                    }
                }
            } else {
                if (e->get_type() == ELEMENT_TYPE_CONTAINER) {
                    for (auto ce : e->elements) {
                        if (ce->get_control_type() != CONTROL_BUTTON)
                            ce->call_callback();
                    }
                } else {
                    if (e->get_control_type() != CONTROL_BUTTON)
                        e->call_callback();
                }
            }
        }
    }

	core->is_config_working = false;
	core->is_loadanim_shown = false;

	features->speclist->did_set_pos = false;

    menu->wnd->get_parent()->add_message(str("Loaded"), str("Config has been loaded"));
}

void c_config::reset() {
	if (core->is_config_working) return;
	core->is_config_working = true;

	if (!b.disable_loader) {
		core->loadanim_text = str("Resetting config...");
		core->is_loadanim_shown = true;
	}

	memcpy(&b, o_b, sizeof(b));
	memcpy(&i, o_i, sizeof(i));
	memcpy(&f, o_f, sizeof(f));
	memcpy(&c, o_c, sizeof(c));
	memcpy(&m, o_m, sizeof(m));

    if (menu->wnd) {
        for (auto e : menu->wnd->elements) {
            if (e->is_tab()) {
                for (auto te : e->elements) {
                    if (te->get_type() == ELEMENT_TYPE_CONTAINER) {
                        for (auto ce : te->elements) {
                            if (ce->get_control_type() != CONTROL_BUTTON)
                                ce->call_callback();
                        }
                    } else {
                        if (te->get_control_type() != CONTROL_BUTTON)
                            te->call_callback();
                    }
                }
            } else {
                if (e->get_type() == ELEMENT_TYPE_CONTAINER) {
                    for (auto ce : e->elements) {
                        if (ce->get_control_type() != CONTROL_BUTTON)
                            ce->call_callback();
                    }
                } else {
                    if (e->get_control_type() != CONTROL_BUTTON)
                        e->call_callback();
                }
            }
        }
    }

	core->is_config_working = false;
	core->is_loadanim_shown = false;

    menu->wnd->get_parent()->add_message(str("Reset"), str("Config has been reset"));
}

extern bool sub_strings_ci(const char* a, const char* b);

void c_config::refresh() {
    slots.clear();

    for (auto d : fs::directory_iterator(str("c:/uc-sflying/cfg"))) {
        if (d.is_regular_file() &&
            d.path().extension() == str(".dat") &&
            sub_strings_ci(d.path().filename().string().c_str(), str("csgo_"))) {

            slots.push_back(d.path().filename().replace_extension(str("")).string().substr(5));
        }
    }

    if (menu->wnd && menu->is_initialized) {
        auto presets = (c_ui_listbox*)menu->wnd->find(str("Miscellaneous"), str("Presets"), str("Presets"));
        if (presets) {
            presets->remove_all();

            for (auto s : slots)
                presets->add(s);

            presets->call_callback();
        }
    }
}

void c_config::save_skins() {
    if (core->is_config_working) return;
    core->is_config_working = true;

    char path[MAX_PATH];
    sprintf(path, str("c:/uc-sflying/cfg/_csgo_skins.dat"));

    std::ofstream s(path, std::ios::binary);
    if (!s.is_open()) {
        core->is_config_working = false;
        core->is_loadanim_shown = false;

        menu->wnd->get_parent()->add_message(str("Save failed"), str("Could not open file to write"));
        return;
    }

    bitwrite(s, '\xFA');
    bitwrite(s, '\xCC');
    bitwrite(s, cfg_version);

    auto size = paintkits.size() * sizeof(c_config_paintkit);
    bitwrite(s, size);

    auto buffer = new char[size];
    memset(buffer, 0, size);

    for (auto n = 0, k = 0; n < size; n += sizeof(c_config_paintkit), k++)
        memcpy(buffer + n, &paintkits[k], sizeof(c_config_paintkit));

    bitwrite_ptr(s, buffer, size);

    s.close();

    core->is_config_working = false;
    menu->wnd->get_parent()->add_message(str("Saved"), str("Config has been saved"));
}

void c_config::load_skins() {

}

void c_config::reset_skins() {
    paintkits.clear();
    menu->wnd->get_parent()->add_message(str("Reset"), str("Config has been reset"));
}
