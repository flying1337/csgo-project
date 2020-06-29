#pragma once

class c_entitylist {
public:
	virtual c_networkable*	get_networkable(int entnum) = 0;
	virtual c_networkable*	get_networkable_handle(c_ehandle hEnt) = 0;
	virtual c_unknown*		get_unknown(c_ehandle hEnt) = 0;
	virtual c_entity*		get_entity(int entnum) = 0;
	virtual c_entity*		get_entity_handle(c_ehandle hEnt) = 0;
	virtual int				number_of_entities(bool bIncludeNonNetworkable) = 0;
	virtual int				get_highest_index(void) = 0;
	virtual void			set_max_entities(int maxents) = 0;
	virtual int				get_max_entities() = 0;
};

class c_entity_info
{
public:
	c_handle* entity;
	int	serial_number;
	c_entity_info* prev;
	c_entity_info* next;
};

class c_baseentitylist {
public:
	virtual void on_add(c_handle* pEnt, c_ehandle handle) = 0;
	virtual void on_remove(c_handle* pEnt, c_ehandle handle) = 0;

	class c_entity_info_list
	{
	public:
		c_entity_info* head;
		c_entity_info* tail;
	};

	c_entity_info ent_ptr_array[1 << 12];
	c_entity_info_list active_list;
	c_entity_info_list free_list;
};

class c_entitylist_listener
{
public:
	virtual void on_created(c_entity* pEntity) {};
	virtual void on_deleted(c_entity* pEntity) {};
};

class c_cliententitylist : public c_baseentitylist, public c_entitylist {
public:
    virtual                 ~c_cliententitylist() = 0;
    virtual c_networkable*  get_networkable(int entnum) = 0;
    virtual c_entity*		get_entity(int entnum) = 0;
    virtual int				number_of_entities(bool bIncludeNonNetworkable = false) = 0;
    virtual c_unknown*		get_unknown_handle(c_ehandle hEnt) = 0;
    virtual c_networkable*  get_networkable_handle(c_ehandle hEnt) = 0;
    virtual c_entity*		get_entity_handle(c_ehandle hEnt) = 0;
    virtual int				get_highest_index(void) = 0;
    virtual void			set_max_entities(int maxents) = 0;
    virtual int				get_max_entities() = 0;
    virtual void			on_add_entity(c_handle* pEnt, c_ehandle handle) = 0;
    virtual void			on_remove_entity(c_handle* pEnt, c_ehandle handle) = 0;

    CUtlVector<c_entitylist_listener*> listeners;
};