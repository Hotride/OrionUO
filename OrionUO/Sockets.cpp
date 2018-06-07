#include "Sockets.h"
#include <cassert>
#include <string.h>
#include <SDL_timer.h>

struct ICMPHDR
{
    unsigned char Type;
    unsigned char Code;
    uint16_t Checksum;
    uint16_t ID;
    uint16_t Seq;
    char Data;
};

struct ECHOREQUEST
{
    ICMPHDR icmpHdr;
    uint32_t dwTime;
    char cData[64];
};

struct IPHDR
{
    unsigned char VIHL;
    unsigned char TOS;
    short TotLen;
    short ID;
    short FlagOff;
    unsigned char TTL;
    unsigned char Protocol;
    uint16_t Checksum;
    struct in_addr iaSrc;
    struct in_addr iaDst;
};

struct ECHOREPLY
{
    IPHDR ipHdr;
    ECHOREQUEST echoRequest;
    char cFiller[256];
};

uint16_t icmp_checksum(const uint16_t *addr, int count)
{
    uint16_t checksum = 0;
    while (count > 1)
    {
        checksum += *addr++;
        count -= 2;
    }

    if (count > 0)
        checksum += *(unsigned char *)addr;

    while (checksum >> 16)
        checksum = (checksum & 0xffff) + (checksum >> 16);

    return ~checksum;
}

#if defined(ORION_WINDOWS)

static bool wsa_initialized = false;

bool socket_init()
{
    WSADATA wsaData = { 0 };
    wsa_initialized = !WSAStartup(MAKEWORD(2, 2), &wsaData);

    return wsa_initialized;
}

void socket_shutdown()
{
    WSACleanup();
}

tcp_socket tcp_open()
{
    socket_init();

    SOCKET h = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (h == INVALID_SOCKET)
        return nullptr;

    SOCKET *s = new SOCKET;
    *s = h;
    return (tcp_socket)s;
}

bool tcp_connect(tcp_socket socket, const char *address, uint16_t port)
{
    assert(socket);
    SOCKET h = *(SOCKET *)socket;
    assert(h != INVALID_SOCKET);

    sockaddr_in caddr;
    memset(&caddr, 0, sizeof(sockaddr_in));
    caddr.sin_family = AF_INET;

    struct hostent *he;
    int rt = inet_addr(address);
    if (rt != -1)
    {
        caddr.sin_addr.s_addr = rt;
    }
    else
    {
        he = gethostbyname(address);
        if (he == nullptr)
            return false;

        memcpy(&caddr.sin_addr, he->h_addr, he->h_length);
    }

    caddr.sin_port = htons(port);
    if (connect(h, (struct sockaddr *)&caddr, sizeof(caddr)) == -1)
    {
        auto e = WSAGetLastError();
        return false;
    }

    WSASetLastError(0);
    return true;
}

int tcp_select(tcp_socket socket)
{
    assert(socket);
    SOCKET h = *(SOCKET *)socket;
    assert(h != INVALID_SOCKET);

    fd_set rfds;
    struct timeval tv = { 0, 0 };
    FD_ZERO(&rfds);
    FD_SET(h, &rfds);
    return select((int)h, &rfds, nullptr, nullptr, &tv);
}

int tcp_recv(tcp_socket socket, unsigned char *data, size_t max_size)
{
    assert(socket);
    SOCKET h = *(SOCKET *)socket;
    assert(h != INVALID_SOCKET);

    auto s = recv(h, (char *)data, (int)max_size, 0);
    //LOG_DUMP(data, s);
    return s;
}

int tcp_send(tcp_socket socket, unsigned char *data, size_t size)
{
    assert(socket);
    SOCKET h = *(SOCKET *)socket;
    assert(h != INVALID_SOCKET);
    //LOG_DUMP(data, size);
    return send(h, (char *)data, (int)size, 0);
}

void tcp_close(tcp_socket socket)
{
    assert(socket);
    SOCKET h = *(SOCKET *)socket;
    assert(h != INVALID_SOCKET);

    closesocket(h);

    delete (int *)socket;
    socket = nullptr;
}

icmp_handle icmp_open()
{
    socket_init();
    SOCKET h = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (h == INVALID_SOCKET)
        return nullptr;

    if (h == SOCKET_ERROR)
        return nullptr;

    SOCKET *s = new SOCKET;
    *s = h;
    return (icmp_handle)s;
}

int icmp_query(icmp_handle handle, const char *ip, uint32_t *timems)
{
    assert(wsa_initialized);
    assert(handle);
    assert(ip);
    assert(timems);
    SOCKET h = *(SOCKET *)handle;
    assert(h != INVALID_SOCKET);

    LPHOSTENT lpHost = gethostbyname(ip);
    if (lpHost != nullptr)
    {
        sockaddr_in destAddress;
        destAddress.sin_addr.s_addr = ((in_addr *)lpHost->h_addr_list[0])->s_addr;
        destAddress.sin_family = AF_INET;
        destAddress.sin_port = 0;

        const int ICMP_ECHOREQ = 8;
        ECHOREQUEST request = { 0 };
        request.icmpHdr.Type = ICMP_ECHOREQ;
        request.dwTime = *timems;
        memset(request.cData, 80, 64);

        const auto rs = sizeof(ECHOREQUEST);
        request.icmpHdr.Checksum = icmp_checksum((uint16_t *)&request, rs);

        const auto r = (LPSTR)&request;
        const auto dst = (LPSOCKADDR)&destAddress;
        sendto(h, r, rs, 0, dst, sizeof(SOCKADDR_IN));

        timeval tomeoutInfo;
        fd_set readfds = {};
        readfds.fd_count = 1;
        readfds.fd_array[0] = *(SOCKET *)socket;
        tomeoutInfo.tv_sec = 1;
        tomeoutInfo.tv_usec = 0;

        if (select(1, &readfds, nullptr, nullptr, &tomeoutInfo))
        {
            ECHOREPLY answer;
            sockaddr_in sourceAddress;
            int length = sizeof(sockaddr_in);

            const auto a = (LPSTR)&answer;
            const auto as = sizeof(ECHOREPLY);
            const auto src = (LPSOCKADDR)&sourceAddress;
            if (recvfrom(h, a, as, 0, src, &length) != -1)
            {
                *timems = answer.echoRequest.dwTime;
                return 0;
            }
            else
                return -1;
        }
        return -2;
    }
    return -3;
}

void icmp_close(icmp_handle handle)
{
    assert(handle);
    SOCKET h = *(SOCKET *)handle;
    assert(h != INVALID_SOCKET);
    closesocket(h);
}

#else

bool socket_init()
{
    return true;
}

void socket_shutdown()
{
}

tcp_socket tcp_open()
{
    int h = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (h == -1)
        return nullptr;

    LOG("socket opened\n");
    int *s = new int;
    *s = h;
    return (tcp_socket)s;
}

bool tcp_connect(tcp_socket socket, const char *address, uint16_t port)
{
    assert(socket);
    int h = *(int *)socket;
    assert(h != -1);

    sockaddr_in caddr;
    memset(&caddr, 0, sizeof(sockaddr_in));
    caddr.sin_family = AF_INET;

    struct hostent *he;
    int rt = inet_addr(address);
    if (rt != -1)
    {
        caddr.sin_addr.s_addr = rt;
    }
    else
    {
        he = gethostbyname(address);
        if (he == nullptr)
            return false;

        memcpy(&caddr.sin_addr, he->h_addr, he->h_length);
    }
    LOG("socket connected\n");
    caddr.sin_port = htons(port);
    return (connect(h, (struct sockaddr *)&caddr, sizeof(caddr)) != -1);
}

int tcp_select(tcp_socket socket)
{
    assert(socket);
    int h = *(int *)socket;
    assert(h != -1);

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(h, &rfds);

    struct timeval tv = { 0, 0 };

    auto r = select(h, &rfds, nullptr, nullptr, &tv);
    LOG("tcp_select: %d\n", r);
    return r;
}

int tcp_recv(tcp_socket socket, unsigned char *data, size_t max_size)
{
    assert(socket);
    int h = *(int *)socket;
    assert(h != -1);
    auto r = recv(h, data, max_size, 0);
    LOG("RECV: %d\n", r);
    LOG_DUMP(data, r);
    return r;
}

int tcp_send(tcp_socket socket, unsigned char *data, size_t size)
{
    assert(socket);
    int h = *(int *)socket;
    assert(h != -1);

    auto r = send(h, data, size, 0);
    LOG("SEND: %d\n", r);
    LOG_DUMP(data, r);
    return r;
}

void tcp_close(tcp_socket socket)
{
    assert(socket);
    int h = *(int *)socket;
    assert(h != -1);

    close(h);
    LOG("socket closed\n");
    delete (int *)socket;
    socket = nullptr;
}

icmp_handle icmp_open()
{
    int h = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (h == -1)
        return nullptr;

    LOG("icmp opened\n");
    int *s = new int;
    *s = h;
    return (icmp_handle)s;
}

int icmp_query(icmp_handle handle, const char *ip, uint32_t *timems)
{
    assert(handle);
    assert(timems);
    int h = *(int *)handle;
    assert(h != -1);

    LOG("icmp query\n");
    LPHOSTENT lpHost = gethostbyname(ip);
    if (lpHost != nullptr)
    {
        sockaddr_in destAddress;
        destAddress.sin_addr.s_addr = ((in_addr *)lpHost->h_addr_list[0])->s_addr;
        destAddress.sin_family = AF_INET;
        destAddress.sin_port = 0;

        const int ICMP_ECHOREQ = 8;
        ECHOREQUEST request = { 0 };
        request.icmpHdr.Type = ICMP_ECHOREQ;
        request.dwTime = *timems;
        memset(request.cData, 80, 64);

        const auto rs = sizeof(ECHOREQUEST);
        request.icmpHdr.Checksum = icmp_checksum((uint16_t *)&request, rs);

        const auto r = (void *)&request;
        const auto dst = (LPSOCKADDR)&destAddress;
        sendto(h, r, rs, 0, dst, sizeof(SOCKADDR_IN));

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(h, &readfds);

        timeval tv = { 1, 0 };
        if (select(1, &readfds, nullptr, nullptr, &tv))
        {
            ECHOREPLY answer;
            sockaddr_in sourceAddress;

            const auto a = (void *)&answer;
            const auto as = sizeof(ECHOREPLY);
            const auto src = (sockaddr *)&sourceAddress;
            socklen_t length = sizeof(sockaddr_in);
            if (recvfrom(h, a, as, 0, src, &length) != -1)
            {
                *timems = answer.echoRequest.dwTime;
                LOG("icmp: %d\n", *timems);
                return 0;
            }
            else
                return -1;
        }
        return -2;
    }
    return -3;
}

void icmp_close(icmp_handle handle)
{
    assert(handle);
    int h = *(int *)handle;
    assert(h != -1);
    close(h);
    LOG("icmp closed\n");
}

#endif
