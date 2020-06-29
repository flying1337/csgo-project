//
// Created by ruppet on 05.01.2020.
//

#include "../../cheat.h"

void c_skins::load() {
    static auto items = utils->scan(str("client_panorama.dll"), str("E8 ? ? ? ? FF 76 0C 8D 48 04 E8"));

    using fnGetItemSystem = void*(*)();
    static auto get_item_system = (fnGetItemSystem)(items + *(ptr*)(items + 1) + 5);

    auto item_schema = (void*)((ptr)get_item_system() + sizeof(void*));

    load_paintkits(items, item_schema);
}

void c_skins::load_paintkits(ptr items, void* item_schema) {
    using fnGetPaintKitDefinition = c_paintkit*(__thiscall*)(void*, int);
    static auto get_paintkit_definition = (fnGetPaintKitDefinition)(items + *(ptr*)(items + 12) + 16);

    auto vec = (c_nodelist<int, c_paintkit*>*)((ptr)item_schema + (*(ptr*)((ptr)get_paintkit_definition + 10) - 12));
    for (auto i = 0; i <= vec->last_element; i++) {
        auto pk = vec->data[i].value;
        if (pk->id == 9001) continue;

        std::wstring name = csgo->localize->find_safe(pk->item_name.value);
        paintkits.push_back({ pk->id, std::string(name.begin(), name.end()) });
    }
}