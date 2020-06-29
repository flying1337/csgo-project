#include "../../cheat.h"
#include <DirectXMath.h>
#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define M_RADPI 57.295779513082f
#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)
#define M_PI 3.14159265358979323846

void RotateMovement(c_usercmd* cmd, float yaw)
{
    c_angle viewangles;
    csgo->engine->get_view_angles(&viewangles);

    float rotation = DEG2RAD(viewangles.y - yaw);

    float cos_rot = cos(rotation);
    float sin_rot = sin(rotation);

    float new_forwardmove = cos_rot * cmd->forwardmove - sin_rot * cmd->sidemove;
    float new_sidemove = sin_rot * cmd->forwardmove + cos_rot * cmd->sidemove;

    cmd->forwardmove = new_forwardmove;
    cmd->sidemove = new_sidemove;
}

void vector_angles(const c_vector& forward, c_angle& angles)
{
    float tmp, yaw, pitch;

    if(forward[1] == 0 && forward[0] == 0) {
        yaw = 0;
        if(forward[2] > 0)
            pitch = 270;
        else
            pitch = 90;
    } else {
        yaw = (atan2(forward[1], forward[0]) * 180 / 3.141592654f);
        if(yaw < 0)
            yaw += 360;

        tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
        pitch = (atan2(-forward[2], tmp) * 180 / 3.141592654f);
        if(pitch < 0)
            pitch += 360;
    }

    angles[0] = pitch;
    angles[1] = yaw;
    angles[2] = 0;
}

template<class T, class U>
static T clamps(T in, U low, U high)
{
    if (in <= low)
        return low;

    if (in >= high)
        return high;

    return in;
}

float normalize_yaw(float value)
{
    while (value > 180)
        value -= 360.f;

    while (value < -180)
        value += 360.f;

    return value;
}

void c_autojump::work(c_usercmd* c) {
    if (!csgo->local_player) return;

	if (config->b.misc_autojump)
		autojump(c);
	if (config->b.misc_airstrafe && !csgo->want_sp && !keymanager->is_key_down(VK_SHIFT))
		airstrafe(c);
}

void c_autojump::autojump(c_usercmd* c) {
    is_hopping = (c->buttons & IN_JUMP) || !(csgo->local_player->flags() & FL_ONGROUND);
	if ((c->buttons & IN_JUMP) && !(csgo->local_player->flags() & FL_ONGROUND))
		c->buttons &= ~IN_JUMP;
}

void c_autojump::airstrafe(c_usercmd* m_pcmd) {
	if (csgo->local_player->flags() & FL_ONGROUND) return;
	if (csgo->local_player->movetype() == MOVETYPE_LADDER || csgo->local_player->movetype() == MOVETYPE_NOCLIP) return;
    static auto cl_sidespeed = csgo->cvar->find_var(str("cl_sidespeed"));
    auto side_speed = cl_sidespeed->get_float();
	if (config->b.misc_wasdmove)
	{
        static auto old_yaw = 0.0f;

        auto get_velocity_degree = [](float velocity)
        {
            auto tmp = RAD2DEG(atan(30.0f / velocity));

            if (CheckIfNonValidNumber(tmp) || tmp > 90.0f)
                return 90.0f;

            else if (tmp < 0.0f)
                return 0.0f;
            else
                return tmp;
        };

        if (csgo->local_player->movetype() != MOVETYPE_WALK)
            return;

        auto velocity = csgo->local_player->velocity();
        velocity.z = 0.0f;

        auto forwardmove = m_pcmd->forwardmove;
        auto sidemove = m_pcmd->sidemove;

        if (velocity.length_2d() < 5.0f && !forwardmove && !sidemove)
            return;

        static auto flip = false;
        flip = !flip;

        auto turn_direction_modifier = flip ? 1.0f : -1.0f;
        auto viewangles = m_pcmd->viewangles;

        if (forwardmove || sidemove)
        {
            m_pcmd->forwardmove = 0.0f;
            m_pcmd->sidemove = 0.0f;

            auto turn_angle = atan2(-sidemove, forwardmove);
            viewangles.y += turn_angle * M_RADPI;
        }
        else if (forwardmove) //-V550
            m_pcmd->forwardmove = 0.0f;

        auto strafe_angle = RAD2DEG(atan(15.0f / velocity.length_2d()));

        if (strafe_angle > 90.0f)
            strafe_angle = 90.0f;
        else if (strafe_angle < 0.0f)
            strafe_angle = 0.0f;

        auto temp = c_vector(0.0f, viewangles.y - old_yaw, 0.0f);
        temp.y = normalize_yaw(temp.y);

        auto yaw_delta = temp.y;
        old_yaw = viewangles.y;

        auto abs_yaw_delta = fabs(yaw_delta);

        if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 30.0f)
        {
            c_angle velocity_angles;
            vector_angles(velocity, velocity_angles);

            temp = c_vector(0.0f, viewangles.y - velocity_angles.y, 0.0f);
            temp.y = normalize_yaw(temp.y);

            auto velocityangle_yawdelta = temp.y;
            auto velocity_degree = get_velocity_degree(velocity.length_2d()) * 0.01f;

            if (velocityangle_yawdelta <= velocity_degree || velocity.length_2d() <= 15.0f)
            {
                if (-velocity_degree <= velocityangle_yawdelta || velocity.length_2d() <= 15.0f)
                {
                    viewangles.y += strafe_angle * turn_direction_modifier;
                    m_pcmd->sidemove = side_speed * turn_direction_modifier;
                }
                else
                {
                    viewangles.y = velocity_angles.y - velocity_degree;
                    m_pcmd->sidemove = side_speed;
                }
            }
            else
            {
                viewangles.y = velocity_angles.y + velocity_degree;
                m_pcmd->sidemove = -side_speed;
            }
        }
        else if (yaw_delta > 0.0f)
            m_pcmd->sidemove = -side_speed;
        else if (yaw_delta < 0.0f)
            m_pcmd->sidemove = side_speed;

        auto move = c_vector(m_pcmd->forwardmove, m_pcmd->sidemove, 0.0f);
        auto speed = move.length();

        c_angle angles_move;
        vector_angles(move, angles_move);

        auto normalized_x = fmod(m_pcmd->viewangles.p + 180.0f, 360.0f) - 180.0f;
        auto normalized_y = fmod(m_pcmd->viewangles.y + 180.0f, 360.0f) - 180.0f;

        auto yaw = DEG2RAD((normalized_y - viewangles.y) + angles_move.y);

        if (normalized_x >= 90.0f || normalized_x <= -90.0f || m_pcmd->viewangles.p >= 90.0f && m_pcmd->viewangles.p <= 200.0f || m_pcmd->viewangles.p <= -90.0f && m_pcmd->viewangles.p <= 200.0f) //-V648
            m_pcmd->forwardmove = -cos(yaw) * speed;
        else
            m_pcmd->forwardmove = cos(yaw) * speed;

        m_pcmd->sidemove = sin(yaw) * speed;
	} else {
        if (fabsf(m_pcmd->mousedx) > 2)
        {
            m_pcmd->sidemove = (m_pcmd->mousedx < 0.f) ? -450.f : 450.f;
            return;
        }
        if (csgo->local_player->velocity().length_2d() <= 0.5f)
        {
            m_pcmd->forwardmove = 400.f;
            return;
        }

        m_pcmd->forwardmove = std::clamp((5850.f / csgo->local_player->velocity().length_2d()), -400.f, 400.f);

        if ((m_pcmd->forwardmove < -400.f || m_pcmd->forwardmove > 400.f))
            m_pcmd->forwardmove = 0;

        const auto vel = csgo->local_player->velocity();
        const float y_vel = utils->deg(atan2(vel.y, vel.x));
        const float diff_ang = c_angle(0, m_pcmd->viewangles.y - y_vel, 0).normalize().y;

        static auto sv_airaccelerate = csgo->cvar->find_var(str("sv_airaccelerate"));

        m_pcmd->sidemove = (diff_ang > 0.0) ? -450.f : 450.f;
        m_pcmd->viewangles.y = c_angle(0, m_pcmd->viewangles.y - diff_ang * (sv_airaccelerate->get_float() > 30.f ? .8f : .96f), 0).normalize().y;
	}


}