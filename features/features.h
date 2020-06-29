#pragma once
#include "events.h"
#include "eventlistener.h"
#include "discord.h"
#include "entitylistener.h"

#include "ragebot/lc.h"
#include "ragebot/ragebot.h"
#include "ragebot/fakelag.h"
#include "ragebot/antiaims.h"
#include "ragebot/autowall.h"
#include "ragebot/fakeduck.h"
#include "ragebot/c_sync.h"
#include "ragebot/c_future.h"
#include "ragebot/resolver.h"

#include "visuals/visuals.h"
#include "visuals/glow.h"
#include "visuals/coloredmodels.h"
#include "visuals/effects.h"
#include "visuals/hitlog.h"
#include "visuals/bullettracer.h"
#include "visuals/scope.h"
#include "visuals/speclist.h"
#include "visuals/bombtimer.h"
#include "visuals/indicators.h"
#include "visuals/debuginfo.h"

#include "legitbot/c_legitbot.h"

#include "misc/autojump.h"
#include "misc/misc.h"

#include "skins/skins.h"

class c_features {
public:
	c_visuals* visuals = new c_visuals;
    c_glow* glow = new c_glow;
    c_coloredmodels* coloredmodels = new c_coloredmodels;
    c_hitlog* hitlog = new c_hitlog;
    c_bullettracer* bullettracer = new c_bullettracer;
    c_scope* scope = new c_scope;
    c_speclist* speclist = new c_speclist;
    c_bombtimer* bombtimer = new c_bombtimer;
    c_effects* effects = new c_effects;
    c_indicators* indicators = new c_indicators;
    c_debuginfo* debuginfo = new c_debuginfo;

	c_events* events = new c_events;
	c_eventlistener* listener = new c_eventlistener;
	c_discord* discord = new c_discord;
    c_entitylistener* entlistener = new c_entitylistener;

    c_misc* misc = new c_misc;
    c_autojump* autojump = new c_autojump;
    c_skins* skins = new c_skins;

	c_ragebot* ragebot = new c_ragebot;
	c_fakelag* fakelag = new c_fakelag;
	c_antiaims* antiaims = new c_antiaims;
	c_autowall* autowall = new c_autowall;
	c_fakeduck* fakeduck = new c_fakeduck;
    c_lc* lc = new c_lc;
    c_resolver* resolve = new c_resolver;

    c_sync* prediction = new c_sync;
    c_future* future = new c_future;

    c_legitbot* legitbot = new c_legitbot;
};