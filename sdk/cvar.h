#pragma once

class c_concommandbase {
public:
	friend void ConVar_Register(int nCVarFlag, void* pAccessor);
	friend void ConVar_PublishToVXConsole();
	
	c_concommandbase(void);
	c_concommandbase(const char* pName, const char* pHelpString = 0,
		int flags = 0);

	virtual					~c_concommandbase(void);
	virtual	bool			is_command(void) const;
	virtual bool			is_flag_set(int flag) const;
	virtual void			add_flags(int flags);
	virtual const char*		get_name(void) const;
	virtual const char*		get_help_text(void) const;
	const c_concommandbase* get_next(void) const;
	c_concommandbase*		get_next(void);
	virtual bool			is_registered(void) const;
	virtual int				get_dll_identifier() const;
	virtual void			create_base(const char* pName, const char* pHelpString = 0, int flags = 0);
	virtual void			init();
	void					shutdown();
	char*					copy_string(const char* from);

	c_concommandbase* next;
	int isregistered;
	const char* name;
	const char* help_string;
	int	flags;
	c_concommandbase* s_pConCommandBases;
	void* s_pAccessor;
};

class c_iconvar
{
public:
	virtual void			set_value(const char* pValue) = 0;
	virtual void			set_value(float flValue) = 0;
	virtual void			set_value(int nValue) = 0;
	virtual const char*		get_name(void) const = 0;
	virtual bool			is_flag_set(int nFlag) const = 0;
};

class c_convar {
public:
	void set(const char* v) { g_vfunc(void(__thiscall*)(void*, const char*), this, 14)(this, v); }
	void set(float v) { g_vfunc(void(__thiscall*)(void*, float), this, 15)(this, v); }
	void set(int v) { g_vfunc(void(__thiscall*)(void*, int), this, 16)(this, v); }

	float get_float() {
        const auto val = *reinterpret_cast<uint32_t*>(&value);
        auto xored = static_cast<uint32_t>(val ^ reinterpret_cast<uint32_t>(this));
        return *reinterpret_cast<float*>(&xored);
	}

    int get_int()
    {
        const auto val = *reinterpret_cast<uint32_t*>(&n_val);
        auto xored = static_cast<uint32_t>(val ^ reinterpret_cast<uint32_t>(this));
        return *reinterpret_cast<int32_t*>(&xored);
    }

    const char* get_string() {
	    return string;
	}

    char pad0[0x4]{};
    c_convar* next;
    int32_t registered;
    char* name;
    char* help_string;
    int32_t flags;
    char pad1[0x4];
    c_convar* parent;
    char* default_value;
    char* string;
    int32_t string_length;
    float value;
    int32_t n_val;
    int32_t has_min;
    float min_val;
    int32_t has_max;
    float max_val;
    void* change_cbk;
};

class c_concommand : public c_concommandbase {
public:
	virtual ~c_concommand(void);
	virtual	bool is_command(void) const;
	virtual int auto_complete_suggest(const char* partial, void* commands);
	virtual bool can_auto_complete(void);
	virtual void dispatch(const void* command);

	union
	{
		void* command_cbk_v1;
		void* command_cbk;
		void** p_command_cbk;
	};

	union
	{
		void* completion_cbk;
		void** o_completion_cbk;
	};

	bool has_completion_cbk : 1;
	bool using_nc_cbk : 1;
	bool using_nc_interface : 1;
};

class c_cvar : public c_appsystem {
public:
	virtual void*					alloc_dll_identifier() = 0;
	virtual void					register_concommand(c_concommandbase* pCommandBase, int unk = 0) = 0;
	virtual void					unregister_concommand(c_concommandbase* pCommandBase) = 0;
	virtual void					unregister_concommands(void* id) = 0;
	virtual const char*				get_commandline_value(const char* pVariableName) = 0;
	virtual c_concommandbase*		find_commandbase(const char* name) = 0;
	virtual const c_concommandbase* find_commandbase(const char* name) const = 0;
	virtual c_convar*				find_var(const char* var_name) = 0;
	virtual const c_convar*			find_var(const char* var_name) const = 0;
	virtual c_concommand*			find_command(const char* name) = 0;
	virtual const c_concommand*		find_command(const char* name) const = 0;
	virtual void					install_global_change_cbk(void* callback) = 0;
	virtual void					remove_global_change_cbk(void* callback) = 0;
	virtual void					call_global_change_cbk(c_convar* var, const char* pOldString, float flOldValue) = 0;
	virtual void					install_console_display_fn(void* pDisplayFunc) = 0;
	virtual void					remove_console_display_fn(void* pDisplayFunc) = 0;
	virtual void					console_color_printf(const c_color& clr, const char* pFormat, ...) const = 0;
	virtual void					console_printf(const char* pFormat, ...) const = 0;

	void devconsole_color_printf(const c_color& clr, const char* fmt, ...);
	void devconsole_printf(const char* fmt, ...);
};