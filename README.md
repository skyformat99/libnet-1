# libnet
Single file,c socket api wrap,support windows and linux,easy to use

## install
windows
```c
//just copy file to your project
#include "libnet.h"
```
linux
```
gcc -c libnet.c
ar rcs libnet.a libnet.o
gcc test.c -static -L . -lnet
```
## Documention
libnet just wraps socket api,so easy to use.
### tcp client 
```c
sock_init();
ip ip = ipaddr("www.google.com", 80, IPV4);
int s = tcpconnect(ip);
int len = tcpsend(s, "GET / HTTP/1.0\r\n\r\n\r\n", 20);
char buf[1024] = "";
tcprecv(s,buf,1024);
puts(buf);

memset(&buf,0, sizeof(buf));
tcprecvuntil(s,buf,1024,"\r",1);
puts(buf);
tcpclose(s);
sock_clean();
```
### udp client
```c
...
int u = udpconnect(ip);
udpsend(u,ip,"hello",5);
udprecv(u,ip,buf,1024);
udpclose(u);
...
```
### tcp server
```c
...
ip s_ip = ipaddr("0.0.0.0",1234,IPV4);
int s_s = tcpbind(s_ip);
tcplisten(s,10);
while (1){
    ip c_ip;
    int c_s = tcpaccept(s,c_ip);
}
...
```

