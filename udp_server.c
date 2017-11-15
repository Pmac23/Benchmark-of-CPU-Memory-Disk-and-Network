#include<stdio.h> 
#include<string.h> 
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <errno.h>

#define PORT 8888
#define final_size 65536



int main(void)
{

    
    FILE *output;
    struct sockaddr_in si_me, si_other;
    int udp, i, recv_len;
    char *des=(char *) malloc(final_size);
    int add_size = sizeof(si_other); 

    output = fopen("UDP_server_output_PP.txt", "a+");

    if ((udp=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
          fprintf(output, "Error: socket", strerror(errno));
          exit(1);
    }

    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

        if( bind(udp , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
        {
          fprintf(output, "Error: bind()", strerror(errno));
          exit(1);
        }

        while(1)
        {
            
            
            if ((recv_len = recvfrom(udp, des, final_size+1, 0, (struct sockaddr *) &si_other, &add_size)) == -1)
            {
                fprintf(output, "Error: recvfrom()", strerror(errno));
          	exit(1);
            }
	    printf("\nPacket received-");
            printf("\nAddress:Port -> %s : %d", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

               if (sendto(udp, des, strlen(des)+1, 0, (struct sockaddr*) &si_other, add_size) == -1)
               {
                   fprintf(output, "Error: sendto()", strerror(errno));
          	   exit(1);
               }
           }

           close(udp);
           return 0;
       }

