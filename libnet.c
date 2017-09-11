
#include "libnet.h"

int sock_init()
{
#ifdef _WIN32
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
		errno = WSAGetLastError();
		return -1;
	}
	return 0;
#else
	return 0;
#endif
}

ip ipaddr(const char* name, u_short port, enum iptype iptype)
{
	ip          ip = { 0 };
	char        host[100] = { 0 };
	struct      hostent *hp;
	if (iptype == IPV6) {
		ip.iptype = IPV6;
		ip.ipaddress.ipv6.sin6_port = htons(port);
        ip.ipaddress.ipv6.sin6_family = AF_INET6;
	}
	else {
		ip.iptype = IPV4;
		ip.ipaddress.ipv4.sin_port = htons(port);
        ip.ipaddress.ipv4.sin_family =AF_INET;
	}
	hp = gethostbyname(name);
	if (hp) {
		if (hp->h_addrtype == AF_INET)
			ip.ipaddress.ipv4.sin_addr =
                    *(struct in_addr*)hp->h_addr_list[0];
		else
			ip.ipaddress.ipv6.sin6_addr =
                    *(struct in6_addr*)hp->h_addr_list[0];
	}
	else {
		errno = ADDRESS_ERROR;
		return ip;
	}
	return ip;
}

const char* addrstr(ip ip, char *str)
{
	if (ip.iptype == IPV4) {
		return inet_ntop(AF_INET, &(ip.ipaddress.ipv4.sin_addr),
                         str, INET_ADDRSTRLEN);
	}
	else
	{
		return inet_ntop(AF_INET6, &(ip.ipaddress.ipv6.sin6_addr),
                         str, INET6_ADDRSTRLEN);
	}
}

int port(ip ip)
{
	return ip.iptype == IPV4 ? ntohs(ip.ipaddress.ipv4.sin_port)
                             : ntohs(ip.ipaddress.ipv6.sin6_port);
}

int tcpconnect(ip ip)
{
	if (ip.iptype == IPV4) {
		int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == -1) {
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			return NULL;
		}
		ip.ipaddress.ipv4.sin_family = AF_INET;
		if (connect(sock, (struct sockaddr *) &ip.ipaddress.ipv4,
                    sizeof(ip.ipaddress.ipv4)) !=0)
        {
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			tcpclose(sock);
			return NULL;
		}
		return sock;
	}
	else
    {
		int sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
		if (sock == -1)
		{
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			return NULL;
		}
		ip.ipaddress.ipv6.sin6_family = AF_INET6;
		if (connect(sock, (struct sockaddr*)&ip.ipaddress.ipv6,
                    sizeof(ip.ipaddress.ipv6)) !=0)
		{
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 	
			tcpclose(sock);
			return NULL;
		}
		return sock;
	}
}


int tcpsend(int sock, const char *buf, size_t len)
{
	return send(sock, buf, len, 0);
}

int tcprecv(int sock, char *buf, size_t len)
{
	return recv(sock, buf, len, 0);
}

int tcpbind(ip ip)
{
	if (ip.iptype == IPV4) {
		int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == -1) {
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			return NULL;
		}
		if (bind(sock, (struct sockaddr *) &ip.ipaddress.ipv4,
                 sizeof(ip.ipaddress.ipv4)) != 0) {
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			return NULL;
		}
	}
	else
    {
		int  sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
		if (sock == -1) {
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			return NULL;
		}

		if (bind(sock, (struct sockaddr *)&ip.ipaddress.ipv6,
                 sizeof(ip.ipaddress.ipv6)) !=0) {
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			return NULL;
		}
		return sock;
	}
}

int tcplisten(int sock, int backlog)
{
	return listen(sock, backlog);
}

int tcpaccept(int sock, ip clientip)
{
	int addrlen = sizeof(struct sockaddr);
	return accept(sock, (struct sockaddr*)&clientip.ipaddress.ipv4, 
		&addrlen);
}


int tcprecvuntil(int sock, char *buf, size_t len, const char *s, size_t s_len)
{
	size_t i;
	for (i = 0; i != len; ++i, ++buf) {
		int len = tcprecv(sock, buf, 1);
		if (len == 1) {
			size_t j;
			for (j = 0; j != s_len; ++j)
				if (*buf == s[j])
					return i + 1;
		}
		if (errno != 0)
			return i + len;
	}
	errno = BUF_NOT_ENOUGH;
	return len;
}


int tcpclose(int sock) {
#ifdef _WIN32
	closesocket(sock);
#else
	close(sock);
#endif
}

int udpconnect(ip ip)
{
	if (ip.iptype == IPV4) {
		int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock == -1) {
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			return NULL;
		}
		return sock;
	}
	else {
		int sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
		if (sock == -1) {
#ifdef _WIN32
			errno = WSAGetLastError();
#else
			errno = errno;
#endif 
			return NULL;
		}
		return sock;
	}
}


int udpbind(int sock, ip ip)
{
    if (ip.iptype == IPV4)
		return bind(sock, (struct sockaddr*)&ip.ipaddress.ipv4,
                    sizeof(ip.ipaddress.ipv4));
	else
		return bind(sock, (struct sockaddr*)&ip.ipaddress.ipv6,
                    sizeof(ip.ipaddress.ipv6));
}


int udpsend(int sock, ip ip, const char* buf, size_t len)
{
	if (ip.iptype == IPV4) {
		return sendto(sock, buf, len, 0,\
         (struct sockaddr*)&ip.ipaddress.ipv4, sizeof(ip.ipaddress.ipv4));
	}
	else {
		return sendto(sock, buf, len, 0, \
        (struct sockaddr *)&ip.ipaddress.ipv6, sizeof(ip.ipaddress.ipv6));
	}
}

int udprecv(int sock, ip ip, char* buf, size_t len)
{
	if (ip.iptype == IPV4) {
		return recvfrom(sock, buf, len, 0, (struct sockaddr*)&ip.ipaddress.ipv4,
                        sizeof(ip.ipaddress.ipv4));
	}
	else {
		return recvfrom(sock, buf, len, 0, (struct sockaddr*)&ip.ipaddress.ipv6,
                        sizeof(ip.ipaddress.ipv6));
	}
}

int udpclose(int sock)
{
#ifdef _WIN32
	closesocket(sock);
#else
	close(sock);
#endif
}

void sock_clean(void) {
#ifdef _WIN32
	WSACleanup();
#endif
}