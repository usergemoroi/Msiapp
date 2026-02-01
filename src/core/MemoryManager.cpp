#include "MemoryManager.h"
#include <Psapi.h>
#include <tchar.h>
#pragma comment(lib, "psapi.lib")

MemoryManager::MemoryManager() : m_hProcess(NULL), m_unityBase(0) {
    memset(&m_offsets, 0, sizeof(m_offsets));
}

bool MemoryManager::Initialize(DWORD pid) {
    m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!m_hProcess) return false;

    if (!FindUnityBase()) return false;
    if (!PopulateOffsets()) return false;

    return true;
}

bool MemoryManager::FindUnityBase() {
    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (!EnumProcessModules(m_hProcess, hMods, sizeof(hMods), &cbNeeded)) {
        return false;
    }

    for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
        TCHAR szModName[MAX_PATH];
        if (GetModuleFileNameEx(m_hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
            if (_tcsstr(szModName, _T("libunity.so")) || _tcsstr(szModName, _T("UnityPlayer.dll"))) {
                MODULEINFO modInfo;
                if (GetModuleInformation(m_hProcess, hMods[i], &modInfo, sizeof(modInfo))) {
                    m_unityBase = (uintptr_t)modInfo.lpBaseOfDll;
                    return true;
                }
            }
        }
    }
    return false;
}

bool MemoryManager::PopulateOffsets() {
    // Fill with values found in analysis
    m_offsets.offset_PlayerManager = 0x0; // Placeholder for static instance
    m_offsets.offset_PlayerManager_LocalPlayer = Offsets::PlayerManager_LocalPlayer;
    m_offsets.offset_PlayerManager_PlayerList = Offsets::PlayerManager_PlayerList;
    m_offsets.offset_Player_Health = Offsets::PlayerController_Health;
    m_offsets.offset_Player_Team = Offsets::PlayerController_Team;
    m_offsets.offset_Player_Position = Offsets::TranslationData_Position;
    return true;
}

bool MemoryManager::ReadMemory(uintptr_t address, void* buffer, size_t size) {
    SIZE_T bytesRead;
    return ReadProcessMemory(m_hProcess, (LPCVOID)address, buffer, size, &bytesRead) && bytesRead == size;
}

bool MemoryManager::WriteMemory(uintptr_t address, const void* buffer, size_t size) {
    DWORD oldProtect;
    if (!VirtualProtectEx(m_hProcess, (LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return false;
    }

    SIZE_T bytesWritten;
    bool success = WriteProcessMemory(m_hProcess, (LPVOID)address, buffer, size, &bytesWritten) && bytesWritten == size;

    VirtualProtectEx(m_hProcess, (LPVOID)address, size, oldProtect, &oldProtect);
    return success;
}

uintptr_t MemoryManager::GetDynamicAddress(uintptr_t base, const std::vector<uintptr_t>& offsets) {
    uintptr_t address = base;
    for (size_t i = 0; i < offsets.size(); i++) {
        ReadMemory(address, &address, sizeof(address));
        address += offsets[i];
    }
    return address;
}

bool MemoryManager::GetLocalPlayerPosition(Vector3& outPos) {
    // Simplified logic for example
    return false;
}
