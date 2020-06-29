//
// Created by ruppet on 05.01.2020.
//

#ifndef uc-sflying-cm-_PAINTKIT_H
#define uc-sflying-cm-_PAINTKIT_H

template <typename K, typename V>
class c_node {
public:
    int prev_id;
    int next_id;
    char pad[8];
    K key;
    V value;
};

template <typename K, typename V>
class c_nodelist {
public:
    c_node<K, V>* data;
    int alloc_size;
    int grow_size;
    int start_element;
    int next_available;
    char pad[4];
    int last_element;
};

class c_utlstring {
public:
    char* value;
    unsigned int capacity;
    unsigned int grow_size;
    unsigned int size;
};

class c_paintkit {
public:
    int id;
    c_utlstring name;
    c_utlstring description;
    c_utlstring item_name;
    c_utlstring material_name;
    c_utlstring image_inventory;

    char pad_0x0054[0x8C];
};

#endif //uc-sflying-cm-_PAINTKIT_H
