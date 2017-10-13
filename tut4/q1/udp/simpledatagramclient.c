#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXBUFLEN 100

int main(int argc, char *argv[]) {
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr;
  socklen_t addr_len;
  int rv;
  int numbytes;
  char buf[MAXBUFLEN];

  if(argc!=3) {
    fprintf(stderr, "usage: hostname port message\n");
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for(p = servinfo; p != NULL; p = p->ai_next) {
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("talker: socket");
      continue;
    }
    break;
  }

  if(p == NULL) {
    fprintf(stderr, "failed to create socket\n");
    exit(1);
  }

  char* msg="Hello server\n";
  if ((numbytes = sendto(sockfd, msg, strlen(msg), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }


    //printf("talker: sent %d bytes to %s", numbytes, argv[1]);

    //memset(their_addr, 0, sizeof(their_addr));
    //memset(addr_len, 0, sizeof(addr_len));
    memset(buf, 0, sizeof(buf));
    addr_len = sizeof(their_addr);
    if((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
      perror("recvfrom");
      exit(1);
    }
    buf[numbytes] = '\0';
    printf("%s\n",buf);

    freeaddrinfo(servinfo);
    close(sockfd);

    return 0;
}
