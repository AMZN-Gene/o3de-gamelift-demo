
#pragma once

#include <AzCore/Component/Component.h>

#include <GameLiftDemo/GameLiftDemoBus.h>

namespace GameLiftDemo
{
    class GameLiftDemoSystemComponent
        : public AZ::Component
        , protected GameLiftDemoRequestBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(GameLiftDemoSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        GameLiftDemoSystemComponent();
        ~GameLiftDemoSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // GameLiftDemoRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
