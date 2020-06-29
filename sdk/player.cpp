#include "../cheat.h"
#include "player.h"


bool c_player::bounding_box(ImVec2* a, ImVec2* b, c_vector &og) {
	auto collid = g_offset_ptr(c_collider, this, 0x320);
	if (!collid)
		return false;

	auto mins = collid->mins;
	auto maxs = collid->maxs;

	auto pos_3d = og;
	auto top_3d = pos_3d + c_vector(0, 0, maxs.z);

	c_vector pos, top;
	if (utils->world_to_screen(pos_3d, &pos) && utils->world_to_screen(top_3d, &top))
	{
		int height = pos.y - top.y;
		int width = height / 2;

		*a = ImVec2(pos.x - width / 2, top.y);
		*b = ImVec2(a->x + width, a->y + height);

		return true;
	}
	else
		return false;
}

c_vector c_player::get_hitbox_pos(int hb, c_matrix3x4* mat) {
    auto model = this->get_renderable()->get_model();
    if (!model)
        return c_vector(0, 0, 0);

    auto studio = csgo->modelinfo->get_studio_model(model);
    if (!studio)
        return c_vector(0, 0, 0);

    auto hbset = studio->get_hitbox_set(0);
    if (!hbset)
        return c_vector(0, 0, 0);

    auto hitbox = hbset->get_hitbox(hb);
    if (!hitbox)
        return c_vector(0, 0, 0);

    c_vector mins, maxs;
    utils->vector_transform(hitbox->bbmin, mat[hitbox->bone], mins);
    utils->vector_transform(hitbox->bbmax, mat[hitbox->bone], maxs);

    return (mins + maxs) * 0.5f;
}

c_collider* c_player::get_collider_offset() {
    return g_offset_ptr(c_collider, this, 0x320);
}

std::unordered_map<int, s_player_info> infos;
s_player_info& c_player::get_player_info() {
	csgo->engine->get_player_info(this->index(), &infos[index()]);
	
	return infos[index()];
}

bool c_player::is_fake_ducking() {
	return false; // rework )))
}

int& c_player::team() {
	static auto n = csgo->netvars[str("DT_BaseEntity")][str("m_iTeamNum")];
	return g_offset(int, this, n.offset);
}

int& c_player::health() {
	static auto n = csgo->netvars[str("DT_BasePlayer")][str("m_iHealth")];
	return g_offset(int, this, n.offset);
}

int& c_player::armor() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_ArmorValue")];
	return g_offset(int, this, n.offset);
}

bool& c_player::has_helmet() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_bHasHelmet")];
	return g_offset(bool, this, n.offset);
}

bool& c_player::has_heavy_armor() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_bHasHeavyArmor")];
	return g_offset(bool, this, n.offset);
}

int& c_player::money() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_iAccount")];
	return g_offset(int, this, n.offset);
}

bool& c_player::is_scoped() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_bIsScoped")];
	return g_offset(bool, this, n.offset);
}

c_usercmd*& c_player::current_command(){
    static auto current_command = * reinterpret_cast< uint32_t* >(utils->scan(str("client_panorama.dll"), str("80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81")));

    return * reinterpret_cast< c_usercmd** > ( reinterpret_cast< uintptr_t > ( this ) + current_command );

}

c_ehandle& c_player::weapon() {
	static auto n = csgo->netvars[str("DT_BaseCombatCharacter")][str("m_hActiveWeapon")];
	return g_offset(c_ehandle, this, n.offset);
}

float& c_player::healthshot_effect() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_flHealthShotBoostExpirationTime")];
	return g_offset(float, this, n.offset);
}

float& c_player::max_flash_alpha() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_flFlashMaxAlpha")];
	return g_offset(float, this, n.offset);
}

float& c_player::simtime() {
    static auto n = csgo->netvars[str("DT_BaseEntity")][str("m_flSimulationTime")];
    return g_offset(float, this, n.offset);
}

float& c_player::nextattack() {
    static auto n = csgo->netvars[str("DT_BaseCombatCharacter")][str("bcc_localdata")][str("m_flNextAttack")];
    return g_offset(float, this, n.offset);
}

c_vector c_player::view_offset() {
	static auto n1 = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_vecViewOffset[0]")];
	static auto n2 = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_vecViewOffset[1]")];
	static auto n3 = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_vecViewOffset[2]")];
	return c_vector(g_offset(float, this, n1.offset), g_offset(float, this, n2.offset), g_offset(float, this, n3.offset));
}

int& c_player::spec_mode() {
	static auto n = csgo->netvars[str("DT_BasePlayer")][str("m_iObserverMode")];
	return g_offset(int, this, n.offset);
}

c_angle& c_player::viewpunch() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("baseclass")][str("localdata")][str("m_Local")][str("m_viewPunchAngle")];
	return g_offset(c_angle, this, n.offset);
}

c_angle& c_player::aimpunch() {
	static auto n = csgo->netvars[str("DT_CSPlayer")][str("baseclass")][str("localdata")][str("m_Local")][str("m_aimPunchAngle")];
	return g_offset(c_angle, this, n.offset);
}

c_angle& c_player::aimpunchvel() {
    static auto n = csgo->netvars[str("DT_CSPlayer")][str("baseclass")][str("localdata")][str("m_Local")][str("m_aimPunchAngleVel")];
    return g_offset(c_angle, this, n.offset);
}

c_angle& c_player::eyeangle() {
    static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_angEyeAngles[0]")];
    return g_offset(c_angle, this, n.offset);
}

c_ehandle& c_player::spectator() {
	static auto n = csgo->netvars[str("DT_BasePlayer")][str("m_hObserverTarget")];
	return g_offset(c_ehandle, this, n.offset);
}

unsigned int & c_player::flags() {
	static auto n = csgo->netvars[str("DT_BasePlayer")][str("m_fFlags")];
	return g_offset(unsigned int, this, n.offset);
}

int& c_player::movetype() {
	return g_offset(int, this, 0x25C);
}

c_vector c_player::velocity() {
	static auto n1 = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_vecVelocity[0]")];
	static auto n2 = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_vecVelocity[1]")];
	static auto n3 = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_vecVelocity[2]")];
	return c_vector(g_offset(float, this, n1.offset), g_offset(float, this, n2.offset), g_offset(float, this, n3.offset));
}

c_animstate *c_player::get_anim_state() {
    return *(c_animstate**)((unsigned long long)this + 0x3900);
}

void c_player::set_abs_angles(c_angle v) {
    using t = void(__thiscall*)(void*, const c_angle&);
    static auto fn = (t)utils->scan(str("client_panorama.dll"), str("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));

    fn(this, v);
}

float c_player::get_max_body_delta() {
    auto animstate = this->get_anim_state();

    auto duckammount = animstate->duck_amount;
    auto speedfraction = max( 0, min( animstate->feet_speed_sideways, 1 ) );
    auto speedfactor = max( 0, min( 1, animstate->feet_speed_unk ) );

    auto unk1 = ( ( animstate->unk123 * -0.30000001 ) - 0.19999999 ) * speedfraction;
    auto unk2 = unk1 + 1.f;

    if ( duckammount > 0 )
        unk2 += ( ( duckammount * speedfactor ) * ( 0.5f - unk2 ) );

    return *( float * ) ( (unsigned long long)animstate + 0x334 ) * unk2;
}

c_entity* c_player::get_entity_scripting() {
    return get_entity();
}

void c_player::set_abs_origin(c_vector v){
    using t = void(__thiscall*)(void*, const c_vector&);
    static auto fn = (t)utils->scan(str("client_panorama.dll"), str("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));

    fn(this, v);
}

bool& c_player::is_immune() {
    static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_bGunGameImmunity")];
    return g_offset(bool, this, n.offset);
}

int& c_player::tickbase() {
    static auto n = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_nTickBase")];
    return g_offset(int, this, n.offset);
}

void c_player::create_anim_state(c_csgo_animstate* as) {
    // 55 8B EC 56 8B F1 B9 ? ? ? ? C7 46

    using t = void(__thiscall*)(c_csgo_animstate*, c_entity*);
    static auto fn = (t)utils->scan(str("client_panorama.dll"), str("55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));

    fn(as, this);
}

void c_player::reset_anim_state(c_csgo_animstate* as) {
    // 56 6A 01 68 ? ? ? ? 8B F1

    using t = void(__thiscall*)(c_csgo_animstate*);
    static auto fn = (t)utils->scan(str("client_panorama.dll"), str("56 6A 01 68 ? ? ? ? 8B F1"));

    fn(as);
}

void c_player::update_anim_state(c_csgo_animstate* state, c_angle absangle) {
    // 55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24

    static auto UpdateAnimState = utils->scan(
        "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");

    if (!UpdateAnimState)
        return;

    __asm {
        push 0
    }

    __asm
    {
        mov ecx, state

        movss xmm1, dword ptr[absangle + 4]
        movss xmm2, dword ptr[absangle]

        call UpdateAnimState
    }
}

float& c_player::spawntime() {
    return *(float*)((unsigned long long)this + 0xA350);
}

c_animlayer* c_player::animlayers() {
    return *(c_animlayer**)((unsigned long long)this + 0x2990);
}

float* c_player::poseparams() {
    static auto n = csgo->netvars[str("DT_BaseAnimating")][str("m_flPoseParameter")];
    return (float*)((unsigned long long)this + n.offset);
}

bool& c_player::clientsideanim() {
    static auto n = csgo->netvars[str("DT_BaseAnimating")][str("m_bClientSideAnimation")];
    return *(bool*)((unsigned long long)this + n.offset);
}

c_angle& c_player::rotation() {
    static auto n = csgo->netvars[str("DT_BaseEntity")][str("m_angRotation")];
    return *(c_angle*)((unsigned long long)this + n.offset);
}

int& c_player::effects() {
    static auto n = csgo->netvars[str("DT_BaseEntity")][str("m_fEffects")];
    return *(int*)((unsigned long long)this + n.offset);
}

c_matrix3x4** c_player::cached_bones() {
    return (c_matrix3x4**)((unsigned long long)this + 0x2910);
}

int& c_player::cached_bones_count() {
    return *(int*)((unsigned long long)this + 0x291C);
}

void c_player::invalidate_bone_cache() {
    static auto ibc = utils->scan(str("client_panorama.dll"), str("80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81"));
    auto mbc = **(unsigned long**)(ibc + 10);

    *(unsigned int*)((unsigned long long)this + 0x2924) = 0xFF7FFFFF;
    *(unsigned int*)((unsigned long long)this + 0x2690) = (mbc - 1);
}

void c_player::update_clientside_animation() {
    g_vfunc(fnUpdateClientsideAnimation, this, 223)(this);
}

float c_player::calc_servertime() {
    static int tick = 0;
    static c_usercmd* last_cmd = nullptr;

    if (!csgo->cmd)
        return tick * csgo->globals->tickinterval;

    if (!last_cmd || last_cmd->hasbeenpredicted)
        tick = tickbase();
    else
        ++tick;

    last_cmd = csgo->cmd;
    return tick * csgo->globals->tickinterval;
}

float& c_player::lby() {
    static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_flLowerBodyYawTarget")];
    return *(float*)((unsigned long long)this + n.offset);
}

bool &c_player::spotted() {
    static auto n = csgo->netvars[str("DT_BaseEntity")][str("m_bSpotted")];
    return g_offset(bool, this, n.offset);
}

int &c_player::spotted_by(int i) {
    static auto n = csgo->netvars[str("DT_BaseEntity")][str("m_bSpottedBy")];
    return g_offset(int, this, n.offset + i * 4);
}

bool &c_player::fog_enabled() {
    static auto n = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_Local")][str("m_skybox3d.fog.enable")];
    return g_offset(bool, this, n.offset);
}

int &c_player::fog_color() {
    static auto n = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_Local")][str("m_skybox3d.fog.colorPrimary")];
    return g_offset(int, this, n.offset);
}

float &c_player::fog_start() {
    static auto n = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_Local")][str("m_skybox3d.fog.start")];
    return g_offset(float, this, n.offset);
}

float &c_player::fog_end() {
    static auto n = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_Local")][str("m_skybox3d.fog.end")];
    return g_offset(float, this, n.offset);
}

float &c_player::fog_maxdensity() {
    static auto n = csgo->netvars[str("DT_BasePlayer")][str("localdata")][str("m_Local")][str("m_skybox3d.fog.maxdensity")];
    return g_offset(float, this, n.offset);
}

int &c_player::agent() {
    static auto n = csgo->netvars[str("DT_CSPlayer")][str("m_iClass")];
    return g_offset(int, this, n.offset);
}

float &c_player::velocity_mod() {
    static auto n = csgo->netvars[str("DT_CSPlayer")][str("cslocaldata")][str("m_flVelocityModifier")];
    return g_offset(float, this, n.offset);
}

float& c_player::duck_speed() {
    static auto n = csgo->netvars[str("CCSPlayer")][str("localdata")][str("m_flDuckSpeed")];
    return g_offset(float, this, n.offset);
}

float& c_player::duck_amount() {
    static auto n = csgo->netvars[str("CCSPlayer")][str("localdata")][str("m_flDuckAmount")];
    return g_offset(float, this, n.offset);
}