# libnet
Single file,c socket api wrap,support windows and linux,easy to use

## install
### windows
just copy files to your project 
```c
#include "libnet.h"
```
or
```cmd
git clone https://github.com/p00s/libnet.git
cd libnet
mkdir build
cd build 
cmake ../
open vs and build
```
### linux
```shell
gcc -c libnet.c
ar rcs libnet.a libnet.o
gcc test.c -static -L . -lnet
```
or
```shell
git clone https://github.com/p00s/libnet.git
cd libnet
mkdir build
cd build 
cmake ../
make
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

