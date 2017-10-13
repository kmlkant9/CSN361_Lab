/*
compile with -std=c++11 flag
usage: ./client 127.0.0.1 portno
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char **argv) {
 int sockfd,n=0;
 char recvline[1000+1];
 struct sockaddr_in servaddr;

 if(argc!=3) {
  printf("Error with arguments!!!");
  exit(0);
 }
 if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
  printf("Socket Error!!!");
  exit(0);
 }
 bzero(&servaddr, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(atoi(argv[2]));
servaddr.sin_addr.s_addr = inet_addr(argv[1]);
 if(connect(sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr))<0) {
  printf("Connect Error!!!");
  exit(0);
 }
 else {
  while( (n=read(sockfd,recvline,1000))>0) {  
   recvline[n] = 0; /* null terminate */
   if(fputs(recvline,stdout) == EOF) {
    printf("fputs error!!!");
    exit(0);
   }
  }
 }
 if(n<0) {
  printf("read error!!!");
  exit(0); 
 }
 exit(0);
}
