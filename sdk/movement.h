#pragma once

class c_movedata {
public:
	bool    first_run_of_functions;
	bool    game_code_moved_player;
	int     player_handle;        // edict index on server, client entity handle on client=
	int     impulse_command;      // Impulse command issued.
	c_vector  viewangles;        // Command view angles (local space)
	c_vector  absviewangles;     // Command view angles (world space)
	int     buttons;             // Attack buttons.
	int     oldbuttons;          // From host_client->oldbuttons;
	float   forwardmove;
	float   sidemove;
	float   upmove;
	float   maxspeed;
	float   clientmaxspeed;
	c_vector  velocity;          // edict::velocity        // Current movement direction.
	c_vector  angles;            // edict::angles
	c_vector  oldangles;
	float   outstepheight;        // how much you climbed this move
	c_vector  outwishvel;           // This is where you tried 
	c_vector  outjumpvel;           // This is your jump velocity
	c_vector  constraintcenter;
	float   constraintradius;
	float   constraintwidth;
	float   constraingfactor;
	float   unknown[5];
	c_vector  absorigin;
};

class c_move : c_movedata {
public:
    virtual	void _vpad() = 0;
    virtual void set_host(c_cliententity* host) = 0;
};

class c_movement {
public:
	virtual		  ~c_movement(void) { }
	virtual void  process_movement(c_entity* pPlayer, c_movedata* pMove) = 0;
	virtual void  Reset(void) = 0;
	virtual void  start_track_prediction_errors(c_entity* pPlayer) = 0;
	virtual void  finish_track_prediction_errors(c_entity* pPlayer) = 0;
};

class c_movehelper {
public:
    virtual	char const*		get_name( void* handle ) const = 0;
    virtual void	        reset_touch_list( void ) = 0;
    virtual bool	        add_to_touched( const void* tr, const c_vector& impactvelocity ) = 0;
    virtual void	        process_impacts( void ) = 0;
    virtual void	        con_nprintf( int idx, char const* fmt, ... ) = 0;
    virtual void	        start_sound( const c_vector& origin, int channel, char const* sample, float volume, int soundlevel, int fFlags, int pitch ) = 0;
    virtual void	        start_sound( const c_vector& origin, const char *soundname ) = 0;
    virtual void	        playback_event_full( int flags, int clientindex, unsigned short eventindex, float delay, c_vector& origin, c_vector& angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 ) = 0;
    virtual bool	        player_falling_damage( void ) = 0;
    virtual void	        player_set_animation( int playerAnim ) = 0;
    virtual void*           get_surface_props( void ) = 0;
    virtual bool            is_world_entity( const c_ehandle &handle ) = 0;
};