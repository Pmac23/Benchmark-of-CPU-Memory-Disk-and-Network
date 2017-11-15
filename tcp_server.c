#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<malloc.h>

#define block_size 64 * 1024

void *TCP_connection(void *socketd)
{

  int sock = *(int*)socketd;
  int read;
  char *des=(char *) malloc(block_size);
  
  while( (read = recv(sock , des , block_size , 0)) > 0 )
  {

      write(sock , des , strlen(des));
  }

 
  if(read == -1)
  {
      perror("recv failed");
  }

    free(socketd);

    return 0;
}


int main(int argc , char *argv[])
{
    int socketd , csock , c , *nsock, read_size;
    struct sockaddr_in server , client;
    char *des=(char *) malloc(block_size);

    socketd = socket(AF_INET , SOCK_STREAM , 0);
    if (socketd == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    if( bind(socketd,(struct sockaddr *)&server , sizeof(server)) < 0)
    {

              perror("bind failed. Error");
              return 1;
    }

          listen(socketd , 3);

          
          c = sizeof(struct sockaddr_in);

          while( (csock = accept(socketd, (struct sockaddr *)&client, (socklen_t*)&c)) )
          {
              

              pthread_t pthread;
              nsock = malloc(1);
              *nsock = csock;

              if( pthread_create( &pthread , NULL ,  TCP_connection , (void*) nsock) < 0)
              {
                  perror("could not create thread");
                  return 1;
              }

              puts("Handler assigned");
          }

          if (csock < 0)
          {
              perror("accept failed");
              return 1;
  }

  return 0;
}


