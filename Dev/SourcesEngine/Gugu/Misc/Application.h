#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/DeltaTime.h"

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

            Application			();
    virtual ~Application		();

	virtual	void AppStart		();
	virtual	void AppStop		();

    virtual void AppStep		(const DeltaTime& dt);
    virtual void AppUpdate		(const DeltaTime& dt);

    virtual void ComputeCommandLine (const std::string& _strCommand, const std::vector<std::string>& _vecArgs);

	virtual NetPacketGame*	ReadGamePacket		(sf::Packet* _pSFPacket);
	virtual void			PlayerAddedToGame	(ClientInfo* pClient);
};

Application* GetApplication();

}   // namespace gugu
