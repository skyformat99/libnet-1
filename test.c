#include <stdio.h>
#include "libnet.h"

int main() {
	sock_init();
    //init

	ip ip = ipaddr("www.baidu.com", 80, IPV4);

    //tcp client
	int s = tcpconnect(ip);
	int len = tcpsend(s, "GET / HTTP/1.0\r\n\r\n\r\n", 20);
	char buf[1024] = "";
    tcprecv(s,buf,1024);
    puts(buf);

 //    memset(&buf,0, sizeof(buf));
	// tcprecvuntil(s,buf,1024,"\r",1);
	// puts(buf);
 //    tcpclose(s);


 //    //udp client
 //    int u = udpconnect(ip);
 //    udpsend(u,ip,"hello",5);
 //    udprecv(u,ip,buf,1024);
 //    udpclose(u);

 //    //tcp server
 //    ip s_ip = ipaddr("0.0.0.0",1234,IPV4);
 //    int s_s = tcpbind(s_ip);
 //    tcplisten(s,10);
 //    while (1){
 //        ip c_ip;
 //        int c_s = tcpaccept(s,c_s);
 //    }


	sock_clean();

}
