#pragma once
#define SURF_LIGHT		0x0001		// value will hold the light strength
#define SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define SURF_SKY		0x0004		// don't draw, but add to skybox
#define SURF_WARP		0x0008		// turbulent water warp
#define SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define SURF_NODRAW		0x0080	// don't bother referencing the texture

#define SURF_HINT		0x0100	// make a primary bsp splitter

#define SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOPAINT	SURF_NODECALS	// the surface can not have paint placed on it
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface
#define SURF_HITBOX		0x8000	// surface is part of a hitbox

struct s_surfacephysicsparams
{
	// vphysics physical properties
	float			friction;
	float			elasticity;				// collision elasticity - used to compute coefficient of restitution
	float			density;				// physical density (in kg / m^3)
	float			thickness;				// material thickness if not solid (sheet materials) in inches
	float			dampening;
};

struct s_surfaceaudioparams
{
	// sounds / audio data
	float			reflectivity;		// like elasticity, but how much sound should be reflected by this surface
	float			hardnessFactor;	// like elasticity, but only affects impact sound choices
	float			roughnessFactor;	// like friction, but only affects scrape sound choices

	// audio thresholds
	float			roughThreshold;	// surface roughness > this causes xorstr_("rough") scrapes, < this causes xorstr_("smooth") scrapes
	float			hardThreshold;	// surface hardness > this causes xorstr_("hard") impacts, < this causes xorstr_("soft") impacts
	float			hardVelocityThreshold;	// collision velocity > this causes xorstr_("hard") impacts, < this causes xorstr_("soft") impacts
	// NOTE: Hard impacts must meet both hardnessFactor AND velocity thresholds
    float highPitchOcclusion; //a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
    float midPitchOcclusion;
    float lowPitchOcclusion;
};

struct s_surfacesoundnames
{
	unsigned short	stepleft;
	unsigned short	stepright;

    unsigned short	runStepLeft;
    unsigned short	runStepRight;

	unsigned short	impactSoft;
	unsigned short	impactHard;

	unsigned short	scrapeSmooth;
	unsigned short	scrapeRough;

	unsigned short	bulletImpact;
	unsigned short	rolling;

	unsigned short	breakSound;
	unsigned short	strainSound;
};

struct s_surfacegameprops
{
public:
	float    maxSpeedFactor; //0x0000
	float    jumpFactor; //0x0004
	float    flPenetrationModifier; //0x000C
	float    flDamageModifier; //0x0010
	unsigned short    material; //0x0014
	byte climbable;

};//Size=0x0019

struct s_surfacedata
{
	s_surfacephysicsparams	physics;	// physics parameters
	s_surfaceaudioparams	audio;		// audio parameters
	s_surfacesoundnames		sounds;		// names of linked sounds
	s_surfacegameprops		game;		// Game data / properties
};

class c_physics
{
public:
	virtual					~c_physics(void) {}
	virtual int				parse_surface_data(const char* pFilename, const char* pTextfile) = 0;
	virtual int				surface_prop_count(void) = 0;
	virtual int				get_surface_index(const char* pSurfacePropName) = 0;
	virtual void			get_physics_properties(int surfaceDataIndex, float* density, float* thickness, float* friction, float* elasticity) = 0;
	virtual s_surfacedata*	get_surface_data(int surfaceDataIndex) = 0;
	virtual const char*		get_string(unsigned short stringTableIndex) = 0;
	virtual const char*		get_prop_name(int surfaceDataIndex) = 0;
	virtual void			set_world_material_index_table(int* pMapArray, int mapSize) = 0;
	virtual void			get_physics_parameters(int surfaceDataIndex, s_surfacephysicsparams* pParamsOut) = 0;
};