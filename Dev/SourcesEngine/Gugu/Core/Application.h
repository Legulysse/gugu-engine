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

    // Return true to allow the engine to automatically close the main window, or false to manually handle the event.
    virtual bool OnMainWindowCloseEvent();

    virtual void ComputeCommandLine(const std::string& _strCommand, const std::vector<std::string>& _vecArgs);

    virtual NetPacketGame* ReadGamePacket(sf::Packet* _pSFPacket);
    virtual void PlayerAddedToGame(ClientInfo* pClient);
};

Application* GetApplication();

}   // namespace gugu
