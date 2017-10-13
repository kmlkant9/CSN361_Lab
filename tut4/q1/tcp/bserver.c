/*
compile with -std=c++11 flag
usage ./server portno
*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char **argv)
{
 const int PORT = atoi(argv[1]);
 int listenfd,connfd;
 struct sockaddr_in servaddr;
 char buff[1000];
 time_t ticks;        
 listenfd = socket(AF_INET,SOCK_STREAM,0);

 bzero(&servaddr, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons(PORT); 
 
 if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))!=0)
 	printf("Error while binding\n");
 else printf("bind() successful with port %d\n", PORT);
 
 printf("Listening\n");

 listen(listenfd,8);
 for( ; ; ) {
  connfd = accept(listenfd,(struct sockaddr *)NULL,NULL);
  printf("Accepted a client\n");
  ticks = time(NULL);
  snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
  //snprintf(buff,sizeof(buff),"hello");
  write(connfd,buff,strlen(buff));
  //printf("Going to write\n");
  close(connfd);
 }
}
