# 1: Overview
**Notice\!** Tutorial requires O3DE engine version greater than 24.09. If you're using 24.09 please add https://github.com/o3de/o3de/commit/aa100770eb744027ef221aeabfb7e9552781595d, https://github.com/o3de/o3de/commit/963f7cf3f90e0f929a4575abf20db805eecafcc8, and https://github.com/o3de/o3de/commit/27fe9c292dc8d556a4fb229cc6a53d5e88fb4fca 
**Notice\!** Tutorial requires Amazon Web Server (AWS) gems. See https://github.com/aws/o3de-repo for install instructions.

This tutorial is just the first step for running your multiplayer project on AWS GameLift. By the end, we will have an O3D game server running in the cloud, with a publicly available IP address that the GameLauncher (running locally) can connect to.   
This **will not** be totally implemented for something a game developer would ship in the wild. The GameLauncher will not have any way of requesting to create a new game, or searching for existing servers to connect to. The game launcher will be at the mercy of the developer to feed it an IP address directly.

You could use this public IP for testing your game in-house for latency, or giving the IP address to testers or friends. It’s a great way (maybe the only way) for doing multiplayer in early development with friends since O3DE doesn’t have a NAT punchthrough, there’s no way of connecting directly from my network to yours since our true IP is hidden behind a router, where as GameLift gives us a public IP address, and some basic security protection.

Time to write new code: 10 minutes  
Exporting a monolithic server: 1 hour  
Testing Locally using GameLift Anywhere: 5 minutes  
Deploying fleet to GameLift: 50 minutes  
Total time: \~2 hours

## ServerLauncher

1. Notify GameLift the process is ready  
2. Listen for GameLift’s Create Session event in order to launch level  
   1. Development branch won’t need this: Multiplayer::SessionNotificationBus::Handler::BusConnect(); OnCreateSessionEnd();

## GameLift

1. Upload the server launcher  
2. Create a Fleet  
3. Create a Game Session  
4. Create a Player Session

## GameLauncher

1. Connect to server given an ip address and player session id

### No GameLift API calls from the Game Launcher

Generally, game clients shouldn't ship with GameLift code for several important reasons:

1. Security: Including GameLift SDK or direct API calls in the client could expose sensitive information or allow unauthorized access to your AWS resources.  
2. Flexibility: Using a backend service allows you to change your matchmaking or session management logic without updating the game client.  
3. Scalability: A backend service can better handle the load of multiple client requests and manage game sessions more efficiently.

Instead of including GameLift code in the client, consider this approach:

1. Implement a backend service that interacts with GameLift using the AWS SDK.  
2. Have your game client communicate with this backend service via a secure API.  
3. The backend service can handle tasks like creating game sessions, managing matchmaking, and providing connection information to clients.

# 2: Create a new game using Multiplayer Template

1. Enable AWSGameLift gem  
2. Compile  
3. Wait for asset processor  
4. Demo the default level

# 3: ServerLauncher

1. Update the Module.cpp AZ\_DECLARE\_MODULE\_CLASS for the client-server split  
2. Dedicated Servers with sv\_gameliftEnabled should NotifyGameLiftProcessReady

\-bg\_ConnectToAssetProcessor=0 Speed up launch time by not loading Asset Processor (we aren’t changing assets anyways)

\--sv\_gameLiftEnabled=true Only perform server logic if the gamelift cvar is set

## Let GameLift system component activate first

**GetRequiredServices()**
```
#if AZ_TRAIT_SERVER && !AZ_TRAIT_CLIENT
    required.push_back(AZ_CRC_CE("AWSGameLiftServerService"));
#endif
```

## Tell GameLift when the server process is ready

Add GameLift Server for the server build dependency, because the server needs to tell AWS that it’s ready to begin hosting

### CMakeList.txt 
```
${gem_name}.Server.Static STATIC
...
    BUILD_DEPENDENCIES
        Gem::AWSGameLift.Server.Static
```
### GameLiftDemoSystemComponent.cpp
```
// Unified launchers and Editors can be either server and client.
// Make sure code only builds in dedicated servers.
#if AZ_TRAIT_SERVER && !AZ_TRAIT_CLIENT
    #include <Request/AWSGameLiftServerRequestBus.h>
#endif

...

GameLiftDemoSystemComponent::Activate()
{
    #if AZ_TRAIT_SERVER && !AZ_TRAIT_CLIENT
        const auto console = AZ::Interface<AZ::IConsole>::Get();
        console->GetCvarValue("sv_gameliftEnabled", gameLiftEnabled);

        if (gameLiftEnabled)
        {
            AWSGameLift::AWSGameLiftServerRequestBus::Broadcast(
                &AWSGameLift::IAWSGameLiftServerRequests::NotifyGameLiftProcessReady);
        }
    #endif
```

## Export Headless Server Launcher

Open Project Manager  
Export settings  
Export \> Windows

## Test Server Locally with GameLift Anywhere™ 

Start up a headless server

Create a game session with game-properties

Check the server log to see that Multiplayer operating in DedicatedServer mode and default level is loaded

*\--game-properties "Key=level,Value=DefaultLevel"*

*aws gamelift create-game-session \--region us-west-2 \--location custom-location-1 \--fleet-id fleet-1234 \--name GameSession1 \--maximum-player-session-count 3 \--game-properties "Key=level,Value=DefaultLevel"*

## GameLift Proper

### Upload Server Build to GameLift
```
aws gamelift upload-build --server-sdk-version 5.1.2 --operating-system WINDOWS_2016 --build-root <path-to-export-folder>\GameLiftDemoHeadlessServerPackage --name GameLiftDemo --build-version v1.0 --region us-west-2
```

### Create GameLift Fleet

```
aws gamelift create-fleet --region us-west-2 --name GameLiftO3DTest --ec2-instance-type c5.xlarge --fleet-type ON_DEMAND --build-id <BuildId> --runtime-configuration "GameSessionActivationTimeoutSeconds=300, ServerProcesses=[{LaunchPath=C:\game\GameLiftDemo.HeadlessServerLauncher.exe, Parameters= --rhi=null -sys_PakPriority=2 -NullRenderer -sv_terminateOnPlayerExit=true -bg_ConnectToAssetProcessor=0 --sv_gameLiftEnabled=true --sv_dedicated_host_onstartup=false, ConcurrentExecutions=1}]" --ec2-inbound-permissions "FromPort=33450,ToPort=34449,IpRange=0.0.0.0/0,Protocol=UDP"
```

*Wait 47 minutes and check GameLift console for updates*

### Create Game Session
```
aws gamelift create-game-session --region us-west-2 --fleet-id fleet-123 --name foogamesession1 --maximum-player-session-count 10 --game-properties "Key=level,Value=DefaultLevel"
```

# 4: ClientLauncher

**Notice\!** There are no GameLift specific API calls required in the game launcher

## Create Player Session and Launch Client

Refer to [MultiplayerSample/MPSGameLift/Documentation](https://github.com/o3de/o3de-multiplayersample/blob/development/MPSGameLift/Documentation/GameLift.md)
```
aws gamelift create-player-session --region us-west-2 --game-session-id <GameSessionId> --player-id Player1
```

Run GameLauncher and connect to GameLift server.

```
--bg_ConnectToAssetProcessor=0 
--connect="35.85.44.17:33450:psess-abcd-ef12-3456"
```


# Future Improvements

## Scripting

In theory, scripting could work. C++ side knows what type of app you’re running. We’re open source, so this a call to action for anyone to add functionality which exposes the app type to scripting.

## Matchmaking

Demonstrating matchmaking using GameLift Flexmatch. 

There’s an example of Matchmaking for Multiplayer Sample, but it requires more AWS backend: 

1. AWS Cognito (for login credentials)  
2. AWS Lambda for RESTful calls the client makes which calls GameLift API on the GameLaunchers behalf  
3. AWS DynamoDb, for storing which clients have requested a match

[o3de-multiplayersample/MPSGameLift/Code/Source/MatchmakingSystemComponent.h](https://github.com/o3de/o3de-multiplayersample/blob/development/MPSGameLift/Code/Source/MatchmakingSystemComponent.h)