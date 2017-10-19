#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <iostream>
#include <fstream>
using namespace std;

void getAddresses(char *urladdress, char hostAddress[100], char fileAddress[100]);
char* get_hashed_file_name(char*);
int recv_msg(int s);

int main(int argc, char* argv[]) {
  char urladdress[256];
  char user_name[256];
  while(true) {

  printf("%s", "Enter q to quit or Your username to proceed: ");
  scanf("%s", user_name);

  if(strcmp(user_name, "q") == 0) break; //exit client

  struct stat st = {};
  char directory_addr[256];
  sprintf(directory_addr, "/tmp/%s", user_name);
  //create temp profile for user
  if(stat(directory_addr, &st) == -1) {
    mkdir(directory_addr, 0700);
    printf("Created temp profile for user %s\n", user_name);
  }
  while(true) {

  printf("%s", "Enter q to logout or URL to proceed: ");
  scanf("%s", urladdress);

  if(strcmp(urladdress, "q") == 0) break; //logout user

  char hostAddress[256], fileAddress[256];
  getAddresses(urladdress, hostAddress, fileAddress);

  sprintf(directory_addr, "/tmp/%s/%s", user_name, hostAddress);
  if(stat(directory_addr, &st) == -1) {
    mkdir(directory_addr, 0700);
  }
  char *hashed_file_name = get_hashed_file_name(fileAddress);
  printf("%s\n", hashed_file_name);
  //check if file is already present in cache
  char file_addr[256];
  sprintf(file_addr, "%s/%s", directory_addr, hashed_file_name);

  char call_system_cmd[1024];

  if(stat(file_addr, &st) != -1) {
    //file already present
    printf("%s\n", "File present in cache. Loading from cache\n");
    sprintf(call_system_cmd, "xdg-open %s", file_addr);
    system(call_system_cmd);
    continue;
  }

  //socket programming
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo,*p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  status = getaddrinfo(hostAddress, "80", &hints, &servinfo);
  p = servinfo;

  int client_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in remote_address;

  connect(client_socket, p->ai_addr, p->ai_addrlen);

  char request[1024];
  sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection: Close\r\n\r\n",fileAddress, hostAddress);
  printf("%s\n", request);

  send(client_socket, request,sizeof(request), 0);

  int total_recv = recv_msg(client_socket);
  printf("\n\nDone. Received a total of %d bytes\n\n" , total_recv);
  close(client_socket);

  char *fileData = (char*)malloc(sizeof(char)*total_recv);
  ifstream myfile("test", ios::in | ios::binary);

  if (myfile.is_open())
  {
    myfile.read(fileData, total_recv);
    myfile.close();
  }

  //Content-Length
  char content_length_string[30] = "Content-Length: ";
  char *content_length_ptr = strstr(fileData, content_length_string);
  int content_length = atoi(content_length_ptr+strlen(content_length_string));


  char *fileContent = strstr(fileData, "\r\n\r\n");
  char *name_of_output_file = file_addr;
  ofstream ofile(name_of_output_file, ios::out | ios::binary);
  if(ofile.is_open()) {
    ofile.write(fileContent+4, content_length);//-(fileData-fileContent));
  }

  ofile.close();

  pid_t pid = fork();
  if(pid == 0) {
    execlp("/usr/bin/xdg-open", "xdg-open", name_of_output_file, (char *)0);

  }

}//user loop
}//client loop
  return 0;
}


void getAddresses(char *urladdress, char hostAddress[100], char fileAddress[100]) {
  // * if name contains only host name then add '/' at end in url name at runtime
  // . eg. "www.google.com/"

  // remove http or https
  if(strstr(urladdress, "https://")) {
    urladdress = strstr(urladdress, "https://")+8;
  }
  else if(strstr(urladdress, "http://")) {
    urladdress = strstr(urladdress, "http://")+7;
  }


  int index;

  index=0;
  while(index < strlen(urladdress) && urladdress[index] != '/') {
    index++;
  }
  strncpy(hostAddress, urladdress, index-0);
  hostAddress[index]='\0';

  index = index+1;
  strncpy(fileAddress, urladdress+index, strlen(urladdress)-index);
  fileAddress[strlen(urladdress)-index] = '\0';
  return;
}

char* get_hashed_file_name(char *name) {
  if(name == 0 || strlen(name)==0) {
    char *def_name = (char *)malloc(sizeof(char)*strlen("index"));
    char default_name[] = "index";
    memcpy(def_name, default_name, sizeof(default_name));
    def_name[strlen(default_name)]='\0';
    return def_name;
  }
  int lname = strlen(name);
  char *ret_name = (char *)malloc(sizeof(char)*lname/2);
  for(int i=0; i<lname/2; i++) {
    ret_name[i] = (char)(((name[i]+name[lname-i-1])/2)%26+97);
  }
  return ret_name;
}

int recv_msg(int s) {
  char response[4096];
  ofstream myfile("test", ios::binary | ios::out);

  int size_recv , total_size= 0;
  while(1)
    {
        memset(response ,0 , sizeof(response));  //clear the variable
        int sizeof_response = sizeof(response);
        if((size_recv =  recv(s, response, sizeof_response, 0) ) < 0)
        {

        }
        else
        {
            if(size_recv == 0) break;
            total_size += size_recv;
            if(myfile.is_open()) {
              myfile.write(response, size_recv);
            }
        }

    }
    myfile.close();

    return total_size;
}
