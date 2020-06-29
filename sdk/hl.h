#pragma once

class c_hl {
public:
	virtual int				init(void*, void*, void*) = 0;
	virtual void			post_init() = 0;
	virtual void			shutdown(void) = 0;
	virtual bool			replay_init(void* replayFactory) = 0;
	virtual bool			replay_post_init() = 0;
	virtual void			level_init_pre_entity(char const* pMapName) = 0;
	virtual void			level_init_post_entity() = 0;
	virtual void			level_shutdown(void) = 0;
	virtual c_entityinfo*	get_all_classes(void) = 0;
};