#pragma once

class s_player_info {
public:
	char _00[8];
	long long steam64;
	char name[128];
	int user_id;
	char steam_id[33];
	unsigned int friendlist_id;
	char friendlist_name[128];
	bool is_bot;
	bool is_gotv;
	char _01[128];

	std::string get_name() {return name;}
	std::string get_steam32() {return steam_id;}
	std::string get_friendname() {return friendlist_name;}
};

enum
{
    EF_BONEMERGE			= 0x001,	// Performs bone merge on client side
    EF_BRIGHTLIGHT 			= 0x002,	// DLIGHT centered at entity origin
    EF_DIMLIGHT 			= 0x004,	// player flashlight
    EF_NOINTERP				= 0x008,	// don't interpolate the next frame
    EF_NOSHADOW				= 0x010,	// Don't cast no shadow
    EF_NODRAW				= 0x020,	// don't draw entity
    EF_NORECEIVESHADOW		= 0x040,	// Don't receive no shadow
    EF_BONEMERGE_FASTCULL	= 0x080,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
    // parent and uses the parent's bbox + the max extents of the aiment.
    // Otherwise, it sets up the parent's bones every frame to figure out where to place
    // the aiment, which is inefficient because it'll setup the parent's bones even if
    // the parent is not in the PVS.
            EF_ITEM_BLINK			= 0x100,	// blink an item so that the user notices it.
    EF_PARENT_ANIMATES		= 0x200,	// always assume that the parent entity is animating
    EF_MAX_BITS = 10
};

class c_player : public c_entity {
public:
	bool bounding_box(ImVec2* mins, ImVec2* maxs, c_vector &og);
	s_player_info& get_player_info();
	bool is_fake_ducking();
	c_animstate* get_anim_state();
	void set_abs_angles(c_angle v);
	void set_abs_origin(c_vector v);
    c_collider* get_collider_offset();
    c_vector get_hitbox_pos(int hb, c_matrix3x4* bones);
    float get_max_body_delta();
    void create_anim_state(c_csgo_animstate* as);
    void reset_anim_state(c_csgo_animstate* as);
    void update_anim_state(c_csgo_animstate* as, c_angle absangle);
    void invalidate_bone_cache();
    void update_clientside_animation();
    float calc_servertime();

    c_entity* get_entity_scripting();

    int& agent();
    bool& fog_enabled();
    int& fog_color();
    float& fog_start();
    float& fog_end();
    float& fog_maxdensity();
    bool& spotted();
    int& spotted_by(int i);
    int& team();
	int& health();
	int& armor();
	bool& has_helmet();
	bool& has_heavy_armor();
	int& money();
	bool& is_scoped();
	c_usercmd*& current_command();
	c_ehandle& weapon();
	float& healthshot_effect();
	float& max_flash_alpha();
	c_vector view_offset();
	int& spec_mode();
	c_angle& viewpunch();
	c_angle& aimpunch();
	c_ehandle& spectator();
	unsigned int & flags();
	int& movetype();
	c_vector velocity();
	float& simtime();
	bool& is_immune();
	int& tickbase();
	float& spawntime();
	c_animlayer* animlayers();
	float* poseparams();
	bool& clientsideanim();
	c_angle& rotation();
	int& effects();
	c_matrix3x4** cached_bones();
	int& cached_bones_count();
	float& lby();
	c_angle& eyeangle();
	float& nextattack();
	float& velocity_mod();
    c_angle& aimpunchvel();
    float& duck_speed();
    float& duck_amount();

};