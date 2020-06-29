#include "../cheat.h"
#define fail(err) { SetLastError(err); return false; }

#include "celshader.h"

using namespace std::chrono_literals;
void c_core::initialize() {
	if (!verify()) memset(0, 0, 1);

	std::this_thread::sleep_for(250ms);

    if (!load_interfaces_dx()) crash(str("Load failed - error 8. Please contact support"));
    if (!load_dx()) crash(str("Load failed - error 4. Please contact support"));

    is_loadanim_shown = true;

    is_dx_loaded = true;

    loadanim_text = str("Initializing...");

	if (!load_interfaces())
	    crash(str("Load failed - error 2. Please contact support"));
	else{
        csgo->cvar->console_color_printf(c_color(239, 9, 95), str("uc-sflying "));
        csgo->cvar->console_printf(str("| initializing... (20%/100%)\n"));
	}

	csgo->engine->client_cmd_unrestricted(str("clear"));
	std::this_thread::sleep_for(250ms);

	if (!load_netvars())
	    crash(str("Load failed - error 5. Please contact support"));
	else{
            csgo->cvar->console_color_printf(c_color(239, 9, 95), str("uc-sflying "));
            csgo->cvar->console_printf(str("| initializing... (30%/100%)\n"));
	}
	if (!load_files())
	    crash(str("Load failed - error 1. Please contact support"));
	else{
        csgo->cvar->console_color_printf(c_color(239, 9, 95), str("uc-sflying "));
        csgo->cvar->console_printf(str("| initializing... (40%/100%)\n"));
	}
	if (!load_hooks())
	    crash(str("Load failed - error 3. Please contact support"));
	else{
        csgo->cvar->console_color_printf(c_color(239, 9, 95), str("uc-sflying "));
        csgo->cvar->console_printf(str("| initializing... (50%/100%)\n"));
	}
	if (!load_others())
	    crash(str("Load failed - error 6. Please contact support"));
    else{
        csgo->cvar->console_color_printf(c_color(239, 9, 95), str("uc-sflying "));
        csgo->cvar->console_printf(str("| initializing... (66%/100%)\n"));
    }

	features->listener->initialize();
	features->entlistener->initialize();

	std::this_thread::sleep_for(250ms);

	auto cheats = csgo->cvar->find_var(str("sv_cheats"));

	auto dummy = (c_convar*)malloc(sizeof(c_convar));
	if (!dummy)
		crash(str("Load failed - error 7. Please contact support"));
	else{
        csgo->cvar->console_color_printf(c_color(239, 9, 95), str("uc-sflying "));
        csgo->cvar->console_printf(str("| initializing... (87%/100%)\n"));
	}

	memset(dummy, 0, sizeof(c_convar));
	memcpy(dummy, cheats, sizeof(c_convar));
	dummy->next = nullptr;

	csgo->cvar->register_concommand((c_concommandbase*)dummy);

	DWORD old_protect = 0;
	VirtualProtect((void*)cheats->name, 128, PAGE_READWRITE, &old_protect);
	strcpy((char*)cheats->name, str("old_cheats"));
	VirtualProtect((void*)cheats->name, 128, old_protect, &old_protect);

	cheats->flags = 0;
	cheats->set(1);
	if(cheats->get_int() == 1){
        csgo->cvar->console_color_printf(c_color(239, 9, 95), str("uc-sflying "));
        csgo->cvar->console_printf(str("| initializing... (100%/100%)\n"));
	}

	csgo->cvar->console_color_printf(c_color(239, 9, 95), str("uc-sflying "));
	csgo->cvar->console_printf(str("| welcome %s\n"), username);
	features->events->add(str("welcome %s"), username);

	is_loadanim_shown = false;
	is_fully_loaded = true;

#ifndef _DEBUG
	static std::string last_update = api->get_update_time();
	if (last_update == str("Invalid"))
	    memset(0, 0, 1);

	std::thread([&]() {
        while (true) {
            auto time = api->get_update_time();

            if (time == str("Invalid"))
                memset(0, 0, 1);

            if (last_update != time)
                menu->wnd->get_parent()->add_message(str("Update"), str("A new update is live! Reload cheat and get access to new features"));

            std::this_thread::sleep_for(1min);
        }
	}).detach();
#endif

	while (!menu->wnd)
	    std::this_thread::sleep_for(100ms);

	for (auto i = 0; i < config->slots.size(); i++) {
	    if (config->slots.at(i) == str("Default")) {
	        config->selected_slot = i;
	        config->load();
	    }
	}

	menu->wnd->find(str("Extensions"), str("Loader"), str("Refresh"))->call_callback();
    scripting->load(str("autorun"));
}

void c_core::crash(const char* message, ...) {
    auto wnd = FindWindowA(str("Valve001"), 0);
    if (wnd)
        ShowWindow(wnd, SW_HIDE);

	char buffer[2048] = { 0 };

	va_list args;
	va_start(args, message);
	vsprintf(buffer, message, args);
	va_end(args);

	MessageBoxA(0, buffer, str("uc-sflying | Crashed!"), MB_OK | MB_ICONERROR | MB_APPLMODAL);
	memset(0, 0, 1);
}

bool c_core::verify() {
#ifndef _DEBUG
	if (!api->connect()) fail(1);

	if (!std::filesystem::exists(str("c:/uc-sflying"))) fail(2);
	if (!std::filesystem::exists(str("c:/uc-sflying/cfg"))) fail(3);
	if (!std::filesystem::exists(str("c:/uc-sflying/ext"))) fail(4);
	if (!std::filesystem::exists(str("c:/uc-sflying/ldr"))) fail(5);

	GetPrivateProfileStringA(str("l"), str("a"), "", username, 64, str("c:/uc-sflying/ldr/_"));
	GetPrivateProfileStringA(str("l"), str("b"), "", password, 64, str("c:/uc-sflying/ldr/_"));

	if (!username[0] || !password[0]) fail(6);
	if (!api->authenticate(username, base64_decode(password))) fail(7);
	if (!api->verify()) fail(8);
#endif

	return true;
}

bool c_core::load_files() {

    std::ofstream("csgo/materials/csgo_shaded_glow_walls.vmt") << R"#("VertexLitGeneric" {
	    "$additive" "1"
	    "$envmap" "models/effects/cube_white"
	    "$envmaptint" "[1 1 1]"
	    "$envmapfresnel" "1"
	    "$envmapfresnelminmaxexp" "[0 1 2]"
	    "$alpha" "0.8"
        "$ignorez" "1"
    })#";
    std::ofstream("csgo/materials/csgo_shaded_glow_normal.vmt") << R"#("VertexLitGeneric" {
	    "$additive" "1"
	    "$envmap" "models/effects/cube_white"
	    "$envmaptint" "[1 1 1]"
	    "$envmapfresnel" "1"
	    "$envmapfresnelminmaxexp" "[0 1 2]"
	    "$alpha" "0.8"
        "$ignorez" "0"
    })#";
	std::ofstream(str("csgo/materials/csgo_reflective_normal.vmt")) << str(R"#("VertexLitGeneric" {
        "$basetexture" "vgui/white_additive"
        "$envmap" "env_cubemap"
        "$envmaptint" "[.10 .10 .10]"
        "$phong" "1"
        "$phongexponent" "200"
        "$phongboost" "1.0"
        "$rimlight" "1"
        "$nofog" "1"
        "$model" "1"
        "$nocull" "0"
        "$lightwarptexture" "metalic"
        "$selfillum" "1"
        "$halflambert" "1"
        "$ignorez" "0"
    })#");
	std::ofstream(str("csgo/materials/csgo_reflective_walls.vmt")) << str(R"#("VertexLitGeneric" {
        "$basetexture" "vgui/white_additive"
        "$envmap" "env_cubemap"
        "$envmaptint" "[.10 .10 .10]"
        "$phong" "1"
        "$phongexponent" "200"
        "$phongboost" "1.0"
        "$rimlight" "1"
        "$nofog" "1"
        "$model" "1"
        "$nocull" "0"
        "$lightwarptexture" "metalic"
        "$selfillum" "1"
        "$halflambert" "1"
	    "$ignorez" "1"
    })#");
    std::ofstream(str("csgo/materials/csgo_celshaded_normal.vmt")) << str(R"#("VertexLitGeneric" {
        "$basetexture" "vgui/white_additive"
        "$envmap" "env_cubemap"
        "$envmaptint" "[.10 .10 .10]"
        "$nofog" "1"
        "$model" "1"
        "$nocull" "0"
        "$halflambert" "0"
        "$lightwarptexture" "celshader"
        "$ignorez" "0"
    })#");
    std::ofstream(str("csgo/materials/csgo_celshaded_walls.vmt")) << str(R"#("VertexLitGeneric" {
        "$basetexture" "vgui/white_additive"
        "$envmap" "env_cubemap"
        "$envmaptint" "[.10 .10 .10]"
        "$nofog" "1"
        "$model" "1"
        "$nocull" "0"
        "$halflambert" "0"
        "$lightwarptexture" "celshader"
	    "$ignorez" "1"
    })#");
	std::ofstream(str("csgo/materials/csgo_default_normal.vmt")) << str(R"#("VertexLitGeneric" {
		"$basetexture" "vgui/white_additive"
		"$ignorez" "0"
		"$envmap" ""
		"$normalmapalphaenvmapmask" "1"
		"$envmapcontrast"  "1"
		"$nofog" "1"
		"$model" "1"
		"$nocull" "0"
		"$selfillum" "1"
		"$halflambert" "1"
		"$znearer" "0"
		"$flat" "1" 
	})#");
	std::ofstream(str("csgo/materials/csgo_default_walls.vmt")) << str(R"#("VertexLitGeneric" {
		"$basetexture" "vgui/white_additive"
		"$ignorez" "1"
		"$envmap" ""
		"$normalmapalphaenvmapmask" "1"
		"$envmapcontrast"  "1"
		"$nofog" "1"
		"$model" "1"
		"$nocull" "0"
		"$selfillum" "1"
		"$halflambert" "1"
		"$znearer" "0"
		"$flat" "1" 
	})#");
	std::ofstream(str("csgo/materials/csgo_flat_normal.vmt")) << str(R"#("UnlitGeneric" {
		"$basetexture" "vgui/white_additive"
		"$ignorez" "0"
		"$envmap" ""
		"$normalmapalphaenvmapmask" "1"
		"$envmapcontrast"  "1"
		"$nofog" "1"
		"$model" "1"
		"$nocull" "0"
		"$selfillum" "1"
		"$halflambert" "1"
		"$znearer" "0"
		"$flat" "1" 
	})#");
	std::ofstream(str("csgo/materials/csgo_flat_walls.vmt")) << str(R"#("UnlitGeneric" {
		"$basetexture" "vgui/white_additive"
		"$ignorez" "1"
		"$envmap" ""
		"$normalmapalphaenvmapmask" "1"
		"$envmapcontrast"  "1"
		"$nofog" "1"
		"$model" "1"
		"$nocull" "0"
		"$selfillum" "1"
		"$halflambert" "1"
		"$znearer" "0"
		"$flat" "1" 
	})#");
	std::ofstream(str("csgo/materials/csgo_glow_normal.vmt")) << str(R"#("VertexLitGeneric" {
		"$additive" "1"
		"$envmap" "models/effects/cube_white"
		"$envmaptint" "[1 1 1]"
		"$envmapfresnel" "1"
		"$envmapfresnelminmaxexp" "[0 1 2]"
		"$alpha" "0.8"
	})#");
	std::ofstream(str("csgo/materials/csgo_glow_walls.vmt")) << str(R"#("VertexLitGeneric" {
		"$ignorez" "1"
		"$additive" "1"
		"$envmap" "models/effects/cube_white"
		"$envmaptint" "[1 1 1]"
		"$envmapfresnel" "1"
		"$envmapfresnelminmaxexp" "[0 1 2]"
		"$alpha" "0.8"
	})#");
    std::ofstream(str("csgo\\materials\\glowOverlay.vmt")) << str (R"#("VertexLitGeneric" {

    	"$additive" "1"
    	"$envmap" "models/effects/cube_white"
    	"$envmaptint" "[1 1 1]"
    	"$envmapfresnel" "1"
    	"$envmapfresnelminmaxexp" "[0 1 2]"
    	"$alpha" "0.8"
        "$ignorez" "1"
    })#");
	return true;
}

bool c_core::load_interfaces_dx() {
    static auto create_interface = [](std::string iv, std::string dllname, bool isExact = false) -> void* {
        auto CreateInterface = (void* (*)(const char*, int*))
                GetProcAddress(GetModuleHandleA(dllname.c_str()), str("CreateInterface"));
        if (!CreateInterface)
            return 0;

        if (isExact)
            return CreateInterface(iv.c_str(), 0);
        else {
            char buffer[128] = { 0 };
            for (int i = 0; i < 1000; i++) {
                sprintf(buffer, str("%s%03i"), iv.c_str(), i);

                auto iface = CreateInterface(buffer, 0);
                if (iface)
                    return iface;
            }
        }

        return 0;
    };

    csgo->hl = g_cast(c_hl*, create_interface(CSGO_1, CSGO_2)); if (!csgo->hl) return false;
    csgo->engine = g_cast(c_engine*, create_interface(CSGO_ENGINE_0, CSGO_ENGINE_1)); if (!csgo->engine) return false;

    csgo->globals = **g_cast(c_globals***, ((*(unsigned int**)(csgo->hl))[0] + 0x1b)); if (!csgo->globals) return false;

    return true;
}

bool c_core::load_interfaces() {
	static auto create_interface = [](std::string iv, std::string dllname, bool isExact = false) -> void* {
		auto CreateInterface = (void* (*)(const char*, int*))
			GetProcAddress(GetModuleHandleA(dllname.c_str()), str("CreateInterface"));
		if (!CreateInterface)
			return 0;

		if (isExact)
			return CreateInterface(iv.c_str(), 0);
		else {
			char buffer[128] = { 0 };
			for (int i = 0; i < 1000; i++) {
				sprintf(buffer, str("%s%03i"), iv.c_str(), i);

				auto iface = CreateInterface(buffer, 0);
				if (iface)
					return iface;
			}
		}

		return 0;
	};

	auto client_panorama = std::string(CSGO_2);
	auto engine = std::string(CSGO_ENGINE_1);

	csgo->cvar = g_cast(c_cvar*, create_interface(CSGO_3, CSGO_4)); if (!csgo->cvar) return false;
	csgo->events = g_cast(c_eventmanager*, create_interface(CSGO_5, engine, true)); if (!csgo->events) return false;
	csgo->entities = g_cast(c_entitylist*, create_interface(CSGO_6, client_panorama)); if (!csgo->entities) return false;
	csgo->modelrender = g_cast(c_modelrender*, create_interface(CSGO_7, engine)); if (!csgo->modelrender) return false;
	csgo->materialsystem = g_cast(c_materialsystem*, create_interface(CSGO_8, CSGO_9)); if (!csgo->materialsystem) return false;
	csgo->renderview = g_cast(c_renderview*, create_interface(CSGO_10, engine)); if (!csgo->renderview) return false;
	csgo->debugoverlay = g_cast(c_debugoverlay*, create_interface(CSGO_11, engine)); if (!csgo->debugoverlay) return false;
	csgo->modelinfo = g_cast(c_modelinfo*, create_interface(CSGO_12, engine)); if (!csgo->modelinfo) return false;
	csgo->enginetrace = g_cast(c_enginetrace*, create_interface(CSGO_13, engine)); if (!csgo->enginetrace) return false;
	csgo->physics = g_cast(c_physics*, create_interface(CSGO_14, CSGO_15)); if (!csgo->physics) return false;
	csgo->movement = g_cast(c_movement*, create_interface(CSGO_16, client_panorama)); if (!csgo->movement) return false;
	csgo->prediction = g_cast(c_prediction*, create_interface(CSGO_17, client_panorama)); if (!csgo->prediction) return false;
	csgo->surface = g_cast(c_surface*, create_interface(CSGO_18, CSGO_19)); if (!csgo->surface) return false;
	csgo->localize = g_cast(c_localize*, create_interface(CSGO_35, CSGO_20)); if (!csgo->localize) return false;

	csgo->panel = g_cast(void*, create_interface(CSGO_21, CSGO_22)); if (!csgo->panel) return false;
	csgo->enginesound = g_cast(void*, create_interface(CSGO_23, engine)); if (!csgo->enginesound) return false;

	csgo->glowmanager = *(c_glowmanager**)(utils->scan(client_panorama.c_str(), CSGO_24) + 3);
	if (!csgo->glowmanager) return false;

	csgo->movehelper = **(c_move***)(utils->scan(client_panorama.c_str(), CSGO_25) + 2);
	if (!csgo->movehelper) return false;

	csgo->beamrenderer = *(c_beamrenderer**)(utils->scan(client_panorama.c_str(), CSGO_26) + 1);
    if (!csgo->beamrenderer) return false;

    csgo->clientstate = **(c_clientstate***)(utils->scan(engine.c_str(), CSGO_27) + 2);
    if (!csgo->clientstate) return false;

    csgo->input = *(c_input**)(utils->scan(client_panorama.c_str(), CSGO_28) + 1);
    if (!csgo->input) return false;

	csgo->clientmode = **g_cast(void***, ((*(unsigned int**)(csgo->hl))[10] + 0x5)); if (!csgo->clientmode) return false;

    csgo->memory = *reinterpret_cast<c_enginememory**>(GetProcAddress(GetModuleHandleA(str("tier0.dll" )), str("g_pMemAlloc"))); if (!csgo->memory) return false;

	return true;
}

bool c_core::load_dx() {
	hide_from_obs = GetPrivateProfileIntA(str("csgo"), str("a"), 0, str("c:/uc-sflying/cfg/_"));
	if (hide_from_obs == 1) {
		auto present_addr = utils->scan(str("gameoverlayrenderer.dll"), str("FF 15 ? ? ? ? 8B F8 85 DB")) + 2;
		auto reset_addr = utils->scan(str("gameoverlayrenderer.dll"), str("FF 15 ? ? ? ? 8B F8 85 FF 78 18")) + 2;

		if (present_addr == 2 || reset_addr == 2)
			return false;

		oPresent = ***(fnPresent**)present_addr;
		oReset = ***(fnReset**)reset_addr;

		**(void***)present_addr = &hkPresent;
		**(void***)reset_addr = &hkReset;
	}
	else {
		auto dx_device = utils->scan(CSGO_31, CSGO_32);
		if (!dx_device)
			return false;

		auto dx = new c_hookable(**(void***)(dx_device + 1));
		if (!dx)
			return false;

		oReset = (fnReset)dx->orig(16);
		oPresent = (fnPresent)dx->orig(17);

		dx->hook(16, hkReset);
		dx->hook(17, hkPresent);
	}

	oWndProc = (WNDPROC)SetWindowLongA(FindWindow(CSGO_33, 0), GWL_WNDPROC, (long)&hkWndProc);

	return true;
}

static uint32_t get_bool_retn_address = 0;

__declspec(naked) bool get_bool(void* ecx, void* edx)
{
    _asm
    {
        push eax

        mov eax, [esp + 4]
        mov get_bool_retn_address, eax

        push get_bool_retn_address
        push ecx
        call hkCamera

        cmp eax, 0
        je _retn0

        cmp eax, 1
        je _retn1

        cmp eax, 2
        je _retn2

        _retn0 :
        mov al, 0
        retn

        _retn1 :
        pop eax

        mov eax, get_bool_retn_address
        add eax, 0x462

        push eax

        retn

        _retn2 :
        pop eax

        mov eax, get_bool_retn_address
        add eax, 0x462

        push eax

        retn
    }
}

bool c_core::load_hooks() {
	auto panel = new c_hookable(csgo->panel); if (!panel) return false;
	auto surface = new c_hookable(csgo->surface); if (!surface) return false;
	auto clientmode = new c_hookable(csgo->clientmode); if (!clientmode) return false;
	auto modelrender = new c_hookable(csgo->modelrender); if (!modelrender) return false;
	auto hl = new c_hookable(csgo->hl); if (!hl) return false;
	auto bsp = new c_hookable(csgo->engine->get_bsp_tree_query()); if (!bsp) return false;
	auto engine = new c_hookable(csgo->engine); if (!engine) return false;
	auto pred = new c_hookable(csgo->prediction); if (!pred) return false;
	auto clientstate = new c_hookable((c_clientstate*)((unsigned long long)csgo->clientstate + 8)); if (!clientstate) return false;
    auto camera = new c_hookable(csgo->cvar->find_var(str("cl_camera_height_restriction_debug"))); if (!camera) return false;
    auto grenade = new c_hookable(csgo->cvar->find_var(str("cl_grenadepreview"))); if (!grenade) return false;
    auto player = new c_hookable((void*)(utils->scan(CSGO_2, CSGO_34) + 0x47));
	if (!player) return false;

    camera->hook(13, get_bool);
    grenade->hook(13, grenade_preview_get_bool);
	oPaintTraverse = (fnPaintTraverse)panel->orig(41);
	oLockCursor = (fnLockCursor)surface->orig(67);
	oCreateMove = (fnCreateMove)clientmode->orig(24);
	oDoPostScreenSpaceEffects = (fnDoPostScreenSpaceEffects)clientmode->orig(44);
	oDrawModelExecute = (fnDrawModelExecute)modelrender->orig(21);
	oFrameStageNotify = (fnFrameStageNotify)hl->orig(37);
	oListLeavesInBox = (fnListLeavesInBox)bsp->orig(6);
	oOverrideView = (fnOverrideView)clientmode->orig(18);
	oGetScreenAspectRatio = (fnGetScreenAspectRatio)engine->orig(101);
	oDoExtraBonesProcessing = (fnDoExtraBonesProcessing)player->orig(197);
	oEyeAngles = (fnEyeAngles)player->orig(169);
	oUpdateClientsideAnimation = (fnUpdateClientsideAnimation)player->orig(223);
	oStandartBlendingRules = (fnStandartBlendingRules)player->orig(205);
	oGetViewModelFOV = (fnGetViewModelFOV)clientmode->orig(35);
    oPacketStart = (fnPacketStart)clientstate->orig(5);
    oFireEvent = (fnFireEvent)engine->orig(59);
    oWriteUsercmdDeltaToBuffer = (fnWriteUsercmdDeltaToBuffer)hl->orig(24);
    oIN_KeyEvent = (fnIN_KeyEvent)hl->orig(21);
    oHlCreateMove = (fnHlCreateMove)hl->orig(22);
    oRunCommand = (fnRunCommand)pred->orig(19);
    oGrenadePreview_get_bool = (fnClGrenadePreview_get_bool)grenade->orig(13);
    //oPostDataUpdate = (fnPostDataUpdate)player->orig(411);

	panel->hook(41, hkPaintTraverse);
	surface->hook(67, hkLockCursor);
	clientmode->hook(24, hkCreateMove);
	clientmode->hook(44, hkDoPostScreenSpaceEffects);
	clientmode->hook(18, hkOverrideView);
	clientmode->hook(35, hkGetViewModelFOV);
	modelrender->hook(21, hkDrawModelExecute);
	hl->hook(37, hkFrameStageNotify);
	hl->hook(24, hkWriteUsercmdDeltaToBuffer);
	hl->hook(22, hkHlCreateMove);
	hl->hook(21, hkIN_KeyEvent);
	bsp->hook(6, hkListLeavesInBox);
	engine->hook(101, hkGetScreenAspectRatio);
	engine->hook(59, hkFireEvent);
	player->hook(197, hkDoExtraBonesProcessing);
	player->hook(169, hkEyeAngles);
	player->hook(223, hkUpdateClientsideAnimation);
	player->hook(205, hkStandartBlendingRules);
	//player->hook(411, hkPostDataUpdate);
	pred->hook(19, hkRunCommand);
    clientstate->hook(5, hkPacketStart);



    oCL_SendMove = (fnCL_SendMove)DetourFunction((PBYTE)utils->scan(str("engine.dll"),
            str("55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98 ? ? ? ? 56 8B B0 ? ? ? ? 43 A1 ? ? ? ? 03 DE 57 6A 00"
                " 8B 40 34 FF D0 84 C0 0F 85 ? ? ? ? 0F 28 05 ? ? ? ? 8D 4D B0 0F 11 45 E4 66 C7 45 ? ? ? C7 45 ? ? ? ? ? C7 45 "
                "? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 33 D2 C7 45 ? ? ? ? ? 83 C6 01 C7 45 ? ? ? ? ? B9 ? ? ? ? C7 45 ? ? ? ?"
                " ? 8D 85 ? ? ? ? 89 4D BC 88 55 C8 C7 45 ? ? ? ? ? 89 55 DC 88 55 CC 89 45 E4 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? "
                "89 55 F0 88 55 F4 79 04 33 F6 EB 0A")), (PBYTE)hkSendMove);

    DetourFunction((PBYTE)utils->scan(str("client_panorama.dll"), str("57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02 5F C3")), (PBYTE)hkShouldSkipAnimFrame);

	return true;
}

std::unordered_map<std::string, c_netvar> get_childs(c_netvar parent, c_recvtable* table) {
	std::unordered_map<std::string, c_netvar> thischilds;

	for (int i = 0; i < table->num; i++) {
		auto prop = &table->props[i];
		auto child = prop->table;

		std::unordered_map<std::string, c_netvar> childs;
		auto nv = c_netvar{ parent.offset + prop->offset, prop->type, childs };

		if (child)
			nv.child = get_childs(nv, child);

		thischilds[prop->name] = nv;
	}

	return thischilds;
}

void recursive(c_recvtable* table) {
	for (int i = 0; i < table->num; i++) {
		auto prop = &table->props[i];
		auto child = prop->table;

		std::unordered_map<std::string, c_netvar> childs;
		auto nv = c_netvar{ prop->offset, prop->type, childs };

		if (child)
			nv.child = get_childs(nv, child);

		csgo->netvars[table->name][prop->name] = nv;
	}
}

bool c_core::load_netvars() {
	auto classes = csgo->hl->get_all_classes();
	if (!classes)
		return false;

	int tmp = 0;
	while (classes) {
		auto table = classes->table;
		if (table)
			recursive(table);

		classes = classes->next;
	}

	return true;
}

bool c_core::load_others() {
	if (!scripting->initialize()) return false;

	features->skins->load();
	return true;
}

void recursive_dump(std::unordered_map<std::string, c_netvar> nvs, int tabOffset, std::string& text) {
	for (auto n : nvs) {
		for (int i = 0; i < tabOffset; i++)
			text += str("\t");

		text += n.first;

		switch (n.second.type) {
		case DPT_Int: text += str(" [int]"); break;
		case DPT_Float: text += str(" [float]"); break;
		case DPT_Vector: text += str(" [vector]"); break;
		case DPT_VectorXY: text += str(" [vector2d]"); break;
		case DPT_String: text += str(" [string]"); break;
		case DPT_Array: text += str(" [array]"); break;
		case DPT_DataTable: text += str(" [dt]"); break;
		}

		if (n.second.child.size() > 0) {
			text += str(" -> \n");

			recursive_dump(n.second.child, tabOffset + 1, text);
		}
		else
			text += str("\n");
	}
}

void c_core::dump_netvars() {
	std::string text;
	for (auto dt : csgo->netvars) {
		text += dt.first;

		if (dt.second.size() > 0) {
			text += str(" -> \n");

			recursive_dump(dt.second, 1, text);
		}
		else
			text += str("\n");
	}

	std::ofstream(str("_NETVAR_DUMP.txt")) << text;
}