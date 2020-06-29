#pragma once

enum
{
	FBEAM_STARTENTITY = 0x00000001,
	FBEAM_ENDENTITY = 0x00000002,
	FBEAM_FADEIN = 0x00000004,
	FBEAM_FADEOUT = 0x00000008,
	FBEAM_SINENOISE = 0x00000010,
	FBEAM_SOLID = 0x00000020,
	FBEAM_SHADEIN = 0x00000040,
	FBEAM_SHADEOUT = 0x00000080,
	FBEAM_ONLYNOISEONCE = 0x00000100, // Only calculate our noise once
	FBEAM_NOTILE = 0x00000200,
	FBEAM_USE_HITBOXES = 0x00000400, // Attachment indices represent hitbox indices instead when this is set.
	FBEAM_STARTVISIBLE = 0x00000800, // Has this client actually seen this beam's start entity yet?
	FBEAM_ENDVISIBLE = 0x00001000, // Has this client actually seen this beam's end entity yet?
	FBEAM_ISACTIVE = 0x00002000,
	FBEAM_FOREVER = 0x00004000,
	FBEAM_HALOBEAM = 0x00008000, // When drawing a beam with a halo, don't ignore the segments and endwidth
	FBEAM_REVERSED = 0x00010000,
	NUM_BEAM_FLAGS = 17 // KEEP THIS UPDATED!
};

enum
{
	TE_BEAMPOINTS = 0x00, // beam effect between two points
	TE_SPRITE = 0x01, // additive sprite, plays 1 cycle
	TE_BEAMDISK = 0x02, // disk that expands to max radius over lifetime
	TE_BEAMCYLINDER = 0x03, // cylinder that expands to max radius over lifetime
	TE_BEAMFOLLOW = 0x04, // create a line of decaying beam segments until entity stops moving
	TE_BEAMRING = 0x05, // connect a beam ring to two entities
	TE_BEAMSPLINE = 0x06,
	TE_BEAMRINGPOINT = 0x07,
	TE_BEAMLASER = 0x08, // Fades according to viewpoint
	TE_BEAMTESLA = 0x09,
};

struct s_beaminfo
{
	int type;
	c_entity* start_ent;
	int start_attachment;
	c_entity* end_ent;
	int end_attachment;
	c_vector start;
	c_vector end;
	int model_index;
	const char* model_name;
	int halo_index;
	const char* halo_name;
	float halo_scale;
	float life;
	float width;
	float end_width;
	float fade_length;
	float amplitude;
	float brightness;
	float speed;
	int startframe;
	float framerate;
	float red;
	float green;
	float blue;
	bool renderable;
	int segments;
	int flags;
	c_vector center;
	float start_radius;
	float end_radius;

	s_beaminfo()
	{
		type = TE_BEAMPOINTS;
		segments = -1;
		model_name = NULL;
		halo_name = NULL;
		model_index = -1;
		halo_index = -1;
		renderable = true;
		flags = 0;
	}
};

class c_beam;
class c_beamrenderer
{
public:
	virtual void		init_beams(void) = 0;
	virtual void		shutdown_beams(void) = 0;
	virtual void		clear_beams(void) = 0;
	virtual void		update_beams() = 0;
	virtual void		draw_beam(void* pbeam, void* pEntityBeamTraceFilter = NULL) = 0;
	virtual void		draw_beam(c_beam* pbeam) = 0;
	virtual void		kill_dead_beams(c_entity* pEnt) = 0;
	virtual c_beam*		create_beam_ents(s_beaminfo& beamInfo) = 0;
	virtual c_beam*		create_beam_point(s_beaminfo& beamInfo) = 0;
	virtual c_beam*		create_beam_points(s_beaminfo& beamInfo) = 0;
	virtual c_beam*		create_beam_ring(s_beaminfo& beamInfo) = 0;
	virtual c_beam*		create_beam_ring_point(s_beaminfo& beamInfo) = 0;
	virtual c_beam*		create_beam_circle_points(s_beaminfo& beamInfo) = 0;
	virtual c_beam*		create_beam_follow(s_beaminfo& beamInfo) = 0;
	virtual void		free_beam(c_beam* pBeam) = 0;
	virtual void		update_beam_info(c_beam* pBeam, s_beaminfo& beamInfo) = 0;
	virtual void		create_beam_ents(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int type = -1) = 0;
	virtual void		create_beam_point(int nStartEntity, const c_vector* pStart, int nEndEntity, const c_vector* pEnd,
		int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual void		create_beam_points(c_vector& start, c_vector& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual void		create_beam_ring(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags = 0) = 0;
	virtual void		create_beam_ring_point(const c_vector& center, float start_radius, float end_radius, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags = 0) = 0;
	virtual void		create_beam_circle_points(int type, c_vector& start, c_vector& end,
		int modelIndex, int haloIndex, float haloScale, float life, float width,
		float m_nEndWidth, float m_nFadeLength, float amplitude, float brightness, float speed,
		int startFrame, float framerate, float r, float g, float b) = 0;
	virtual void		create_beam_follow(int startEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float r, float g, float b,
		float brightness) = 0;
};