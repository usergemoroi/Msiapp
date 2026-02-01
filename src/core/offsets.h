#pragma once
#include <cstdint>

namespace Offsets {
    // Standoff 2 v0.37.0 (extracted from dump.cs)
    
    // PlayerManager (LazySingleton)
    constexpr uintptr_t PlayerManager_StaticInstance = 0x0; // Needs further analysis for exact static pointer
    constexpr uintptr_t PlayerManager_LocalPlayer = 0x68;
    constexpr uintptr_t PlayerManager_PlayerList = 0x38;

    // PlayerController
    constexpr uintptr_t PlayerController_Health = 0x150;
    constexpr uintptr_t PlayerController_Team = 0x154;
    constexpr uintptr_t PlayerController_MovementController = 0x98;

    // MovementController
    constexpr uintptr_t MovementController_TranslationData = 0xB0;

    // GDEBCCDEEHFEGFD (TranslationData)
    constexpr uintptr_t TranslationData_Position = 0x1C;
}
