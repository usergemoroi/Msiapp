#include "MemoryManager.h"
#include <psapi.h>
#include <tchar.h>
#include <algorithm>
#include <cstring>
#include <string>

#pragma comment(lib, "psapi.lib")

MemoryManager::MemoryManager() : m_hProcess(nullptr), m_unityBase(0) {
    std::memset(&m_offsets, 0, sizeof(m_offsets));
}

MemoryManager::~MemoryManager() {
    if (m_hProcess) {
        CloseHandle(m_hProcess);
        m_hProcess = nullptr;
    }
}

bool MemoryManager::Initialize(DWORD pid) {
    m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!m_hProcess) {
        return false;
    }

    if (!FindUnityBase()) {
        CloseHandle(m_hProcess);
        m_hProcess = nullptr;
        return false;
    }

    if (!PopulateOffsets()) {
        CloseHandle(m_hProcess);
        m_hProcess = nullptr;
        return false;
    }

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
#ifdef UNICODE
            std::wstring moduleName(szModName);
#else
            // Convert char* to wchar_t* for MinGW builds
            int len = strlen(szModName);
            std::wstring moduleName(len, L' ');
            for (int j = 0; j < len; j++) {
                moduleName[j] = static_cast<wchar_t>(static_cast<unsigned char>(szModName[j]));
            }
#endif
            std::transform(moduleName.begin(), moduleName.end(), moduleName.begin(), ::tolower);
            
            if (moduleName.find(L"libunity.so") != std::wstring::npos || 
                moduleName.find(L"unityplayer.dll") != std::wstring::npos ||
                moduleName.find(L"unityplayer") != std::wstring::npos) {
                MODULEINFO modInfo;
                if (GetModuleInformation(m_hProcess, hMods[i], &modInfo, sizeof(modInfo))) {
                    m_unityBase = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
                    return true;
                }
            }
        }
    }
    return false;
}

bool MemoryManager::PopulateOffsets() {
    m_offsets.baseUnity = m_unityBase;
    
    m_offsets.offset_PlayerManager = 0x0; // Это нужно найти через статический анализ
    m_offsets.offset_PlayerManager_LocalPlayer = Offsets::PlayerManager_LocalPlayer;
    m_offsets.offset_PlayerManager_PlayerList = Offsets::PlayerManager_PlayerList;
    m_offsets.offset_Player_Health = Offsets::PlayerController_Health;
    m_offsets.offset_Player_Team = Offsets::PlayerController_Team;
    m_offsets.offset_Player_Position = Offsets::TranslationData_Position;
    
    return true;
}

bool MemoryManager::ReadMemory(uintptr_t address, void* buffer, size_t size) {
    if (!m_hProcess || !address || !buffer || size == 0) {
        return false;
    }
    
    SIZE_T bytesRead = 0;
    BOOL result = ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead);
    return result && bytesRead == size;
}

bool MemoryManager::WriteMemory(uintptr_t address, const void* buffer, size_t size) {
    if (!m_hProcess || !address || !buffer || size == 0) {
        return false;
    }
    
    DWORD oldProtect = 0;
    if (!VirtualProtectEx(m_hProcess, reinterpret_cast<LPVOID>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return false;
    }

    SIZE_T bytesWritten = 0;
    BOOL result = WriteProcessMemory(m_hProcess, reinterpret_cast<LPVOID>(address), buffer, size, &bytesWritten);
    
    VirtualProtectEx(m_hProcess, reinterpret_cast<LPVOID>(address), size, oldProtect, &oldProtect);
    
    return result && bytesWritten == size;
}

uintptr_t MemoryManager::GetDynamicAddress(uintptr_t base, const std::vector<uintptr_t>& offsets) {
    if (!base || offsets.empty()) {
        return 0;
    }

    uintptr_t address = base;
    for (size_t i = 0; i < offsets.size(); i++) {
        if (!ReadMemory(address, &address, sizeof(address))) {
            return 0;
        }
        if (address == 0) {
            return 0;
        }
        address += offsets[i];
    }
    return address;
}

uintptr_t MemoryManager::GetUnityBaseAddress() {
    return m_unityBase;
}

bool MemoryManager::GetLocalPlayerPosition(Vector3& outPos) {
    if (!m_offsets.offset_PlayerManager) {
        return false;
    }

    // Пример логики - нужно адаптировать под реальную структуру игры
    std::vector<uintptr_t> offsets = {
        m_offsets.offset_PlayerManager,
        m_offsets.offset_PlayerManager_LocalPlayer,
        m_offsets.offset_Player_Position
    };

    uintptr_t positionAddress = GetDynamicAddress(m_unityBase, offsets);
    if (!positionAddress) {
        return false;
    }

    return ReadMemory(positionAddress, &outPos, sizeof(Vector3));
}

bool MemoryManager::GetViewMatrix(Matrix4x4& outMatrix) {
    // Реализация зависит от конкретной версии игры
    // Обычно матрица вида находится в глобальной переменной Unity
    return false;
}

bool MemoryManager::GetPlayerList(std::vector<uintptr_t>& outList) {
    if (!m_offsets.offset_PlayerManager) {
        return false;
    }

    std::vector<uintptr_t> offsets = {
        m_offsets.offset_PlayerManager,
        m_offsets.offset_PlayerManager_PlayerList
    };

    uintptr_t listAddress = GetDynamicAddress(m_unityBase, offsets);
    if (!listAddress) {
        return false;
    }

    // Пример чтения списка - нужно адаптировать под реальную структуру
    uintptr_t listStart = 0;
    if (!ReadMemory(listAddress, &listStart, sizeof(listStart))) {
        return false;
    }

    int listSize = 0;
    if (!ReadMemory(listAddress + sizeof(uintptr_t), &listSize, sizeof(listSize))) {
        return false;
    }

    outList.clear();
    for (int i = 0; i < listSize && i < 50; i++) { // Ограничение 50 игроков
        uintptr_t playerPtr = 0;
        uintptr_t elementAddress = listStart + (i * sizeof(uintptr_t));
        
        if (ReadMemory(elementAddress, &playerPtr, sizeof(playerPtr)) && playerPtr != 0) {
            outList.push_back(playerPtr);
        }
    }

    return !outList.empty();
}

bool MemoryManager::GetPlayerHealth(uintptr_t playerPtr, int& outHealth) {
    if (!playerPtr) {
        return false;
    }

    uintptr_t healthAddress = playerPtr + m_offsets.offset_Player_Health;
    return ReadMemory(healthAddress, &outHealth, sizeof(outHealth));
}

bool MemoryManager::GetPlayerTeam(uintptr_t playerPtr, int& outTeam) {
    if (!playerPtr) {
        return false;
    }

    uintptr_t teamAddress = playerPtr + m_offsets.offset_Player_Team;
    return ReadMemory(teamAddress, &outTeam, sizeof(outTeam));
}

bool MemoryManager::GetPlayerPosition(uintptr_t playerPtr, Vector3& outPos) {
    if (!playerPtr) {
        return false;
    }

    uintptr_t translationDataAddress = playerPtr + m_offsets.offset_Player_Position;
    uintptr_t translationData = 0;
    
    if (!ReadMemory(translationDataAddress, &translationData, sizeof(translationData))) {
        return false;
    }

    if (!translationData) {
        return false;
    }

    uintptr_t positionAddress = translationData + Offsets::TranslationData_Position;
    return ReadMemory(positionAddress, &outPos, sizeof(outPos));
}

bool MemoryManager::IsPlayerVisible(uintptr_t playerPtr) {
    // Упрощенная проверка видимости
    // В реальности нужно проверять Line of Sight, walls, etc.
    if (!playerPtr) {
        return false;
    }

    int health = 0;
    if (!GetPlayerHealth(playerPtr, health)) {
        return false;
    }

    return health > 0;
}

bool MemoryManager::InstallUnityHooks() {
    // Реализация хуков Unity (если нужна)
    // Например, для перехвата Present() для рендеринга ESP
    return false;
}