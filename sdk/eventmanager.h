#pragma once

class c_event {
public:
	virtual ~c_event() = 0;
	virtual const char*		get_name() const = 0;
	virtual bool			is_reliable() const = 0;
	virtual bool			is_local() const = 0;
	virtual bool			is_empty(const char* keyName = NULL) = 0;
	virtual bool			get_bool(const char* keyName = NULL, bool defaultValue = false) = 0;
	virtual int				get_int(const char* keyName = NULL, int defaultValue = 0) = 0;
	virtual unsigned long	get_ulong(const char* keyName = NULL, unsigned long defaultValue = 0) = 0;
	virtual float			get_float(const char* keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char*		get_string(const char* keyName = NULL, const char* defaultValue = "") = 0;
	virtual const wchar_t*	get_unicode_string(const char* keyName, const wchar_t* defaultValue = L"") = 0;
	virtual void			set_bool(const char* keyName, bool value) = 0;
	virtual void			set_int(const char* keyName, int value) = 0;
	virtual void			set_ulong(const char* keyName, unsigned long value) = 0;
	virtual void			set_float(const char* keyName, float value) = 0;
	virtual void			set_string(const char* keyName, const char* value) = 0;
	virtual void			set_unicode_string(const char* keyName, const wchar_t* value) = 0;
};

class c_eventinfo
{
public:
    enum
    {
        EVENT_INDEX_BITS = 8,
        EVENT_DATA_LEN_BITS = 11,
        MAX_EVENT_DATA = 192,
    };

    short           classID;
    float           fire_delay;
    const void *    pSendTable;
    const void *    pClientClass;
    int		        bits;
    byte	        *pData;
    int		        flags;
    char            filter[15];
};

class c_baseeventlistener {
public:
	virtual			~c_baseeventlistener(void) {};
	virtual void	fire_game_event(c_event* event) = 0;
	virtual int		get_event_debug_id() { return 42; }
};

class c_eventmanager {
public:
	virtual				~c_eventmanager() = 0;
	virtual int			load_events_from_file(const char* filename) = 0;
	virtual void		reset() = 0;
	virtual bool		add_listener(c_baseeventlistener* listener, const char* name, bool bServerSide) = 0;
	virtual bool		find_listener(c_baseeventlistener* listener, const char* name) = 0;
	virtual int			remove_listener(c_baseeventlistener* listener) = 0;
	virtual c_event*	create_game_event(const char* name, bool bForce, unsigned int dwUnknown) = 0;
	virtual bool		fire_event(c_event* event, bool bDontBroadcast = false) = 0;
	virtual bool		fire_event_clientside(c_event* event) = 0;
	virtual c_event*	dublicate_event(c_event* event) = 0;
	virtual void		free_event(c_event* event) = 0;
	virtual bool		serialize_event(c_event* event, void* buf) = 0;
	virtual c_event*	unserialize_event(void* buf) = 0;
};