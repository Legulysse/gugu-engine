#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"
#include "Gugu/Network/EnumsNetwork.h"
#include "Gugu/Network/ClientInfo.h"
#include "Gugu/Network/NetworkPacket.h"

#include <SFML/Network/IpAddress.hpp>

#include <mutex>
#include <list>
#include <map>
#include <queue>

////////////////////////////////////////////////////////////////
// Macros

#ifndef GUGU_PORT_DEFAULT
#define GUGU_PORT_DEFAULT 4567
#endif

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class LoggerEngine;
}

namespace sf
{
    class SocketSelector;
    class TcpListener;
    class Thread;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ManagerNetwork
{
public:

    ManagerNetwork();
    ~ManagerNetwork();

    void StartListening(uint16 _uiPort);
    void StopListening();
    bool IsListening() const;

    void StartReceptionThread();
    void StopReceptionThread();
    void StepReception();
    void Lock();
    void Unlock();
    void ProcessWaitingPackets();

    void ConnectToClient(sf::IpAddress _oIPAddress, uint16 _uiPort);

    //void Disconnect(sf::TcpSocket* _pSocket);
    void Disconnect(ClientInfo* _pClient);
    void DisconnectAll();

    //ClientInfo* FindClient(sf::TcpSocket* _pSocket) const;
    ClientInfo* FindClient(uint32 _oIPAddress, uint16 _uiPort) const;

    void SendNetPacketToAll         (NetPacket* _pPacket, bool _bIncludeSelf);
    void SendNetPacketToAllPlayers  (NetPacket* _pPacket, bool _bIncludeSelf);
    void SendGamePacketToAllPlayers (NetPacketGame* _pPacket, bool _bIncludeSelf);

    bool SendNetPacket(ClientInfo* _pClient, NetPacket& _oPacket);

    bool ReceiveNetPacket(NetPacket* _pPacket, ClientInfo* _pSender);
    void StoreGamePacket(NetPacketGame* _pPacket);

    void HostGame(bool _bJoin);
    void JoinGame(sf::IpAddress _oIPAddress, uint16 _uiPort);

    bool IsHost() const;
    int32 GetPlayerID() const;

    void StartMultiplayerGame();
    bool IsRunningMultiplayerGame() const;

    bool IsReadyForTurn() const;
    void SetTurnPlayed();

    std::list<NetPacketGame*>* GetTurnGamePackets();

    LoggerEngine* GetLogNetwork() const;

private:

    static bool ComparePlayerID(NetPacketGame* _pLeft, NetPacketGame* _pRight);

private:

    sf::SocketSelector* m_selector;
    sf::TcpListener*    m_listener;

    sf::Thread*         m_receptionThread;
    std::mutex          m_mutexReception;

    bool                m_isListening;
    uint16              m_listeningPort;

    ClientInfo*         m_clientInfoSelf;

    bool                m_isRunningMultiplayerGame;
    uint32              m_lastNetTurnProcessed;
    uint32              m_nbTurnsOffset;              //Nb turns used as an offset for sync-games

    std::list<ClientInfo*>  m_clients;
    std::map<uint32, std::list<NetPacketGame*>>    m_gamePackets;  //map <turn, packets>
    
    LoggerEngine*       m_logNetwork;

    struct WaitingPacket
    {
        NetPacket*  packet;
        ClientInfo* sender;
    };

    std::queue<WaitingPacket> m_waitingPackets;
};

void ReceptionThread();

ManagerNetwork* GetNetwork();
LoggerEngine* GetLogNetwork();

}   // namespace gugu
