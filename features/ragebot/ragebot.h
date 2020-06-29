//
// Created by ruppet on 23.12.2019.
//

#ifndef uc-sflying-cm-_RAGEBOT_H
#define uc-sflying-cm-_RAGEBOT_H

struct s_bullet {
    c_vector point;
    bool hittable = false;
    c_player* player;
    int damage = -1;
    int hitgroup = 0;
    int hitbox = 0;
    int hc = 0;
    bool is_center = false;
    bool is_visible = false;
    c_matrix3x4 bones[128];
    int tick;
    c_lag_record lc;
};

struct s_scanctx {
    s_bullet result{};
    c_player* p = 0;
};

class c_ragebot {
public:
    void work(c_usercmd* c);


    void cock();

    s_bullet scan(c_player* p);
    s_shot last_shot{};

    bool is_able_to_shoot();

    bool should_stop = false;
    bool delay_shot = false;
    bool did_shot = false;
    bool wants_to_shoot = false;
    bool having_revolver = false;

    float fire_time = 0.f;
    float stop_target = 0.f;
    float stop_check = 5.f;

    c_weapon* wep;
private:
    std::vector<c_player*> get_players();
    void aim(s_bullet bi);
    bool hitchance(c_angle dir, c_player* p, s_bullet bullet);
    void generate_multipoints(std::vector<s_bullet>& bullets, int hitbox, c_matrix3x4 bones[128], c_player* p);
    int preference_to_hitgroup(int pref);

    c_usercmd* cmd;
    int wep_cfg = 0;
    bool use_def = false;

    s_scanctx ctx[65];
    ThreadHandle_t wait_objects[65];

    std::vector<s_bullet> bullets_ctx;
    std::vector<ThreadHandle_t> bullets_wo;
};

#endif //uc-sflying-cm-_RAGEBOT_H
