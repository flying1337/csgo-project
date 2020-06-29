//
// Created by ruppet on 05.01.2020.
//

#ifndef uc-sflying-cm-_SKINS_H
#define uc-sflying-cm-_SKINS_H

class c_skin {
public:
    int id;
    std::string name;
};

class c_skins {
public:
    void load();
    void load_paintkits(ptr items, void* item_schema);

    std::vector<c_skin> paintkits;
};

#endif //uc-sflying-cm-_SKINS_H
