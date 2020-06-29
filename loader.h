//
// Created by ruppet on 2/4/2020.
//

#ifndef uc-sflying-loader-loader-h
#define uc-sflying-loader-loader-h


#define g_new(t, n) t* n = new t
#define g_ext(t, n) extern t* n

#define IMGUI_DEFINE_MATH_OPERATORS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <WinInet.h>
#include <filesystem>
#include <comdef.h>
#include <WbemIdl.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <thread>
#include <vector>
#include <BlackBone/Process/Process.h>
#include <BlackBone/ManualMap/MMap.h>
#include <Psapi.h>

#include "tools/li.h"
#include "tools/strenc.h"
#include "tools/c_utils.h"

#include "core/subscription.h"

#include "ui/imgui.h"
#include "ui/imgui_internal.h"
#include "ui/imgui_impl_win32.h"
#include "ui/imgui_impl_dx9.h"
#include "ui/ui/c_ui.h"

#include "core/tcp.h"
#include "core/md5.h"
#include "core/base64.h"
#include "core/hwid.h"
#include "core/api.h"
#include "core/c_core.h"

#include "framework/c_dx.h"

g_ext(c_utils, utils);
g_ext(c_core, core);
g_ext(c_hwid, hwid);
g_ext(c_dx, dx);

#pragma comment(lib, "WinInet.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "BlackBone.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#endif //uc-sflying-loader-loader-h
