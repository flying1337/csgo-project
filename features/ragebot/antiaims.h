//
// Created by ruppet on 24.12.2019.
//

#ifndef uc-sflying-cm-antiaims-h
#define uc-sflying-cm-antiaims-h


class c_movfix {
public:
    float forwardmove;
    float sidemove;
    c_angle ang;

    void rotate();
};

struct player_animation_t
{
    float m_last_simtime = 0.f;
    float m_spawntime = 0.f;
    bool m_init = false;
    std::array< c_animlayer, 13 > m_layers{ };
    std::array< float, 20 > m_poses{ };
    c_csgo_animstate* m_state = nullptr;
    bool m_got_matrix = false;
    c_matrix3x4 m_matrix[128] {};
    bool m_should_recalculate_matrix = false;
};

class c_antiaims {
public:
    void work(c_usercmd* c, float& sidemove, float& forwardmove);
    void update_body();
    void player_animation();

    std::array< player_animation_t, 64 > m_player_states;
    bool m_should_update_entity_animstate = true;
    c_csgo_animstate* animstate = nullptr;

    void sidemove();

    int skip = 0;
    int override = 0;
    bool shoulduse = false;

    int code = 0;
    int key = 0;
    std::string binding = "";

    c_angle last_ang;
private:
    void set(c_usercmd* c, int s);
    void update_lby();


    float spawntime = 0.f;
    c_ehandle* selfhandle;
    float simtime = 0.f;

    c_csgo_animstate* animstate_lby = nullptr;
    float spawntime_lby = 0.f;
    c_ehandle* selfhandle_lby;

    c_angle orig;
};

#endif //uc-sflying-cm-antiaims-h
