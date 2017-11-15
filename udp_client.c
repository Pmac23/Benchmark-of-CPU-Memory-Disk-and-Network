#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<sys/time.h>
#include<malloc.h>
#include <pthread.h>
 #include <errno.h>

#define block_size 64 * 1023
#define SERVER "127.0.0.1"
#define PORT 8888
#define iteration 131072 // iteration will have the count fot loops which is 64gb/64kb

char *src,*dest;
int sockt, add_size,thread_count;
struct sockaddr_in si_other;

FILE *output;

void *UDP_connection(void *addr)
{
  int i;
  for(i=0;i<iteration;i++)
  {

  if (sendto(sockt, src, strlen(src) , 0 , (struct sockaddr *) &si_other, add_size)==-1)
      {
          fprintf(output, "Error: sendto()", strerror(errno));
          exit(1);
      }

      if (recvfrom(sockt, dest, block_size, 0, (struct sockaddr *) &si_other, &add_size) == -1)
      {
          fprintf(output, "Error: recvfrom()", strerror(errno));
          exit(1);
        }

  }
  return 0;
}


int main(int argc , char *argv[])
{

    int i;
    clock_t start, end;
    double exec_time,time_found,throughput,gv_latency;
    //scanf("%d", &thread_count);
    pthread_t pthread[thread_count];
    output = fopen("UDP_client_output_PP.txt", "a+");

    if(argc>0)
    {
    	thread_count  = atoi(argv[1]);
    }
    
    add_size = sizeof(si_other);
    src = (char *) malloc(block_size);
    dest = (char *) malloc(block_size);

    if ( (sockt=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
          fprintf(output, "Error: socket", strerror(errno));
          exit(1);
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    memset(src,'P',block_size);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(output, "inet_aton() failed\n");
        exit(1);
    }

        start = clock();
        for(i=0;i<thread_count;i++)
        {
              pthread_create(&pthread[i],NULL,UDP_connection,NULL);
       }
      for(i=0;i<thread_count;i++)
       {
              pthread_join(pthread[i],NULL);
       }
      end = clock();
      exec_time = ((float) (end - start)) / CLOCKS_PER_SEC;
      time_found = exec_time * 1000;
      gv_latency = (double)((time_found)/iteration);
      fprintf(output,"\nlatency ->:%lf ms",gv_latency);
      
      throughput = (double)(64 * iteration * 8)/exec_time;
      throughput = (double)(throughput*thread_count)/1024;
      fprintf(output,"\nthroughput ->:%lf Mb/sec",throughput);


  close(sockt);
}



