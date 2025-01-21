
#pragma once

#include <GameLiftDemo/GameLiftDemoTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace GameLiftDemo
{
    class GameLiftDemoRequests
    {
    public:
        AZ_RTTI(GameLiftDemoRequests, GameLiftDemoRequestsTypeId);
        virtual ~GameLiftDemoRequests() = default;
        // Put your public methods here
    };

    class GameLiftDemoBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GameLiftDemoRequestBus = AZ::EBus<GameLiftDemoRequests, GameLiftDemoBusTraits>;
    using GameLiftDemoInterface = AZ::Interface<GameLiftDemoRequests>;

} // namespace GameLiftDemo
