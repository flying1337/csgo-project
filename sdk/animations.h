//
// Created by ruppet on 23.12.2019.
//

#ifndef uc-sflying-cm-_ANIMATIONS_H
#define uc-sflying-cm-_ANIMATIONS_H

class c_weapon;

class c_animstate {
public:
    void* thisptr;
    char pad2[91];
    c_entity* ent; //0x60
    c_weapon* wep; //0x64
    c_weapon* last_wep; //0x68
    float last_anim_update; //0x6C
    int last_anim_frame; //0x70
    float eye_pitch; //0x74
    float eye_yaw; //0x78
    float pitch; //0x7C
    float abs_yaw; //0x80
    float current_feet_yaw; //0x84
    float current_body_yaw; //0x88
    float moving_lean; //0x8C //changes when moving/jumping/hitting ground
    float lean_amount; //0x90
    char pad4[4]; //NaN
    float feet_cycle; //0x98 0 to 1
    float feet_rate; //0x9C 0 to 1
    float unk;
    float duck_amount; //0xA4
    float landing_bob; //0xA8
    float unk1; //0xAC
    c_vector origin; //0xB0, 0xB4, 0xB8
    c_vector last_origin; //0xBC, 0xC0, 0xC4
    float velocity_x; //0xC8
    float velocity_y; //0xCC
    char pad5[4];
    float unk2; //0xD4 Affected by movement and direction
    char pad6[8];
    float unk3; //0xE0 //from -1 to 1 when moving and affected by direction
    float unk4; //0xE4 //from -1 to 1 when moving and affected by direction
    float unk5; //0xE8
    float speed_2d; //0xEC
    float velocity_z; //0xF0
    float speed; //0xF4 //from 0 to 1
    float feet_speed_sideways; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
    float feet_speed_unk; //0xFC //from 0 to 3. something
    float time_since_started_moving; //0x100
    float time_since_stopped_moving; //0x104
    bool is_on_ground; //0x108
    bool is_in_hit_ground_anim; //0x109
    char pad7[10];
    float last_origin_z; //0x114
    float head_height_on_hit; //0x118 from 0 to 1, is 1 when standing
    float unk123; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
    char pad8[4]; //NaN
    float feet_move_fraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
    char pad9[4]; //NaN
    float unk999;
    char pad10[528];
};

class c_csgo_animstate : public c_animstate {
public:
};

class c_animlayer {
public:
    char  pad_0000[20];
    // These should also be present in the padding, don't see the use for it though
    //float    m_flLayerAnimtime;
    //float    m_flLayerFadeOuttime;
    uint32_t order; //0x0014
    uint32_t sequence; //0x0018
    float_t prev_cycle; //0x001C
    float_t weight; //0x0020
    float_t weight_delta; //0x0024
    float_t playback_rate; //0x0028
    float_t cycle; //0x002C
    void* owner; //0x0030 // player's thisptr
    char  pad_0038[4]; //0x0034
};

#endif //uc-sflying-cm-_ANIMATIONS_H
