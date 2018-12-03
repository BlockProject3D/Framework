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
