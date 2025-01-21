/*
 * Copyright (c) Contributors to the Open 3D Engine Project
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <Multiplayer/NetworkEntity/IFilterEntityManager.h>

namespace GameLiftDemo
{
    //! @class ExampleFilteredEntityComponent
    //! @brief An example of using IFilterEntityManager to filter entities to clients.
    class ExampleFilteredEntityComponent final
        : public AZ::Component
        , public Multiplayer::IFilterEntityManager
    {
    public:
        AZ_COMPONENT(GameLiftDemo::ExampleFilteredEntityComponent, "{B67DBEE2-86CB-4E31-BAD6-B8BF14DA9DDD}");

        static void Reflect(AZ::ReflectContext* context);

        //! AZ::Component overrides.
        //! @{
        void Activate() override;
        void Deactivate() override;
        //! }@

        //! IFilterEntityManager overrides.
        //! @{
        bool IsEntityFiltered(AZ::Entity* entity, Multiplayer::ConstNetworkEntityHandle controllerEntity, AzNetworking::ConnectionId connectionId) override;
        //! }@

    private:
        bool m_enabled = true;

        AZStd::string m_filterNamesForEvenConnectionIds{ "Filter Even" };
        AZStd::string m_filterNamesForOddConnectionIds{ "Filter Odd" };
    };
}
