// Copyright (c) 2020, BlockProject 3D
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject 3D nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef NETWORK_H_
# define NETWORK_H_

namespace Framework
{
    class ENGINE_API ITCPClient
    {
    public:
        virtual ~ITCPClient() {}
        virtual IInputStream *GetInStream() = 0;
        virtual IOutputStream *GetOutStream() = 0;
    };

    class ENGINE_API IUDPClient
    {
    public:
        virtual ~IUDPClient() {}
        virtual char *GetIP() = 0; //WTF IS THAT ?!
        virtual int GetPort() = 0;
    };

    class ENGINE_API ITCPSocketClient
    {
    public:
        virtual ~ITCPSocketClient() {}
        virtual IInputStream *GetInStream() = 0;
        virtual IOutputStream *GetOutStream() = 0;
        virtual void Close() = 0;
    };

    class ENGINE_API ITCPSocketServer
    {
    public:
        virtual ~ITCPSocketServer() {}
        virtual ITCPClient *Accept() = 0;
        virtual void Close() = 0;
    };

    class ENGINE_API IUDPSocket
    {
    public:
        virtual ~IUDPSocket() {}
        virtual void Initialize(const char *ip, int port) = 0;
        virtual void SendTo(IUDPClient *cl, uint8 *data, uint32 size) = 0;
        virtual void ReadFrom(IUDPClient &cl, uint8 *buffer, uint32 bufsize) = 0;
        virtual void Close() = 0;
    };

    /**
     * This is low-level network platform abstraction layer
     * Prefer to use high level APIs
     */
    class ENGINE_API INetworkManager
    {
    public:
        virtual ~INetworkManager() {}
        virtual ITCPSocketClient *ConnectTCPServer(const char *ip, int port) = 0;
        virtual ITCPSocketServer *CreateTCPServer(const char *defaultIP, int defaultPort) = 0;
        virtual IUDPSocket *NewUDPSocket() = 0;
        virtual void DeleteUDPSocket(IUDPSocket *sock) = 0;
        virtual void DestroyTCPServer(ITCPSocketServer *sock) = 0;
        virtual void DestroyTCPClient(ITCPSocketClient *sock) = 0;
    };
};

#endif /* !NETWORK_H_ */
