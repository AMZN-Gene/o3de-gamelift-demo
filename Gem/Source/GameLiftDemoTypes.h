/*
 * Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Asset/AssetSerializer.h>
#include <AzFramework/Spawnable/Spawnable.h>
#include <AzNetworking/DataStructures/FixedSizeBitset.h>
#include <AzNetworking/Utilities/QuantizedValues.h>
#include <Multiplayer/MultiplayerTypes.h>

namespace GameLiftDemo
{
    constexpr AZStd::string_view WinningCoinCountSetting = "/GameLiftDemo/Settings/WinningCoinCount";
    constexpr AZStd::string_view KnockbackDistanceByEnergyBallSetting = "/GameLiftDemo/Settings/EnergyBall/KnockbackDistanceMeters";
    constexpr AZStd::string_view EnergyBallSpeedSetting = "/GameLiftDemo/Settings/EnergyBall/Speed";
    constexpr AZStd::string_view EnergyBallArmorDamageSetting = "/GameLiftDemo/Settings/EnergyBall/ArmorDamage";
    constexpr AZStd::string_view EnergyCannonFiringPeriodSetting = "/GameLiftDemo/Settings/EnergyCannon/FiringPeriodMilliseconds";

    using StickAxis = AzNetworking::QuantizedValues<1, 1, -1, 1>;
    using MouseAxis = AzNetworking::QuantizedValues<1, 2, -1, 1>;

    //! Various character animation states.
    enum class CharacterAnimState
    {
        Idle,
        Sprinting,
        Crouching,
        Jumping,
        Falling,
        Landing,
        Climbing,
        Aiming,
        Shooting,
        Hit,
        Dying,
        MAX
    };
    using CharacterAnimStateBitset = AzNetworking::FixedSizeBitset<static_cast<AZStd::size_t>(CharacterAnimState::MAX)>;

    enum class Action
    {
        Default,
        Strafing,
        Sprinting,
        Jumping,
        Crouching,
        COUNT = Crouching + 1
    };

    AZ_ENUM_CLASS(SoundEffect,
        Unused,

        // Player Sounds
        PlayerFootSteps,
        PlayerExertion,
        PlayerKnockedDown,
        ArmorBreaking,
        ArmorMend,
        PlayerOuch,
        LadderClimb,
        ShutDown,

        // Game Event Sounds
        CountDown,
        GemPickup,
        VictoryFanfare,
        LosingFanfare,
        RoundStart,
        RoundEnd,
        GameEnd,

        // Laser Pistol
        LaserPistolMuzzleFlash,
        LaserPistolImpact,

        // Bubble Gun
        BubbleGunBuildup,
        BubbleGunMuzzleFlash,
        BubbleGunProjectile,
        BubbleGunImpact,

        // Environment
        JumpPadLaunch,
        TeleporterUse,

        // Energy Ball Trap
        EnergyBallTrapRisingOutOfTheGround,
        EnergyBallTrapBuildup, // followed by muzzle flash
        EnergyBallTrapProjectile,
        EnergyBallTrapImpact,
        EnergyBallTrapOnCooldown // plays when you try to fire it during cooldown
    );

    using RoundTimeSec = AzNetworking::QuantizedValues<1, 2, 0, 3600>; // 1 hour max round duration

    static constexpr int MaxSupportedPlayers = 10;


    using PlayerNameString = AZStd::fixed_string<50>;

    struct PlayerState
    {
        PlayerNameString m_playerName;
        uint32_t m_score = 0;          // coins collected
        uint8_t m_remainingArmor = 0;
        bool operator!=(const PlayerState& rhs) const;
        bool Serialize(AzNetworking::ISerializer& serializer);
    };

    inline bool PlayerState::Serialize(AzNetworking::ISerializer& serializer)
    {
        return serializer.Serialize(m_playerName, "Name")
            && serializer.Serialize(m_score, "Score")
            && serializer.Serialize(m_remainingArmor, "Armor");
    }

    inline bool PlayerState::operator!=(const PlayerState& rhs) const
    {
        return m_playerName != rhs.m_playerName
            || m_score != rhs.m_score
            || m_remainingArmor != rhs.m_remainingArmor;
    }


    struct PlayerResetOptions
    {
        bool m_resetArmor = false;
        uint16_t m_coinPenalty = 0;
        bool Serialize(AzNetworking::ISerializer& serializer);
    };

    inline bool PlayerResetOptions::Serialize(AzNetworking::ISerializer& serializer)
    {
        return serializer.Serialize(m_resetArmor, "resetArmor")
            && serializer.Serialize(m_coinPenalty, "coinPenalty");
    }

}

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(GameLiftDemo::SoundEffect, "{50188757-F078-417E-B8CF-B6964AC08104}");
    AZ_TYPE_INFO_SPECIALIZE(GameLiftDemo::CharacterAnimState, "{91ECCC34-4FEC-4F7B-8F4E-6DF58AD9A6D7}");
    AZ_TYPE_INFO_SPECIALIZE(GameLiftDemo::Action, "{7670C3D9-0EE1-40A9-8B99-C9BCEF6AE151}");
}
