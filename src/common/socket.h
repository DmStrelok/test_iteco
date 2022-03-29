#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace app {

//------------------------------------------------------------------------------
class socket
{
protected:
    std::string ip;
    std::uint16_t port;
    int sock{-1};
    sockaddr_in addr;
    void bind(int &, sockaddr_in &);
    void create_addr(sockaddr_in &, const char *, const std::uint16_t);

public:
    socket(const char *, const std::uint16_t);
    socket(const socket &) = delete;
    socket(socket &&) = delete;
    socket &operator=(const socket &) = delete;
    socket &operator=(socket &&) = delete;
    ~socket();
    bool is_valid();
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class socket_client : public socket
{
    sockaddr_in addr_client;
    void connect();
public:
    socket_client(const char *, const std::uint16_t, const char *, const std::uint16_t);
    ~socket_client() = default;
    int send(const void *, int, int = 0);
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class socket_server : public socket
{
    sockaddr addr_client;
    sockaddr from;
    socklen_t fromlen;

public:
    socket_server(const char *, const std::uint16_t, const char *, const std::uint16_t);
    ~socket_server() = default;
    int recvfrom(void *, int, int = 0);
};
//------------------------------------------------------------------------------

} // namespace app
#endif // __COMMON_SOCKET_H__
