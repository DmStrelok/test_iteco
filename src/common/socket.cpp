#include "socket.h"

#include <cstring>

namespace app {

//------------------------------------------------------------------------------
socket::socket(const char *ip_, const std::uint16_t port_)
    : ip{ip_}, port{port_}
{
    sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        std::cout << "Socket error\n";
    create_addr(addr, ip.c_str(), port);
}
//------------------------------------------------------------------------------
socket::~socket()
{
    ::close(sock);
}
//------------------------------------------------------------------------------
void socket::create_addr(sockaddr_in &addr, const char *ip, const std::uint16_t port)
{
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &(addr.sin_addr)) <= 0)
        std::cout << "IP error\n";
}
//------------------------------------------------------------------------------
void socket::bind(int &sock, sockaddr_in &addr)
{
    if (sock < 0) return;
    if (::bind(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) != 0)
        std::cout << "Bind error\n";
}
//------------------------------------------------------------------------------
bool socket::is_valid()
{
    return sock > 0;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
socket_client::socket_client(const char *ip_s, const std::uint16_t port_s, const char *ip_c, const std::uint16_t port_c)
    : socket(ip_s, port_s)
{
    create_addr(addr_client, ip_c, port_c);
    bind(sock, addr_client);
    connect();
}
//------------------------------------------------------------------------------
void socket_client::connect()
{
    if (sock < 0) return;
    if (::connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) != 0)
        std::cout << "Connect error\n";
}
//------------------------------------------------------------------------------
int socket_client::send(const void *msg, int len, int flags)
{
    return ::send(sock, msg, len, flags);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
socket_server::socket_server(const char *ip_s, const std::uint16_t port_s, const char *ip_c, const std::uint16_t port_c)
    : socket(ip_s, port_s)
{
    bind(sock, addr);
    sockaddr_in &adr = *reinterpret_cast<sockaddr_in *>(&addr_client);
    create_addr(adr, ip_c, port_c);
}
//------------------------------------------------------------------------------
int socket_server::recvfrom(void *buf, int len, int flags)
{
    int bytes = ::recvfrom(sock, buf, len, flags, &from, &fromlen);
    if (std::memcmp(from.sa_data, addr_client.sa_data, 6) != 0)
        return -1;
    return bytes;
}
//------------------------------------------------------------------------------

} // namespace app
