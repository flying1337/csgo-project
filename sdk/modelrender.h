#pragma once

class c_renderable;

enum e_overridetype
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};

struct c_renderableinfo {
	c_renderable* renderable;
	void* alpha_prop;
	int enum_count;
	int render_frame;
	unsigned short first_shadow;
	unsigned short leaf_list;
	short area;
	unsigned short _0;   // 0x0016
	unsigned short _1; // 0x0018
	c_vector bloated_abs_mins;
	c_vector bloated_abs_maxs;
	c_vector abs_mins;
	c_vector abs_maxs;
	int pad;
};

class c_modelrenderinfo
{
public:
	c_vector origin;
	c_vector angles;
	char pad[0x4];
	c_renderable* renderable;
	const c_model* model;
	const c_matrix3x4* model_to_world;
	const c_matrix3x4* lighting_offset;
	const c_vector* lighting_origin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	unsigned short instance;
};

class c_drawmodelstate
{
public:
	c_studiohdr* studiohdr;
	void* studio_data;
	c_renderable* renderable;
	const c_matrix3x4* model_to_world;
	void* decals;
	int	flags;
	int	lod;
};

class c_modelrender {
public:
	virtual int          draw_model(int flags, c_renderable* pRenderable, void* instance, int ent, const c_model* model, c_vector const& origin, c_angle const& angles, int skin, int body, int hitboxset, const c_matrix3x4* modelToWorld = NULL, const c_matrix3x4* pLightingOffset = NULL) = 0;
	virtual void         forced_material_override(c_material* newMaterial, e_overridetype nOverrideType = OVERRIDE_NORMAL, int nOverrides = 0) = 0;
	virtual bool         is_forced_material_override(void) = 0;
	virtual void         set_view_target(const c_studiohdr* pStudioHdr, int nBodyIndex, const c_vector& target) = 0;
	virtual void*		 create_intance(c_renderable* pRenderable, void* pCache = NULL) = 0;
	virtual void         destroy_instance(void* handle) = 0;
	virtual void         set_static_lighting(void* handle, void* pHandle) = 0;
	virtual void*		 get_static_lighting(void* handle) = 0;
	virtual bool         change_instance(void* handle, c_renderable* pRenderable) = 0;
	virtual void         add_decal(void* handle, void* const& ray, c_vector const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal) = 0;
	virtual void         remove_all_decals(void* handle) = 0;
	virtual bool         model_has_decals(void* handle) = 0;
	virtual void         remove_all_decals_from_all_models() = 0;
	virtual c_matrix3x4* draw_model_shadow_setup(c_renderable* pRenderable, int body, int skin, void* pInfo, c_matrix3x4* pCustomBoneToWorld = NULL) = 0;
	virtual void         draw_model_shadow(c_renderable* pRenderable, const void* info, c_matrix3x4* pCustomBoneToWorld = NULL) = 0;
	virtual bool         recompute_static_lighting(void* handle) = 0;
	virtual void         release_static_prop_data(void) = 0;
	virtual void         restore_static_prop_data(void) = 0;
	virtual int          draw_model_ex(c_modelrenderinfo& pInfo) = 0;
	virtual int          draw_model_ex_setup_prop(c_modelrenderinfo& pInfo) = 0;
	virtual bool         draw_model_setup(c_modelrenderinfo& pInfo, c_drawmodelstate* pState, c_matrix3x4** ppBoneToWorldOut) = 0;
	virtual void         draw_model_execute(void* ctx, const c_drawmodelstate& state, const c_modelrenderinfo& pInfo, c_matrix3x4* pCustomBoneToWorld = NULL) = 0;
	virtual void         setup_lighting(const c_vector& vecCenter) = 0;
	virtual int          draw_static_props(void* pProps, int count, bool bShadowDepth) = 0;
	virtual void         suppress_engine_lighting(bool bSuppress) = 0;
	virtual void         setup_color_meshes(int nTotalVerts) = 0;
	virtual void         setup_lighting_ex(const c_vector& vecCenter, void* handle) = 0;
	virtual bool         get_brightest_shadowing(const c_vector& vecCenter, c_vector& lightPos, c_vector& lightBrightness, bool bAllowNonTaggedLights) = 0;
	virtual void         compute_lighting_state(int nCount, const void* pQuery, void* pState, void** ppEnvCubemapTexture) = 0;
	virtual void         get_model_decal_handles(void* pDecals, int nDecalStride, int nCount, const void* pHandles) = 0;
	virtual void         compute_static_lighting_state(int nCount, const void* pQuery, void* pState, void* pDecalState, void** ppStaticLighting, void** ppEnvCubemapTexture, void* pColorMeshHandles) = 0;
	virtual void         cleanup_static_lighting_state(int nCount, void* pColorMeshHandles) = 0;
};