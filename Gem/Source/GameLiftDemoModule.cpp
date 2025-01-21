
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "GameLiftDemoSystemComponent.h"

#include <GameLiftDemo/GameLiftDemoTypeIds.h>
#include <Components/AttachPlayerWeaponComponent.h>
#include <Source/AutoGen/AutoComponentTypes.h>


namespace GameLiftDemo
{
    class GameLiftDemoModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(GameLiftDemoModule, GameLiftDemoModuleTypeId, AZ::Module);
        AZ_CLASS_ALLOCATOR(GameLiftDemoModule, AZ::SystemAllocator);

        GameLiftDemoModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                GameLiftDemoSystemComponent::CreateDescriptor(),
                AttachPlayerWeaponComponent::CreateDescriptor(),
            });

            // Register multiplayer components
            CreateComponentDescriptors(m_descriptors);
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<GameLiftDemoSystemComponent>(),
            };
        }
    };
}// namespace GameLiftDemo

#if defined(AZ_MONOLITHIC_BUILD)
    // Monolithic client
    #if defined(O3DE_GEM_NAME)
        AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Client), GameLiftDemo::GameLiftDemoModule)
    #else
        AZ_DECLARE_MODULE_CLASS(Gem_GameLiftDemo_Client, GameLiftDemo::GameLiftDemoModule)
    #endif

    // Monolithic server
    #if defined(O3DE_GEM_NAME)
        AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Server), GameLiftDemo::GameLiftDemoModule)
    #else
        AZ_DECLARE_MODULE_CLASS(Gem_GameLiftDemo_Server, GameLiftDemo::GameLiftDemoModule)
    #endif
#endif

#if defined(O3DE_GEM_NAME)
    AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GameLiftDemo::GameLiftDemoModule)
#else
    AZ_DECLARE_MODULE_CLASS(Gem_GameLiftDemo, GameLiftDemo::GameLiftDemoModule)
#endif
