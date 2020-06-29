//
// Created by ruppet on 2/4/2020.
//

#include "../loader.h"
#include "c_utils.h"


void c_utils::log(const char *text, ...) {
#ifndef _DEBUG
    return;
#endif

    char buffer[2048] = {0};

    va_list va;
    va_start(va, text);
    vsprintf(buffer, text, va);
    va_end(va);

    printf(str("%s\n"), buffer);
}

void c_utils::create_console() {
#ifndef _DEBUG
    return;
#endif

    api(AllocConsole)();

    freopen(str("CONOUT$"), str("w"), stdout);
    freopen(str("CONOUT$"), str("w"), stderr);

    api(SetConsoleTitleA)(str("uc-sflying"));
}

const char *c_utils::format(const char* text, ...) {
    char buffer[2048] = {0};

    va_list va;
    va_start(va, text);
    vsprintf(buffer, text, va);
    va_end(va);

    return buffer;
}

void c_utils::generate_random(char *buffer, size_t size) {
    const char* alphabet = str("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMONPQRSTUVWXYZ0123456789");
    for (int i = 0; i < size; i++)
        buffer[i] = alphabet[rand() % strlen(alphabet) - 1];
}

std::vector<std::string> c_utils::split(const std::string& str, const char* delim)
{
    std::vector<std::string> dest;

    char* pTempStr = _strdup(str.c_str());
    char* pWord = strtok(pTempStr, delim);
    while (pWord != NULL)
    {
        dest.push_back(pWord);
        pWord = strtok(NULL, delim);
    }

    free(pTempStr);
    return dest;
}

unsigned long c_utils::get_process_id(std::string name) {
    LPDWORD lpdwProcessIds;
    LPTSTR  lpszBaseName;
    HANDLE  hProcess;
    DWORD   i = 0, cdwProcesses = 0, dwProcessId = 0;

    lpdwProcessIds = (LPDWORD)HeapAlloc(GetProcessHeap(), 0, 1024 * sizeof(DWORD));
    if (lpdwProcessIds != NULL)
    {
        if (api(K32EnumProcesses)(lpdwProcessIds, 1024 * sizeof(DWORD), &cdwProcesses))
        {
            lpszBaseName = (LPTSTR)HeapAlloc(GetProcessHeap(), 0, MAX_PATH * sizeof(TCHAR));
            if (lpszBaseName != NULL)
            {
                cdwProcesses /= sizeof(DWORD);
                for (i = 0; i < cdwProcesses; i++)
                {
                    hProcess = api(OpenProcess)(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lpdwProcessIds[i]);
                    if (hProcess != NULL)
                    {
                        if (api(K32GetModuleBaseNameA)(hProcess, 0, lpszBaseName, MAX_PATH) > 0)
                        {
                            if (!lstrcmpi(lpszBaseName, name.c_str()))
                            {
                                dwProcessId = lpdwProcessIds[i];
                                api(CloseHandle)(hProcess);
                                break;
                            }
                        }

                        api(CloseHandle)(hProcess);
                    }
                }

                api(HeapFree)(api(GetProcessHeap)(), 0, (LPVOID)lpszBaseName);
            }
        }

        api(HeapFree)(api(GetProcessHeap)(), 0, (LPVOID)lpdwProcessIds);
    }

    return dwProcessId;
}