#pragma once
class c_material;

class c_materialvar
{
public:
    virtual void                unk() = 0;
    virtual void*               get_texture( void ) = 0;
    virtual char const *	    get_name( void ) const = 0;
    virtual void*	            get_name_as_sym() const = 0;
    virtual void			    set_float( float val ) = 0;
    virtual void			    set_int( int val ) = 0;
    virtual void			    set_string( char const *val ) = 0;
    virtual char const *	    get_string( void ) const = 0;
    virtual void			    set_long( unsigned long type, void *pData ) = 0;
    virtual void			    get_long( unsigned long *type, void **ppData ) = 0;
    virtual void			    set_vec( float const* val, int numcomps ) = 0;
    virtual void			    set_vec( float x, float y ) = 0;
    virtual void			    set_vec( float x, float y, float z ) = 0;
    virtual void			    set_vec( float x, float y, float z, float w ) = 0;
    virtual void			    get_vec( float *val, int numcomps ) const = 0;
    virtual void			    set_texture( void * ) = 0;
    virtual c_material *	    get_material( void ) = 0;
    virtual void			    set_material( c_material * ) = 0;
    virtual bool			    is_defined() const = 0;
    virtual void			    set_undefined() = 0;
    virtual void			    set_matrix( c_matrix4x4 const& matrix ) = 0;
    virtual const c_matrix4x4&  get_matrix( ) = 0;
    virtual bool			    matrix_is_identity() const = 0;
    virtual void			    copy_from( c_materialvar *pMaterialVar ) = 0;
    virtual void			    set_value( char const *val ) = 0;
    virtual c_material *		get_owning_material() = 0;
    virtual void			    set_vec_component( float fVal, int nComponent ) = 0;
    virtual int				    get_int_internal( void ) const = 0;
    virtual float			    get_float_internal( void ) const = 0;
    virtual float const*	    get_vec_internal( ) const = 0;
    virtual void			    get_vec_value_internal( float *val, int numcomps ) const = 0;
    virtual int				    vector_size_internal() const = 0;

	char* string_value;
	int int_value;
	float vec_value[4];
	short type : 4;
	short vector_comps : 3;
	short fake_mat_var : 1;
	short temp_index;
	char* name;
};

enum e_materialvarflags
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_IGNORE_ALPHA_MODULATION = (1 << 30),
};

class c_material {
public:
	virtual const char*		get_name() const = 0;
	virtual const char*		get_texture_group_name() const = 0;
	virtual void*			get_pi_properties(int* width, int* height, void* imageFormat, bool* isTranslucent) const = 0;
	virtual void*			get_pi(unsigned char* data, int width, int height, void* imageFormat) const = 0;
	virtual int				get_mapping_w() = 0;
	virtual int				get_mapping_h() = 0;
	virtual int				get_num_anim_frames() = 0;
	virtual bool			in_material_page(void) = 0;
	virtual	void			get_mat_offset(float* pOffset) = 0;
	virtual void			get_mat_scale(float* pScale) = 0;
	virtual c_material*		get_mat_page(void) = 0;
	virtual c_materialvar*	find_var(const char* varName, bool* found, bool complain = true) = 0;
	virtual void			inc_ref(void) = 0;
	virtual void			dec_ref(void) = 0;
	virtual int 			get_enum_id(void) const = 0;
	virtual void			get_color_sample(float s, float t, float* color) const = 0;
	virtual void			recompute() = 0;
	virtual bool			is_transcluent() = 0;
	virtual bool			is_alpha_tested() = 0;
	virtual bool			is_vertex_lit() = 0;
	virtual void*			get_vertex_fmt() const = 0;
	virtual bool			has_proxy(void) const = 0;
	virtual bool			uses_env_cubemap(void) = 0;
	virtual bool			needs_tg_space(void) = 0;
	virtual bool			omgsolongfunc(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			fuckthisshit(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			needs_software_skinning(void) = 0;
	virtual void			alpha_modulate(float alpha) = 0;
	virtual void			color_modulate(float r, float g, float b) = 0;
	virtual void			set_flag(e_materialvarflags flag, bool on) = 0;
	virtual bool			get_flag(e_materialvarflags flag) const = 0;
};

class c_materialsystem {
public:
	c_material* find_material(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL);
	unsigned short first_material();
	unsigned short next_material(unsigned short h);
	unsigned short invalid_material();
	c_material* get_material(unsigned short h);
};