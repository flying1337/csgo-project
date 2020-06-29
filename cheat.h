#ifndef uc-sflying-cm-cheat-h
#define uc-sflying-cm-cheat-h
#pragma warning(disable: 4244)

//#define _BETA

#ifdef a
#define g_crash_start() crInstallToCurrentThread2(0)
#define g_crash_start_hk() static auto crash_rpt_init = false; if (!crash_rpt_init) { g_crash_start(); crash_rpt_init = true; }
#define g_crash_end() crUninstallFromCurrentThread()
#else
#define g_crash_start() ((void)0)
#define g_crash_start_hk() ((void)0)
#define g_crash_end() ((void)0)
#endif

#define g_new(t, n) t* n = new t;
#define g_ext(t, n) extern t* n;
#define g_toggle(v) v = !v
#define g_cast(t, v) (t)v
#define g_vfunc(t, o, i) ((t)((*(int**)o)[i]))
#define g_offset_ptr(c, v, o) (c*)((unsigned int)v + o)
#define g_offset(c, v, o) *(c*)((unsigned int)v + o)
#define g_pad(n, s) char n[s];
#define g_cfgid(t, n, v) (((ptr)&config->n.v - (ptr)&config->n) / sizeof(t) - 1)
#define g_str const std::string&
#define g_refcounter() unsigned int ref = 0; \
void add_ref() { ref++; } \
void release() { if (--ref == 0) delete this; }

#define Assert(x) ((void)0)

extern size_t csgo_memory_get_size(void* m);

#define FORWARD_DECLARE_HANDLE(name) typedef struct name##__ *name

typedef unsigned int ptr;
typedef unsigned int uint32;

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <Psapi.h>
#include <thread>
#include <filesystem>
#include <comdef.h>
#include <WbemIdl.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <sol.hpp>
#include <typeinfo>
#include <detours.h>

#define IMGUI_DEFINE_MATH_OPERATORS

#include "core/base64.h"
#include "core/md5.h"
#include "core/hwid.h"
#include "core/tcp.h"
#include "core/api.h"
#include "core/protname.h"

#include "ui/imgui.h"
#include "ui/imgui_freetype.h"
#include "ui/imgui_impl_dx9.h"
#include "ui/imgui_impl_win32.h"
#include "ui/imgui_internal.h"
#include "ui/ui/c_ui.h"

#include "tools/fnv.h"
#include "tools/strenc.h"
#include "tools/utils.h"

#include "core/core.h"
#include "core/hook.h"

#include "sdk/datamap.h"
#include "sdk/utlmemory.h"
#include "sdk/utlvector.h"
#include "sdk/netchannel.h"
#include "sdk/memory.h"
#include "sdk/platform.h"
#include "sdk/appsystem.h"
#include "sdk/color.h"
#include "sdk/vector.h"
#include "sdk/cvar.h"
#include "sdk/matrix.h"
#include "sdk/recv.h"
#include "sdk/collider.h"
#include "sdk/materials.h"
#include "sdk/model.h"
#include "sdk/modelrender.h"
#include "sdk/entity.h"
#include "sdk/globals.h"
#include "sdk/entityinfo.h"
#include "sdk/eventmanager.h"
#include "sdk/entitylist.h"
#include "sdk/hl.h"
#include "sdk/engine.h"
#include "sdk/usercmd.h"
#include "sdk/renderview.h"
#include "sdk/debugoverlay.h"
#include "sdk/modelinfo.h"
#include "sdk/enginetrace.h"
#include "sdk/physics.h"
#include "sdk/movement.h"
#include "sdk/prediction.h"
#include "sdk/surface.h"
#include "sdk/glow.h"
#include "sdk/beams.h"
#include "sdk/dynamicbb.h"
#include "sdk/weapon.h"
#include "sdk/animations.h"
#include "sdk/player.h"
#include "sdk/tonemap_controller.h"
#include "sdk/bomb.h"
#include "sdk/gamerules.h"
#include "sdk/playerresource.h"
#include "sdk/projectile.h"
#include "sdk/viewsetup.h"
#include "sdk/clientstate.h"
#include "sdk/paintkit.h"
#include "sdk/localize.h"
#include "sdk/input.h"
#include "sdk/c_fogcontroller.h"
#include "sdk/c_hostage.h"
#include "sdk/c_grenade.h"

#include "framework/config.h"
#include "framework/keymanager.h"
#include "framework/menu.h"
#include "framework/csgo.h"
#include "framework/hooks.h"

#include "scripting/scripting.h"

#include "features/discord/discord_rpc.h"
#include "features/features.h"

#define CHEAT_ID 3 // МАМУ ЕБАЛ БЛЯТЬ

g_ext(c_core, core);
g_ext(c_utils, utils);
g_ext(c_config, config);
g_ext(c_menu, menu);
g_ext(c_csgo, csgo);
g_ext(c_scripting, scripting);
g_ext(c_features, features);
g_ext(c_keymanager, keymanager);
g_ext(c_hwid, hwid);
g_ext(c_api, api);

extern std::string g_CURRENT_FUNC;

#pragma comment(lib, "WinInet.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "lua53.lib")


#endif //uc-sflying-cm-cheat-h
