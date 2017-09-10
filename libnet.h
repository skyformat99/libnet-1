#pragma once
#ifndef LIBNET_H
#define LIBNET_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <ws2ipdef.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdint.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#endif

#define    ADDRESS_TYPE_ERROR   1
#define    ADDRESS_ERROR        2

#define    BUF_NOT_ENOUGH       3
#define    REMALLOC_ERROR       4


	enum iptype {
		IPV4 = 0,
		IPV6
	};

	union ipaddress {
		struct sockaddr_in    ipv4;
		struct sockaddr_in6   ipv6;
	};

	typedef struct _ip {
		enum iptype iptype;
		union ipaddress ipaddress;
	}ip;


	int sock_init();

	///////////////////////////////////////////////////////
	ip ipaddr(const char* name, u_short port, enum iptype iptype);

	const char* addrstr(ip ip, char *str);

	int port(ip ip);

	//////////////////////////////////////////////////////

	int tcpconnect(ip ip);

	int tcpsend(int sock, const char *buf, size_t len);

	int tcprecv(int sock, char *buf, size_t len);

	int tcpbind(ip ip);

	int tcplisten(int sock, int backlog);

	int tcpaccept(int sock, ip clientip);

	int tcprecvuntil(int sock, char *buf, size_t len, const char *s, size_t s_len);

	int tcpclose(int sock);

	////////////////////////////////////////////////////////
	int udpconnect(ip ip);

	int udpbind(int sock, ip ip);

	int udpsend(int sock, ip ip, const char* buf, size_t len);

	int udprecv(int sock, ip ip, char* buf, size_t len);

	int udpclose(int sock);

	////////////////////////////////////////////////////////
	void sock_clean(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif