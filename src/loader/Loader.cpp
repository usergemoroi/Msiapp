// Loader.cpp
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>
#include <vector>

#pragma comment(lib, "advapi32.lib")

#define TARGET_PROCESS L"HD-Player.exe"
#define CORE_DLL L"Standoff2Core.dll"

namespace utils {
    DWORD GetProcessIdByName(const std::wstring& processName) {
        PROCESSENTRY32W pe32 = { sizeof(PROCESSENTRY32W) };
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

        DWORD pid = 0;
        if (Process32FirstW(hSnapshot, &pe32)) {
            do {
                if (_wcsicmp(pe32.szExeFile, processName.c_str()) == 0) {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32NextW(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
        return pid;
    }

    bool EnableDebugPrivilege() {
        HANDLE hToken;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
            return false;

        TOKEN_PRIVILEGES tp;
        LUID luid;
        if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
            return false;

        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        bool result = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
        CloseHandle(hToken);
        return result && GetLastError() == ERROR_SUCCESS;
    }

    bool InjectDLL(DWORD pid, const std::wstring& dllPath) {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (!hProcess) return false;

        size_t pathSize = (dllPath.length() + 1) * sizeof(wchar_t);
        LPVOID pRemoteMem = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!pRemoteMem) {
            CloseHandle(hProcess);
            return false;
        }

        if (!WriteProcessMemory(hProcess, pRemoteMem, dllPath.c_str(), pathSize, NULL)) {
            VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
        LPTHREAD_START_ROUTINE pLoadLibraryW = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
        if (!pLoadLibraryW) {
            VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, pLoadLibraryW, pRemoteMem, 0, NULL);
        if (!hRemoteThread) {
            VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        WaitForSingleObject(hRemoteThread, INFINITE);
        DWORD exitCode = 0;
        GetExitCodeThread(hRemoteThread, &exitCode); 

        CloseHandle(hRemoteThread);
        VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);

        return exitCode != 0;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    utils::EnableDebugPrivilege();

    DWORD pid = utils::GetProcessIdByName(TARGET_PROCESS);
    if (!pid) {
        MessageBoxW(NULL, L"Процесс эмулятора не найден. Убедитесь, что BlueStacks 5 запущен.", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    wchar_t dllFullPath[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, dllFullPath);
    wcscat_s(dllFullPath, L"\\");
    wcscat_s(dllFullPath, CORE_DLL);

    if (utils::InjectDLL(pid, dllFullPath)) {
        MessageBoxW(NULL, L"Модуль успешно инициализирован. Используйте назначенные клавиши для управления.", L"Уведомление", MB_ICONINFORMATION);
    } else {
        MessageBoxW(NULL, L"Не удалось инициализировать модуль в целевом процессе.", L"Критическая ошибка", MB_ICONERROR);
        return 2;
    }

    return 0;
}
