#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>
#include<malloc.h>
#include <pthread.h>

#define block_size 64*1024
#define IP "127.0.0.1"
#define port 8888

FILE *output;

int sockt,thread_count;
char *src;
char *dest;
long int iteration;

void *TCP_communication(void *attr)
   {
       int i;
       int final_iterate= iteration/thread_count;
       for(i=0;i<final_iterate;i++)
       {
       if( send(sockt , src , strlen(src) , 0) < 0)
           {
               fprintf(output,"\nSend failed");

           }

           if( recv(sockt , dest , block_size , 0) < 0)
           {
               fprintf(output,"\nrecv failed");
           }
       }

       return 0;
   }



int main(int argc , char *argv[])
{
    int i,tc;
    double d1,d2;
    struct timeval start_time,end_time;
    struct sockaddr_in server;
    src = (char *) malloc(block_size);
    dest = (char *) malloc(block_size);
    clock_t start, end;
    double exec_time,time_val,latency,throughput;
    iteration = (8*1024*1024/64);
    printf("thread count: %d",thread_count);
    if(argc>0)
    {
    	tc = atoi(argv[1]);
    }
    thread_count = tc;
    printf("thread count: %d",thread_count);
    pthread_t p_thread[thread_count];
    output = fopen("TCP_client.txt", "a+");

   
   // printf("\nCould not create socket");
    sockt = socket(AF_INET , SOCK_STREAM , 0);
    if (sockt == -1)
    {
        fprintf(output,"\nCould not create socket");
    }
    fprintf(output,"\nSocket created");

    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

      memset(src,'P',block_size);
      if (connect(sockt , (struct sockaddr *)&server , sizeof(server)) < 0)
      {
          fprintf(output,"\nconnect failed. Error");
          return 1;
      }

      fprintf(output,"\nConnected");

      
          //start = clock();
	  gettimeofday(&start_time, NULL );
          for(i=0;i<thread_count;i++)
           {
                  pthread_create(&p_thread[i],NULL,TCP_communication,NULL);
           }
          for(i=0;i<thread_count;i++)
           {
                  pthread_join(p_thread[i],NULL);
           }
           //end = clock();

	   gettimeofday(&end_time, NULL );

	   d1 = (double)start_time.tv_sec + ((double)start_time.tv_usec/1000000);
           d2 = (double)end_time.tv_sec + ((double)end_time.tv_usec/1000000);
           exec_time = d2-d1;

	  
            // time_val = exec_time* 1000;
	   latency = (double)(exec_time*1000)/iteration;

           //exec_time = ((double) (end - start)) / CLOCKS_PER_SEC;
           time_val = exec_time* 1000;
	   //latency = (double)(time_val)/iteration;
	   throughput = (double)(64 * iteration * 8)/exec_time;
           throughput = (double)(throughput*thread_count)/1024;
	   
	   fprintf(output,"\nlatency -> :%lf",latency," ms");
	   fprintf(output,"\nthroughput -> :%lf",throughput," Mb/sec");

       fclose(output);
       close(sockt);
	return 0;
   }

   
