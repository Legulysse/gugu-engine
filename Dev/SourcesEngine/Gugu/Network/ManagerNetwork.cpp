////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Network/ManagerNetwork.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/String.h"
#include "Gugu/Network/NetworkPacket.h"
#include "Gugu/Core/Application.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/System/Thread.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/Packet.hpp>

#include <set>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerNetwork::ManagerNetwork()
{
    m_selector  = new sf::SocketSelector;
    m_listener  = new sf::TcpListener;
    m_clientInfoSelf = nullptr;

    m_isListening = false;
    m_listeningPort = 0;
    m_isRunningMultiplayerGame = false;
    m_lastNetTurnProcessed = 0;
    m_nbTurnsOffset = 2;

    m_receptionThread = nullptr;
    m_logNetwork = nullptr;
}

ManagerNetwork::~ManagerNetwork()
{
    if (m_isListening)
    {
        StopListening();
        DisconnectAll();
    }

    SafeDelete(m_clientInfoSelf);
    SafeDelete(m_selector);
    SafeDelete(m_listener);

    SafeDelete(m_logNetwork);
}

void ManagerNetwork::StartListening(sf::Uint16 _uiPort)
{
    if (!m_logNetwork)
    {
        m_logNetwork = new LoggerEngine();
        m_logNetwork->SetConsoleOutput(true, false);
        m_logNetwork->SetFilePath("Network.log");
    }

    if(!m_isListening)
    {
        SafeDelete(m_clientInfoSelf);
        m_clientInfoSelf = new ClientInfo(sf::IpAddress::getLocalAddress(), _uiPort);

        m_listeningPort = _uiPort;

        if (m_listener->listen(m_listeningPort) == sf::Socket::Done)
        {
            m_selector->add(*m_listener);

            m_isListening = true;

            GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Listening...");
        }
        else
        {
            GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Can't listen port");
        }
    }
    else
    {
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Already listening a port");
    }
}

void ManagerNetwork::StopListening()
{
    if (m_isListening)
    {
        m_isListening = false;

        StopReceptionThread();

        for (auto iteCurrent = m_gamePackets.begin(); iteCurrent != m_gamePackets.end(); ++iteCurrent)
        {
            ClearStdList((*iteCurrent).second);
        }

        m_selector->clear();
        m_listener->close();

        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Stop listening");
    }
}

bool ManagerNetwork::IsListening() const
{
    return m_isListening;
}

void ManagerNetwork::StartReceptionThread()
{
    StopReceptionThread();

    //Lock();

    if (IsListening())
    {
        if (!m_receptionThread)
            m_receptionThread = new sf::Thread(&ReceptionThread);
        m_receptionThread->launch();
    }

    //Unlock();
}

void ManagerNetwork::StopReceptionThread()
{
    //LogConsole("Wait thread");

    if (m_receptionThread)
    {
        m_receptionThread->wait();
        SafeDelete(m_receptionThread);
    }

    //LogConsole("thread closed");
}

void ReceptionThread()
{
    //LogConsole("Start reception thread");

    //while (GetNetwork()->IsListening())
    {
        GetNetwork()->Lock();
        GetNetwork()->StepReception();
        //GetNetwork()->StopReceptionThread();
        GetNetwork()->Unlock();
    }
}

void ManagerNetwork::Lock()
{
    m_mutexReception.lock();
}

void ManagerNetwork::Unlock()
{
    m_mutexReception.unlock();
}

void ManagerNetwork::ProcessWaitingPackets()
{
    Lock();

    while (!m_waitingPackets.empty())
    {
        WaitingPacket oItem = m_waitingPackets.front();
        m_waitingPackets.pop();

        if (!ReceiveNetPacket(oItem.packet, oItem.sender))
        {
            SafeDelete(oItem.packet);
        }
    }

    Unlock();
}

void ManagerNetwork::StepReception()
{
    if (!m_selector->wait(sf::milliseconds(4)))
        return;

    //Check if a new client is connecting
    if (m_selector->isReady(*m_listener))
    {
        //Accept him
        sf::TcpSocket* pSocketNewClient = new sf::TcpSocket;
        m_listener->accept(*pSocketNewClient);

        sf::IpAddress Address = pSocketNewClient->getRemoteAddress();
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, StringFormat("New client : {0}", Address.toString()));

        //Add him to clients list & selector
        ClientInfo* pClient = new ClientInfo(Address, 0);
        pClient->m_socket = pSocketNewClient;
        m_clients.push_back(pClient);

        m_selector->add(*pSocketNewClient);
    }

    auto iteCLient = m_clients.begin();
    auto iteCLientEnd = m_clients.end();
    while (iteCLient != iteCLientEnd)
    {
        ClientInfo* pClient = *iteCLient;
        ++iteCLient;

        sf::TcpSocket& oSocket = *pClient->m_socket;

        if (!m_selector->isReady(oSocket))
            continue;

        sf::Packet oSFPacket;
        if (oSocket.receive(oSFPacket) == sf::Socket::Done)
        {
            sf::Uint32 iType;
            oSFPacket >> iType;

            ENetPacket::Type eType;
            eType = static_cast< ENetPacket::Type >(iType);

            if(eType != ENetPacket::Undefined)
            {
                NetPacket* pReceivedPacket = nullptr;

                if(eType == ENetPacket::Ping || eType == ENetPacket::Pong || eType == ENetPacket::ClientDisconnection)
                {
                    pReceivedPacket = new NetPacket(eType);
                }
                else if(eType == ENetPacket::ClientInfos)
                {
                    pReceivedPacket = new NetPacketClientConnection(oSFPacket);
                }
                else if (eType == ENetPacket::JoinGame)
                {
                    pReceivedPacket = new NetPacket(eType);
                }
                else if (eType == ENetPacket::AddPlayer)
                {
                    pReceivedPacket = new NetPacketAddPlayer(oSFPacket);
                }
                else if(eType == ENetPacket::TurnReady)
                {
                    pReceivedPacket = new NetPacketTurnReady(oSFPacket);
                }
                else if(eType == ENetPacket::Game)
                {
                    if (GetApplication())
                        pReceivedPacket = GetApplication()->ReadGamePacket(&oSFPacket);
                }

                if(pReceivedPacket)
                {
                    /*if(!ReceiveNetPacket(pReceivedPacket, pClient))
                    {
                        SafeDelete(pReceivedPacket);
                    }*/

                    WaitingPacket oItem;
                    oItem.packet = pReceivedPacket;
                    oItem.sender = pClient;
                    m_waitingPackets.push(oItem);
                }
            }
            else
            {
                GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Error : Unknown Packet received.");
            }
        }
        else
        {
            Disconnect(pClient);
        }
    }
}

void ManagerNetwork::ConnectToClient(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort)
{
    if((sf::IpAddress::LocalHost == _oIPAddress || sf::IpAddress::getLocalAddress() == _oIPAddress) && m_listeningPort == _uiPort)
    {
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Don't connect to yourself !!");
        return;
    }

    if(FindClient(_oIPAddress, _uiPort))
    {
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Already connected");
        return;
    }

    ClientInfo* pClient = new ClientInfo(_oIPAddress, _uiPort);
    pClient->m_socket = new sf::TcpSocket;

    GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, StringFormat("Try connection to {0}:{1}", pClient->m_ipAddress.toString(), pClient->m_port));

    if (pClient->m_socket->connect(pClient->m_ipAddress, pClient->m_port, sf::milliseconds(100)) == sf::Socket::Done)
    {
        m_clients.push_back(pClient);
        m_selector->add(*pClient->m_socket);

        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, StringFormat("Connected to {0}:{1}", pClient->m_ipAddress.toString(), pClient->m_port));

        NetPacketClientConnection oPacket(sf::IpAddress::getLocalAddress(), m_listeningPort);
        SendNetPacket(pClient, oPacket);

        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Sent Connection Infos");
    }
    else
    {
        SafeDelete(pClient);

        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Connection failed");
    }
}

void ManagerNetwork::DisconnectAll()
{
    auto iteClient = m_clients.begin();
    auto iteClientEnd = m_clients.end();
    while(iteClient != iteClientEnd)
    {
        ClientInfo* pClient = *iteClient;
        ++iteClient;

        Disconnect(pClient);
    }
    m_clients.clear();

    GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Disconnected from all clients");
}

/*void ManagerNetwork::Disconnect(sf::TcpSocket* _pSocket)
{
    ClientInfo* pClient = FindClient(_pSocket);

    if(pClient)
    {
        Disconnect(pClient);
    }
}*/

void ManagerNetwork::Disconnect(ClientInfo* _pClient)
{
    sf::TcpSocket* pSocket = _pClient->m_socket;

    m_selector->remove(*pSocket);
    pSocket->disconnect();

    m_clients.remove(_pClient);
    SafeDelete(_pClient);

    GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Disconnected from client");
}

/*ClientInfo* ManagerNetwork::FindClient(sf::TcpSocket* _pSocket) const
{
    FOR_EACH_LIST(ClientInfo*, m_listCLients, pClient)
    {
        if(*pClient->m_pSocket == *_pSocket)
        {
            return pClient;
        }
    }
    FOR_EACH_END

    return nullptr;
}*/

ClientInfo* ManagerNetwork::FindClient(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort) const
{
    for (auto iteCurrent = m_clients.begin(); iteCurrent != m_clients.end(); ++iteCurrent)
    {
        ClientInfo* pClient = *iteCurrent;
        if (pClient->m_ipAddress == _oIPAddress && pClient->m_port == _uiPort)
        {
            return pClient;
        }
    }

    return nullptr;
}

void ManagerNetwork::SendNetPacketToAll(NetPacket* _pPacket, bool _bIncludeSelf)
{
    for (auto iteCurrent = m_clients.begin(); iteCurrent != m_clients.end(); ++iteCurrent)
    {
        ClientInfo* pClient = *iteCurrent;
        if (pClient->m_port != 0)
        {
            SendNetPacket(pClient, *_pPacket);
        }
    }

    if(!_bIncludeSelf || !ReceiveNetPacket(_pPacket, m_clientInfoSelf))
        SafeDelete(_pPacket);
}

void ManagerNetwork::SendNetPacketToAllPlayers(NetPacket* _pPacket, bool _bIncludeSelf)
{
    for (auto iteCurrent = m_clients.begin(); iteCurrent != m_clients.end(); ++iteCurrent)
    {
        ClientInfo* pClient = *iteCurrent;
        if (pClient->m_playerID > -1 && pClient->m_port != 0)
        {
            SendNetPacket(pClient, *_pPacket);
        }
    }

    if(!_bIncludeSelf || m_clientInfoSelf->m_playerID == -1 || !ReceiveNetPacket(_pPacket, m_clientInfoSelf))
        SafeDelete(_pPacket);
}

void ManagerNetwork::SendGamePacketToAllPlayers(NetPacketGame* _pPacket, bool _bIncludeSelf)
{
    _pPacket->m_turn = m_lastNetTurnProcessed + m_nbTurnsOffset + 1;    //m_uiLastNetTurnProcessed + 1 is being processed, and we want this action to be processed X turns later.

    for (auto iteCurrent = m_clients.begin(); iteCurrent != m_clients.end(); ++iteCurrent)
    {
        ClientInfo* pClient = *iteCurrent;
        if (pClient->m_playerID > -1 && pClient->m_port != 0)
        {
            SendNetPacket(pClient, *_pPacket);
        }
    }

    if(!_bIncludeSelf || !ReceiveNetPacket(_pPacket, m_clientInfoSelf))
        SafeDelete(_pPacket);
}

bool ManagerNetwork::SendNetPacket(ClientInfo* _pClient, NetPacket& _oPacket)
{
    sf::Packet oSFPacket;
    _oPacket.FillSFPacket(oSFPacket);

    if(_pClient && _pClient->m_socket && _pClient->m_socket->send(oSFPacket) != sf::Socket::Done)
    {
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "Could not send a packet !");
        return false;
    }

    return true;
}

//return true if packet is stored
bool ManagerNetwork::ReceiveNetPacket(NetPacket* _pPacketReceived, ClientInfo* _pSender)
{
    ENetPacket::Type eType = _pPacketReceived->m_type;

    if(eType == ENetPacket::Ping)
    {
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, ">  pinged !");

        NetPacket oPacket(ENetPacket::Pong);
        SendNetPacket(_pSender, oPacket);
    }
    else if(eType == ENetPacket::Pong)
    {
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "<  ponged !");
    }
    else if(eType == ENetPacket::ClientDisconnection)
    {
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "> got : disconnected !!");

        Disconnect(_pSender);
    }
    else if(eType == ENetPacket::ClientInfos)
    {
        NetPacketClientConnection* pPacket = static_cast<NetPacketClientConnection*>( _pPacketReceived );

        if(pPacket)
        {
            GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "> got : client connection infos !! (" + pPacket->m_ipAddress.toString() + ":" + ToString(pPacket->m_port) + ")");
            _pSender->m_port = pPacket->m_port;
        }
    }
    else if (eType == ENetPacket::JoinGame)
    {
        GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "> A client wants to become a player.");

        if (_pSender != m_clientInfoSelf && _pSender->m_playerID == -1 && IsHost())
        {
            //Store used IDs
            std::set<sf::Int32> oSetIDs;
            for (auto iteCurrent = m_clients.begin(); iteCurrent != m_clients.end(); ++iteCurrent)
            {
                ClientInfo* pClient = *iteCurrent;
                if (pClient->m_playerID >= 0)
                {
                    oSetIDs.insert(pClient->m_playerID);
                }
            }

            //Parse IDs to get the first available one
            sf::Int32 iNewID = 1;
            for (auto iteID = oSetIDs.begin(); iteID != oSetIDs.end(); ++iteID)
            {
                if (*iteID > iNewID)
                    break;

                iNewID = *iteID + 1;
            }

            //Set this ID to the new player, and inform everyone
            _pSender->m_playerID = iNewID;

            NetPacketAddPlayer* pPacketNewPlayer = new NetPacketAddPlayer(_pSender->m_ipAddress, _pSender->m_port, iNewID);
            SendNetPacketToAllPlayers(pPacketNewPlayer, true);

            //Inform this new player of existing players
            for (auto iteCurrent = m_clients.begin(); iteCurrent != m_clients.end(); ++iteCurrent)
            {
                ClientInfo* pClient = *iteCurrent;
                if (pClient->m_playerID >= 0 && pClient->m_playerID != _pSender->m_playerID)
                {
                    NetPacketAddPlayer oPacketOldPlayer(pClient->m_ipAddress, pClient->m_port, pClient->m_playerID);
                    SendNetPacket(_pSender, oPacketOldPlayer);
                }
            }

            //Inform him I am the host, and the player 0
            NetPacketAddPlayer oPacketHostPlayer(m_clientInfoSelf->m_ipAddress, m_clientInfoSelf->m_port, m_clientInfoSelf->m_playerID);
            SendNetPacket(_pSender, oPacketHostPlayer);
        }
    }
    else if(eType == ENetPacket::AddPlayer)
    {
        NetPacketAddPlayer* pPacket = static_cast<NetPacketAddPlayer*>( _pPacketReceived );

        if (m_clientInfoSelf->m_ipAddress == pPacket->m_ipAddress && m_clientInfoSelf->m_port == pPacket->m_port)
        {
            m_clientInfoSelf->m_playerID = pPacket->m_playerID;

            GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, StringFormat("> I am now the player {0}", m_clientInfoSelf->m_playerID));
        }
        else
        {
            ConnectToClient(pPacket->m_ipAddress, pPacket->m_port); //Ensure connection

            for (auto iteCurrent = m_clients.begin(); iteCurrent != m_clients.end(); ++iteCurrent)
            {
                ClientInfo* pClient = *iteCurrent;
                if (pClient->m_ipAddress == pPacket->m_ipAddress && pClient->m_port == pPacket->m_port)
                {
                    pClient->m_playerID = pPacket->m_playerID;

                    if (pClient->m_playerID == 0)
                        pClient->m_isHost = true;

                    GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "> Client " + pClient->m_ipAddress.toString() + ":" + ToString(pClient->m_port) + " is now player " + ToString(pClient->m_playerID));

                    if (GetApplication())
                        GetApplication()->PlayerAddedToGame(pClient);
                }
            }
        }
    }
    else if(eType == ENetPacket::TurnReady)
    {
        NetPacketTurnReady* pPacket = static_cast<NetPacketTurnReady*>( _pPacketReceived );

        if(pPacket)
        {
            GetLogNetwork()->Print(ELog::Info, ELogEngine::Network, "> got : client turn ready !! (" + ToString(pPacket->m_turn) + ")");
            _pSender->m_lastTurnReceived = pPacket->m_turn;
        }
    }
    else if(eType == ENetPacket::Game)
    {
        NetPacketGame* pPacketGame = static_cast<NetPacketGame*>( _pPacketReceived );
        if(pPacketGame)
        {
            StoreGamePacket(pPacketGame);
            return true;
        }
    }

    return false;
}

void ManagerNetwork::StoreGamePacket(NetPacketGame* _pPacket)
{
    m_gamePackets[ _pPacket->m_turn ].push_back(_pPacket);
}

void ManagerNetwork::HostGame(bool _bJoin)
{
    m_clientInfoSelf->m_isHost = true;

    if(_bJoin)
    {
        m_clientInfoSelf->m_playerID = 0;
    }
}

void ManagerNetwork::JoinGame(sf::IpAddress _oIPAddress, sf::Uint16 _uiPort)
{
    //Ask potential host if he is hosting a game
    //send a request to join game
    //ask/receive a game id
    //ask/receive the list of other players

    ClientInfo* pClient = FindClient(_oIPAddress, _uiPort);
    if(pClient)
    {
        NetPacket oPacket(ENetPacket::JoinGame);
        SendNetPacket(pClient, oPacket);
    }
}

bool ManagerNetwork::IsHost() const
{
    if(m_clientInfoSelf)
        return m_clientInfoSelf->m_isHost;
    return false;
}

sf::Int32 ManagerNetwork::GetPlayerID() const
{
    if(m_clientInfoSelf)
        return m_clientInfoSelf->m_playerID;
    return -1;
}

void ManagerNetwork::StartMultiplayerGame()
{
    if(IsListening())
    {
        m_isRunningMultiplayerGame = true;
    }
}

bool ManagerNetwork::IsRunningMultiplayerGame() const
{
    return m_isRunningMultiplayerGame;
}

bool ManagerNetwork::IsReadyForTurn() const
{
    if (IsRunningMultiplayerGame() && m_lastNetTurnProcessed >= m_nbTurnsOffset)
    {
        for (auto iteClient = m_clients.begin(); iteClient != m_clients.end(); ++iteClient)
        {
            if ((*iteClient)->m_lastTurnReceived < m_lastNetTurnProcessed)
            {
                return false;
            }
        }
    }

    return true;
}

void ManagerNetwork::SetTurnPlayed()
{
    if(IsRunningMultiplayerGame())
    {
        ++m_lastNetTurnProcessed;

        NetPacketTurnReady* pPacket = new NetPacketTurnReady(m_lastNetTurnProcessed);
        SendNetPacketToAll(pPacket, false);
    }
}

std::list<NetPacketGame*>* ManagerNetwork::GetTurnGamePackets()
{
    if (m_gamePackets.count(m_lastNetTurnProcessed + 1) == 0)
    {
        return nullptr;
    }

    m_gamePackets[ m_lastNetTurnProcessed + 1 ].sort(ComparePlayerID);

    return &m_gamePackets[ m_lastNetTurnProcessed + 1 ];
}

bool ManagerNetwork::ComparePlayerID(NetPacketGame* _pLeft, NetPacketGame* _pRight)
{
    return false;    //TODO: order packets by player_id
}

LoggerEngine* ManagerNetwork::GetLogNetwork() const
{
    return m_logNetwork;
}

ManagerNetwork* GetNetwork()
{
    return GetEngine()->GetManagerNetwork();
}

LoggerEngine* GetLogNetwork()
{
    return GetNetwork()->GetLogNetwork();
}

}   // namespace gugu
