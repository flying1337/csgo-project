#include "cheat.h"

g_new(c_core, core);
g_new(c_utils, utils);
g_new(c_config, config);
g_new(c_menu, menu);
g_new(c_csgo, csgo);
g_new(c_scripting, scripting);
g_new(c_features, features);
g_new(c_keymanager, keymanager);

std::string g_CURRENT_FUNC = "none";

using namespace std::chrono_literals;
void mainthread() {
    g_CURRENT_FUNC = __FUNCTION__;
    g_crash_start();

#ifndef _DEBUG
    auto chk_addr = GetPrivateProfileIntA(str("l"), str("d"), 0, str("c:/uc-sflying/ldr/_")) ^ 238;

	char chk[48] = { 0 };
	ReadProcessMemory(OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId()), (LPCVOID)chk_addr, chk, 48, nullptr);

	if (!chk[0])
		memset(nullptr, 0, 1);

	auto hw = hwid->get_hwid();
	char correct[128] = { 0 };
	memcpy(correct, str("\x4f\x9f\x7e\xb0\x07\x23\x55\x18\x7b\x59\x82\x61\xfd\x89\x47"), 15);
	memcpy((void*)((unsigned long long)correct + 15), hw.c_str(), 32);

	auto unmatch = false;
	for (auto i = 0; i < 48; i++)
	    if (correct[i] != chk[i]) unmatch = true;

	if (unmatch)
	    memset(nullptr, 0, 1);

	WritePrivateProfileStringA(str("l"), str("d"), "", str("c:/uc-sflying/ldr/_"));

	api->set_verify_string(chk);
#endif

    utils->wait_for_module(str("kernel32.dll"));
    utils->wait_for_module(str("engine.dll"));
    utils->wait_for_module(str("client_panorama.dll"));
    utils->wait_for_module(str("serverbrowser.dll"));
    utils->wait_for_module(str("gameoverlayrenderer.dll"));

    config->init();
    core->initialize();

    while (!core->is_unload_triggered) {
        features->discord->run_callbacks();

        std::this_thread::sleep_for(1s);
    }

    g_crash_end();
}

#ifdef a
int CALLBACK crash_callback(CR_CRASH_CALLBACK_INFOA* info) {
    MessageBoxA(0,
            str("Uh oh! It seems I have crashed.\n\nCrash report has been created in C:/uc-sflying/, please send to developer..."),
            str("uc-sflying | Crashed!"), MB_ICONERROR | MB_APPLMODAL);

    crAddPropertyA(str("F"), g_CURRENT_FUNC.c_str());

    return CR_CB_DODEFAULT;
}
#endif

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID rv) {
//    AllocConsole();
//
//    freopen(str("CONOUT$"), str("w"), stdout);
//    freopen(str("CONOUT$"), str("w"), stderr);
//
//    SetConsoleTitleA(str("uc-sflying"));

    if (reason == DLL_PROCESS_ATTACH) {
#ifdef a
        CR_INSTALL_INFOA cr;
        memset(&cr, 0, sizeof(cr));

        cr.cb = sizeof(cr);
        cr.pszAppName = str("uc-sflying");
        cr.pszAppVersion = str("beta");
        cr.pszErrorReportSaveDir = str("c:/uc-sflying");
        cr.pszCrashSenderPath = str("c:/uc-sflying");
        cr.pszDebugHelpDLL = str("c:/uc-sflying");
        cr.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS | CR_INST_DONT_SEND_REPORT;

        if (crInstallA(&cr) != 0) {
            char buffer[512] = {0};
            crGetLastErrorMsgA(buffer, 512);

            MessageBoxA(0, (std::string(str("Initialization failed - ")) + buffer).c_str(), str("Fatal error"),
                        MB_ICONERROR | MB_APPLMODAL);
            return FALSE;
        }

        crSetCrashCallbackA(crash_callback, 0);

#endif
        std::thread(mainthread).detach();
    }

    return TRUE;
}

size_t csgo_memory_get_size(void* m) {
    return csgo->memory->get_size(m);
}