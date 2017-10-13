/*
usage: ./hostNameFromIP IPaddress
*/
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {

  char host[1024];
  char service[20];
  struct sockaddr_in sa;
  struct sockaddr_in6 sa6;

  if(argc<2){
		printf("Enter ip as an argument.\n");
		return 0;
	}

  bool isipv4 = true;
  for(int i=0; i<strlen(argv[1]); i++) {
    if(argv[1][i]==':') isipv4 = false;
  }

  if(isipv4) {
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &sa.sin_addr.s_addr);
    getnameinfo((struct sockaddr *)&sa, sizeof sa, host, sizeof host, service, sizeof service, 0);
  } else {
    sa6.sin6_family = AF_INET6;
    inet_pton(AF_INET6, argv[1], &sa6.sin6_addr.s6_addr);
    getnameinfo((struct sockaddr *)&sa6, sizeof sa6, host, sizeof host, service, sizeof service, 0);
  }

  printf("Host: %s\n", host);
  printf("Service: %s\n", service);

  return 0;
}
