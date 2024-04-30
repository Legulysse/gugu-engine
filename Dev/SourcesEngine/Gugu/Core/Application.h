#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Datasheet;
    class NetPacketGame;
    class ClientInfo;
}

namespace sf
{
    class Packet;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class Application
{
public:

    Application();
    virtual ~Application();

    virtual void AppStart();
    virtual void AppStop();

    virtual void AppStep(const DeltaTime& dt);
    virtual void AppUpdate(const DeltaTime& dt);
    virtual void AppLateUpdate(const DeltaTime& dt);
    virtual void AppUpdateImGui(const DeltaTime& dt);

    // Return true to allow the engine to automatically close the main window, or false to manually handle the event.
    virtual bool OnMainWindowCloseEvent();

    virtual void ComputeCommandLine(const std::string& command, const std::vector<std::string>& args);

    virtual NetPacketGame* ReadGamePacket(sf::Packet* sfPacket);
    virtual void PlayerAddedToGame(ClientInfo* client);
};

Application* GetApplication();

}   // namespace gugu
