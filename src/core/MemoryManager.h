#pragma once
#include <windows.h>
#include <vector>
#include <cstdint>
#include "offsets.h"

struct GameOffsets {
    uintptr_t baseUnity; 

    uintptr_t offset_PlayerManager;
    uintptr_t offset_PlayerManager_LocalPlayer;
    uintptr_t offset_PlayerManager_PlayerList;
    uintptr_t offset_Player_Health;
    uintptr_t offset_Player_Team;
    uintptr_t offset_Player_Position;
};

class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager();
    bool Initialize(DWORD pid);
    bool ReadMemory(uintptr_t address, void* buffer, size_t size);
    bool WriteMemory(uintptr_t address, const void* buffer, size_t size);
    uintptr_t GetUnityBaseAddress();
    uintptr_t GetDynamicAddress(uintptr_t base, const std::vector<uintptr_t>& offsets);
    bool InstallUnityHooks();

    struct Vector3 { float x, y, z; };
    struct Vector2 { float x, y; };
    struct Matrix4x4 { float m[16]; };

    bool GetLocalPlayerPosition(Vector3& outPos);
    bool GetViewMatrix(Matrix4x4& outMatrix);
    bool GetPlayerList(std::vector<uintptr_t>& outList);
    bool GetPlayerHealth(uintptr_t playerPtr, int& outHealth);
    bool GetPlayerTeam(uintptr_t playerPtr, int& outTeam);
    bool GetPlayerPosition(uintptr_t playerPtr, Vector3& outPos);
    bool IsPlayerVisible(uintptr_t playerPtr);

private:
    HANDLE m_hProcess;
    GameOffsets m_offsets;
    uintptr_t m_unityBase;

    bool FindUnityBase();
    bool PopulateOffsets();
};
