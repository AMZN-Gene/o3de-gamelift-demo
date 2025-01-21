
#include <AzCore/Serialization/SerializeContext.h>

#include "GameLiftDemoSystemComponent.h"

#include <GameLiftDemo/GameLiftDemoTypeIds.h>

#include <Source/AutoGen/AutoComponentTypes.h>
#include <Source/Weapons/WeaponTypes.h>

// Make sure code only builds in dedicated servers
#if AZ_TRAIT_SERVER && !AZ_TRAIT_CLIENT
    #include "Request/AWSGameLiftServerRequestBus.h"
#endif


namespace GameLiftDemo
{
    AZ_COMPONENT_IMPL(GameLiftDemoSystemComponent, "GameLiftDemoSystemComponent",
        GameLiftDemoSystemComponentTypeId);

    void GameLiftDemoSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        ReflectWeaponEnums(context);
        GatherParams::Reflect(context);
        HitEffect::Reflect(context);
        HitEntity::Reflect(context);
        HitEvent::Reflect(context);
        WeaponParams::Reflect(context);
        GameEffect::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GameLiftDemoSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void GameLiftDemoSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GameLiftDemoService"));
    }

    void GameLiftDemoSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GameLiftDemoService"));
    }

    void GameLiftDemoSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("NetworkingService"));

        // Ensure Multiplayer gem is a requirement so that MultiplayerSystemComponent calls RegisterMultiplayerComponents before this component activates.
        // It's important for multiplayer components to be registered in a consistent order so that the server and client
        //   assign the same component-id for each component.
        required.push_back(AZ_CRC_CE("MultiplayerService"));

        #if AZ_TRAIT_SERVER && !AZ_TRAIT_CLIENT
            required.push_back(AZ_CRC_CE("AWSGameLiftServerService"));
        #endif
    }

    void GameLiftDemoSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    GameLiftDemoSystemComponent::GameLiftDemoSystemComponent()
    {
        if (GameLiftDemoInterface::Get() == nullptr)
        {
            GameLiftDemoInterface::Register(this);
        }
    }

    GameLiftDemoSystemComponent::~GameLiftDemoSystemComponent()
    {
        if (GameLiftDemoInterface::Get() == this)
        {
            GameLiftDemoInterface::Unregister(this);
        }
    }

    void GameLiftDemoSystemComponent::Init()
    {
    }

    void GameLiftDemoSystemComponent::Activate()
    {
        GameLiftDemoRequestBus::Handler::BusConnect();
        RegisterMultiplayerComponents();

        #if AZ_TRAIT_SERVER && !AZ_TRAIT_CLIENT
            const auto console = AZ::Interface<AZ::IConsole>::Get();
            bool gameLiftEnabled = false;
            console->GetCvarValue("sv_gameLiftEnabled", gameLiftEnabled);
                // sv_gameLiftEnabled
            if (gameLiftEnabled)
            {
                AWSGameLift::AWSGameLiftServerRequestBus::Broadcast(
                    &AWSGameLift::IAWSGameLiftServerRequests::NotifyGameLiftProcessReady);
            }
        #endif
    }

    void GameLiftDemoSystemComponent::Deactivate()
    {
        GameLiftDemoRequestBus::Handler::BusDisconnect();
    }
}
