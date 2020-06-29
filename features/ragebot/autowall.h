//
// Created by ruppet on 25.12.2019.
//

#ifndef uc-sflying-cm-_AUTOWALL_H
#define uc-sflying-cm-_AUTOWALL_H

struct s_awinfo {
    int damage = -1;
    int hitgroup = -1;
    int walls = 4;
    bool did_penetrate_wall = false;
    float tickness = 1.f;
    c_player* player = 0;
    c_vector end;
};

struct s_awbullet {
    c_vector start;
    c_vector end;
    c_vector pos;
    c_vector dir;

    c_tracefilter* filter = nullptr;
    c_trace trace;

    float thickness = 1.f;
    float damage = 1.f;
    int walls = 4;
};

class c_autowall {
public:
    float get_approx_point_damage(c_vector point);
    float get_point_damage(c_vector point, c_player* player);
    s_awinfo work(c_vector start, c_vector end, c_player* from, c_player* to);
private:
    void scale_damage(c_trace* trace, c_weaponinfo* inf, int& hitgroup, float& damage);
    void clip_trace_to_player(c_vector start, c_vector end, c_player* player, unsigned int mask, c_tracefilter* filter, c_trace* trace);
    bool handle_bullet_penetration(c_weaponinfo* inf, s_awbullet& bullet);
    bool trace_to_exit(c_trace* enter_trace, c_vector& start, c_vector& dir, c_trace* exit_trace);
    bool is_breakable(c_player* player);
};

#endif //uc-sflying-cm-_AUTOWALL_H
