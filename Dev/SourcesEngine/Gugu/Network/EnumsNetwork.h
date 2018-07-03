#pragma once

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
namespace ENetPacket
{
    enum Type
    {
        Undefined = 0,

        Ping,
        Pong,
        ClientInfos,
        ClientDisconnection,

        JoinGame,
        AddPlayer,
        RemovePlayer,
        TurnReady,

        Game,
    };
}

}   // namespace gugu
