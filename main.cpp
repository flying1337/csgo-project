#include "loader.h"

g_new(c_utils, utils);
g_new(c_core, core);
g_new(c_hwid, hwid);
g_new(c_api, api);
g_new(c_dx, dx);

using namespace std::chrono_literals;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    utils->create_console();

    utils->log(str("Loader: starting up"));
    core->setup();

    std::thread([&]() {
        if (!core->verify())
            core->crash(core->last_error.c_str());
    }).detach();

    static auto peekmessagea = api(PeekMessageA);
    static auto translatemessage = api(TranslateMessage);
    static auto dispatchmessagea = api(DispatchMessageA);
    static auto gettickcount64 = api(GetTickCount64);
    static auto sleep = api(Sleep);
    static auto k32enumprocesses = api(K32EnumProcesses);
    static auto openprocess = api(OpenProcess);
    static auto k32getmodulebasenamea = api(K32GetModuleBaseNameA);
    static auto closehandle = api(CloseHandle);
    static auto heapfree = api(HeapFree);
    static auto getprocessheap = api(GetProcessHeap);

    static auto last_creation = gettickcount64();
    while (true) {
#ifndef _DEBUG
        if (gettickcount64() - last_creation > 250) {
            last_creation = gettickcount64();

            std::thread([&]() {
                auto mhm = false;

                LPDWORD lpdwProcessIds;
                LPTSTR  lpszBaseName;
                HANDLE  hProcess;
                DWORD   i = 0, cdwProcesses = 0, dwProcessId = 0;

                lpdwProcessIds = (LPDWORD)HeapAlloc(GetProcessHeap(), 0, 1024 * sizeof(DWORD));
                if (lpdwProcessIds != NULL)
                {
                    if (k32enumprocesses(lpdwProcessIds, 1024 * sizeof(DWORD), &cdwProcesses))
                    {
                        lpszBaseName = (LPTSTR)HeapAlloc(GetProcessHeap(), 0, MAX_PATH * sizeof(TCHAR));
                        if (lpszBaseName != NULL)
                        {
                            cdwProcesses /= sizeof(DWORD);
                            for (i = 0; i < cdwProcesses; i++)
                            {
                                hProcess = openprocess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lpdwProcessIds[i]);
                                if (hProcess != NULL)
                                {
                                    if (k32getmodulebasenamea(hProcess, 0, lpszBaseName, MAX_PATH) > 0)
                                    {
                                        if (!lstrcmpi(lpszBaseName, str("processhacker.exe")) ||
                                                !lstrcmpi(lpszBaseName, str("ida.exe")) ||
                                                !lstrcmpi(lpszBaseName, str("ida64.exe")) ||
                                                !lstrcmpi(lpszBaseName, str("ollydbg.exe")) ||
                                                !lstrcmpi(lpszBaseName, str("x32dbg.exe")) ||
                                                !lstrcmpi(lpszBaseName, str("x64dbg.exe")) ||
                                                !lstrcmpi(lpszBaseName, str("wireshark.exe")))
                                        {
                                            mhm = true;
                                            closehandle(hProcess);
                                            break;
                                        }
                                    }

                                    closehandle(hProcess);
                                }
                            }

                            heapfree(getprocessheap(), 0, (LPVOID)lpszBaseName);
                        }
                    }

                    heapfree(getprocessheap(), 0, (LPVOID)lpdwProcessIds);
                }

                if (mhm)
                {
                    if (core->is_setup)
                        api->send_encoded_string(str("Asshole"));

                    memset(0, 0, 1);
                }
            }).detach();

            std::thread([&]() {
                auto lastcheck = gettickcount64();
                for (auto i = 0; i < 2; i++) {
                    sleep(100);
                    if (gettickcount64() - lastcheck > 250) {
                        if (core->is_setup)
                            api->send_encoded_string(str("Asshole"));

                        memset(0, 0, 1);
                        break;
                    }

                    lastcheck = gettickcount64();
                }
            }).detach();
        }
#endif

        MSG msg;
        if (peekmessagea(&msg, 0, 0, 0, PM_REMOVE)) {
            translatemessage(&msg);
            dispatchmessagea(&msg);
        }

        if (!dx->render())
            break;
    }
}