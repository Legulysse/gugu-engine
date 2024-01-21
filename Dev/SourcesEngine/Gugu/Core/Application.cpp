////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Core/Application.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Application::Application()
{
}

Application::~Application()
{
}

void Application::AppStart()
{
}

void Application::AppStop()
{
}

void Application::AppStep(const DeltaTime& dt)
{
}

void Application::AppUpdate(const DeltaTime& dt)
{
}

void Application::AppLateUpdate(const DeltaTime& dt)
{
}

bool Application::OnMainWindowCloseEvent()
{
    return true;
}

void Application::ComputeCommandLine(const std::string& command, const std::vector<std::string>& args)
{
}

NetPacketGame* Application::ReadGamePacket(sf::Packet* sfPacket)
{
    return nullptr;
}

void Application::PlayerAddedToGame(ClientInfo* client)
{
}

Application* GetApplication()
{
    return GetEngine()->GetApplication();
}

}   // namespace gugu
