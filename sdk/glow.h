#pragma once

struct s_glowobject
{
	c_entity* entity;

	union
	{
		c_vector color;
		struct
		{
			float   r;
			float   g;
			float   b;
		};
	};

	float a;
	char pad_0014[4];
	float unk0;
	char pad_001C[4];
	float unk1;
	bool render_occluded;
	bool render_unoccluded;
	bool full_bloom_render;
	char pad_0027[5];
	int glow_type;
	int splitscreen_slot;
	int next_free_slot;

	bool is_unused() const
	{
		return next_free_slot != -2;
	}
};

class c_glowmanager {
public:
	s_glowobject* objects;
	int numobjects;
};