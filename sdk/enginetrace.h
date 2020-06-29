#pragma once

#define	CONTENTS_EMPTY			0	
#define	CONTENTS_SOLID			0x1	
#define	CONTENTS_WINDOW			0x2	
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8	
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_MIST			0x40
#define CONTENTS_OPAQUE			0x80
#define	LAST_VISIBLE_CONTENTS	0x80
#define ALL_VISIBLE_CONTENTS 	(LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))
#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED5		0x200
#define CONTENTS_UNUSED6		0x4000
#define CONTENTS_TEAM1			0x800				
#define CONTENTS_TEAM2			0x1000				
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000		
#define CONTENTS_MOVEABLE		0x4000				
#define	CONTENTS_AREAPORTAL		0x8000				
#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

#define	CONTENTS_ORIGIN			0x1000000	
#define	CONTENTS_MONSTER		0x2000000	
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	
#define	CONTENTS_TRANSLUCENT	0x10000000	
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	

#define	MASK_ALL					(0xFFFFFFFF)
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 					
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME) 													
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE) 													
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)																		
#define	MASK_VISIBLE				(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE) 														
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE) 												
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX) 
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE) 	
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW) 													
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)									
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE) 									
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE) 				
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 				
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 								
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E'
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z'

class c_ray
{
public:
	c_vectoraligned  start;
	c_vectoraligned  delta;
	c_vectoraligned  start_offset;
	c_vectoraligned  extents;
	const c_matrix3x4* world_axis_transform;
	bool is_ray;
	bool is_swept;

	c_ray() : world_axis_transform(NULL), is_ray(true), is_swept(false) {}

	c_ray(c_vector const& start, c_vector const& end) {
		this->delta = end - start;
		this->is_swept = (delta.length_sqr() != 0);
		this->is_ray = true;
		this->start = start;
	}

	void init(c_vector const& start, c_vector const& end)
	{
		this->delta = end - start;
		this->is_swept = (delta.length_sqr() != 0);
		this->is_ray = true;
		this->start = start;
	}

	void init(c_vector const& start, c_vector const& end, c_vector const& mins, c_vector const& maxs)
	{
		this->delta = end - start;
		this->is_swept = (delta.length_sqr() != 0);
		this->extents = maxs - mins;
		this->extents *= 0.5f;
		this->is_ray = (extents.length_sqr() < 1e-6);
		this->start_offset = maxs + mins;
		this->start_offset *= 0.5f;
		this->start = start + this->start_offset;
		this->start_offset *= -1.0f;
	}
};

class c_plane
{
public:
	c_vector normal;
	float dist;
	char type;		
	char signbits;	
	char pad[2];
};

class c_tracesurface
{
public:
	const char* name;
	short surface_props;
	unsigned short flags;
};

class c_trace
{
public:
	c_vector start;
	c_vector end;
	c_plane plane;
	float fraction;
	int contents;
	unsigned short dispflag;
	bool allsolid;
	bool startSolid;
	float fractionleftsolid;
	c_tracesurface surface;
	int hitGroup;
	short physicsBone;
	c_entity* ent;
	int hitbox;

	bool did_hit() const
	{
		return fraction < 1 || allsolid || startSolid;
	}

	bool did_hit_nonworld_entity() const
	{
		return ent && !did_hit();
	}
};

enum e_tracetype
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				
	TRACE_ENTITIES_ONLY,			
	TRACE_EVERYTHING_FILTER_PROPS,	
};

class c_tracefilter
{
public:
	virtual bool			should_hit_entity(void* pEntity, int contentsMask) = 0;
	virtual e_tracetype		get_trace_type() const = 0;
};

bool standart_filter_rules(c_entity* ent, int contentsMask);

class c_tracefilter_simple : public c_tracefilter {
public:
    bool should_hit_entity(void* pEntityHandle, int contentsMask) {
        if (!standart_filter_rules((c_entity*)pEntityHandle, contentsMask))
            return false;

        if (pEntityHandle == pass_ent)
            return false;

        return true;
    }

    e_tracetype get_trace_type() {
        return TRACE_EVERYTHING;
    }

    c_entity* pass_ent;
    int collision_group;
    void* extrahitfn;
};

class c_tracefilter_everything : public c_tracefilter
{
public:
	bool should_hit_entity(void* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle != skip);
	}

	e_tracetype get_trace_type() const
	{
		return TRACE_EVERYTHING;
	}

	inline void set_ignore_class(char* Class)
	{
		ccIgnore = Class;
	}

	void* skip;
	char* ccIgnore = (char*)"";
};

class c_tracefilter_everything_exactent : public c_tracefilter
{
public:
	bool should_hit_entity(void* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle == entity);
	}

	e_tracetype get_trace_type() const
	{
		return TRACE_EVERYTHING;
	}

	void* entity;
};

class c_tracefilter_entities : public c_tracefilter
{
public:
	bool should_hit_entity(void* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle == entity);
	}

	e_tracetype get_trace_type() const
	{
		return TRACE_ENTITIES_ONLY;
	}

	void* entity;
};

class c_tracefilter_twoentities : public c_tracefilter
{
public:
	c_tracefilter_twoentities(void* pPassEnt1, void* pPassEnt2)
	{
		passentity1 = pPassEnt1;
		passentity2 = pPassEnt2;
	}

	virtual bool should_hit_entity(void* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == passentity1 || pEntityHandle == passentity2);
	}

	virtual e_tracetype get_trace_type() const
	{
		return TRACE_EVERYTHING;
	}

	void* passentity1;
	void* passentity2;
};

class c_tracefilter_entities_butentity : public c_tracefilter
{
public:
	bool should_hit_entity(void* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == skip);
	}

	e_tracetype get_trace_type() const
	{
		return TRACE_ENTITIES_ONLY;
	}

	void* skip;
};

class c_tracefilter_world : public c_tracefilter
{
public:
	bool should_hit_entity(void* pEntityHandle, int contentsMask)
	{
		return false;
	}

	e_tracetype get_trace_type() const
	{
		return TRACE_EVERYTHING;
	}

	void* skip;
};

class c_enginetrace
{
public:
	int get_point_contents(const c_vector& vecAbsPosition, int contentsMask = MASK_ALL, c_handle** ppEntity = NULL);
	void trace_ray(const c_ray& ray, unsigned int fMask, c_tracefilter* filter, c_trace* trace);
	void clip_ray_to_entity(const c_ray& ray, unsigned int fMask, c_entity* pEnt, c_trace* pTrace);
	void clip_ray_to_collideable(const c_ray& ray, unsigned int fMask, c_collider* pEnt, c_trace* pTrace);
};