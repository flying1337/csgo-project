//
// Created by ruppet on 04.01.2020.
//

#ifndef uc-sflying-cm-_CLIENTSTATE_H
#define uc-sflying-cm-_CLIENTSTATE_H

class c_clientstate {
    class c_clock_drift_manager {
    public:
        float clock_offsets[17];
        unsigned int cur_clock_offset;
        unsigned int server_tick;
        unsigned int client_tick;
    };

public:
    char pad_0000[0x9C];
    c_netchannel* netchannel;
    unsigned int challenge_nr;
    char pad_00A4[0x64];
    unsigned int signon_state;
    char pad_010C[0x8];
    float next_cmd_time;
    unsigned int server_count;
    unsigned int current_sequence;
    char pad_0120[4];
    c_clock_drift_manager clock_drift_mgr;
    int delta_tick;
    bool is_paused;
    char pad_0179[7];
    unsigned int view_entity;
    unsigned int player_slot;
    char level_name[260];
    char level_name_short[80];
    char group_name[80];
    char pad_032C[92];
    unsigned int max_clients;
    char pad_0314[18820];
    float last_server_tick_time;
    bool in_simulation;
    char pad_4C9D[3];
    unsigned int oldtickcount;
    float tick_reminder;
    float frametime;
    unsigned int last_command;
    unsigned int choked_commands;
    unsigned int last_command_ack;
    unsigned int command_ack;
    unsigned int sound_sequence;
    char pad_4CC0[80];
    c_angle viewangles;
    char pad_4D14[0xD0];
    c_eventinfo* events;
};

#endif //uc-sflying-cm-_CLIENTSTATE_H
