#pragma once
#include <cstdint>

namespace Offsets {
    // ============================================
    // Standoff 2 v0.37.0 - Offset Configuration
    // ============================================
    // WARNING: These offsets may need updating for different game versions
    // Use Cheat Engine or IDA Pro to find correct offsets for your version
    // ============================================
    
    // PlayerManager (LazySingleton)
    // Find via: static singleton pointer in PlayerManager class
    constexpr uintptr_t PlayerManager_StaticInstance = 0x0; // TODO: FIND THIS! Use Cheat Engine to search for singleton pointer
    
    // PlayerManager offsets (relative to PlayerManager instance)
    constexpr uintptr_t PlayerManager_LocalPlayer = 0x68;     // Local player pointer
    constexpr uintptr_t PlayerManager_PlayerList = 0x38;      // List<Player*> or similar
    
    // ============================================
    // PlayerController Offsets (relative to player instance)
    // ============================================
    // Health is typically an int32 or float
    constexpr uintptr_t PlayerController_Health = 0x150;        
    
    // Team ID (0 = Terrorist, 1 = Counter-Terrorist, etc)
    constexpr uintptr_t PlayerController_Team = 0x154;        
    
    // Pointer to MovementController
    constexpr uintptr_t PlayerController_MovementController = 0x98;
    
    // Player name (if needed, typically a System.String)
    constexpr uintptr_t PlayerController_Name = 0x40;
    
    // IsAlive flag (optional, byte or bool)
    constexpr uintptr_t PlayerController_IsAlive = 0x158;
    
    // ============================================
    // MovementController Offsets
    // ============================================
    // Pointer to TranslationData
    constexpr uintptr_t MovementController_TranslationData = 0xB0;
    
    // ============================================
    // TranslationData (GDEBCCDEEHFEGFD) Offsets
    // ============================================
    // Position Vector3 (x,y,z floats)
    constexpr uintptr_t TranslationData_Position = 0x1C;      // Vector3 m_Position
    
    // Rotation (optional)
    constexpr uintptr_t TranslationData_Rotation = 0x28;      // Quaternion m_Rotation
    
    // Velocity (optional)
    constexpr uintptr_t TranslationData_Velocity = 0x34;      // Vector3 m_Velocity
    
    // ============================================
    // Unity Engine Offsets (for ESP/rendering)
    // ============================================
    // ViewMatrix in Unity's Camera class
    constexpr uintptr_t Camera_ViewMatrix = 0x0;              // TODO: Find via Camera.main
    
    // World to screen matrix
    constexpr uintptr_t Camera_WorldToScreen = 0x0;           // Matrix4x4
    
    // MainCamera pointer
    constexpr uintptr_t Camera_MainCamera = 0x0;              // static Camera* main
    
    // ============================================
    // Additional Offsets for Features
    // ============================================
    // WeaponController (if implementing weapon features)
    constexpr uintptr_t PlayerController_WeaponController = 0x160;
    
    // Ammo in clip
    constexpr uintptr_t WeaponController_AmmoInClip = 0x10;
    
    // Ammo in reserve
    constexpr uintptr_t WeaponController_AmmoReserve = 0x14;
    
    // ============================================
    // Game State Offsets
    // ============================================
    // GameManager singleton (for match state)
    constexpr uintptr_t GameManager_StaticInstance = 0x0;     // TODO: Find GameManager pointer
    
    // IsMatchStarted
    constexpr uintptr_t GameManager_IsMatchStarted = 0x20;
    
    // Round number
    constexpr uintptr_t GameManager_RoundNumber = 0x24;
    
    // ============================================
    // REMEMBER TO UPDATE THESE FOR YOUR VERSION!
    // ============================================
    // To find offsets:
    // 1. Use Cheat Engine to attach to HD-Player.exe
    // 2. Find PlayerManager static pointer (search for singleton pattern)
    // 3. Find local player and inspect memory layout
    // 4. Find other players in list and compare offsets
    // 5. Use Unity's offsetof patterns to verify
    // ============================================
}