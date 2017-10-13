/*To use
Compile: gcc multiroomclient.c -o multiroomclient
Execute: ./multiroomclient localhost PORTNO
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT "3490"
#define MAXDATASIZE 100

void *get_in_addr(struct sockaddr *sa) {
  if(sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
  fd_set read_fds;  // temp file descriptor list for select()
  int fdmax;        // maximum file descriptor number


  int sockfd, numbytes;
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];

  if(argc != 3) {
    fprintf(stderr, "usage: client hostname PORT\n");
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo))) {
    fprintf(stderr, "getaddrinfo%s\n", gai_strerror(rv));
    return 1;
  }

  //loop thru results
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }

    if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }


  if(p == NULL) {
    fprintf(stderr, "client failed to connect\n");
    return 2;
  }
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));

  printf("client: connecting to %s\n", s);
  freeaddrinfo(servinfo);

/*
  FD_ZERO(&read_fds);
  FD_SET(0, &read_fds);
  FD_SET(sockfd, &read_fds);
  fdmax = sockfd+1;
*/
  while(1) {
    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);
    FD_SET(sockfd, &read_fds);
    fdmax = sockfd+1;

    if(select(fdmax, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select");
      exit(4);
    }
    if(FD_ISSET(0, &read_fds)) {
      memset(buf, 0, sizeof(buf));
      scanf("%s", buf);
      send(sockfd, buf, strlen(buf), 0);
    }
    if(FD_ISSET(sockfd, &read_fds)) {
      memset(buf, 0, sizeof(buf));
      if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
      }
      buf[numbytes] = '\0';
      printf("%s\n", buf);
    }

  }
  close(sockfd);
  return 0;
}
