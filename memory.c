
#include<stdio.h>
#include<time.h>
#include<pthread.h>
#include<malloc.h>
#include<string.h>
#include<sys/time.h>
#include<fcntl.h>
#include<limits.h>
#include<stdlib.h>

#define SIZE 1024*1024*1024


// Global variables
long int final_size,flag_time=0;
long int data_process_thread; //data processed by each thread
int  thread_count=0;
double gv_latency=0, gv_throughput=0, gv_exec_time=0;
clock_t end_max,start_min=INT_MAX;

 FILE *output;
    

struct timeval start_time,end_time;


void *threads_RW(void *attr)
{

    	double time_found,latency_value,r_throughput;
    	char *src,*dest, *src_t, *dest_t;
    	int i;
	double d1,d2;
	int total_block_pr_thread=0;
	clock_t start,end;

	total_block_pr_thread = data_process_thread/final_size;
	src=(char*)malloc(data_process_thread);//allocate memory
	dest=(char*)malloc(data_process_thread);//allocate memory

	src_t= src;
	dest_t = dest;

	memset(src,'$',data_process_thread);
	
	//start = clock();
	gettimeofday(&start_time, NULL );
	for(i=0;i<total_block_pr_thread;i++)
        {

            memcpy(dest,src,final_size);
	    dest = dest + final_size;
	    src = src + final_size;

        }
	gettimeofday(&end_time, NULL );

	d1 = (double)start_time.tv_sec + ((double)start_time.tv_usec/1000000);
	d2 = (double)end_time.tv_sec + ((double)end_time.tv_usec/1000000);
	
	gv_exec_time = gv_exec_time + (d2-d1);	
	src = src_t;
	dest = dest_t;
	
	free(src);
	free(dest);
}


void *sequential_write(void *attr)
{
    int n = 4,i;
    int arr[n];
    char *str,*temp;   
    double time_found,latency_value;
    int r_throughput;
    double d1,d2;
    int size_leap=0;
    int total_block_pr_thread=0;
    total_block_pr_thread = data_process_thread/final_size;
    str=(char*)malloc(data_process_thread);//allocate memory
    temp = str;
    clock_t start,end;
    // Fill whole array 
    
	gettimeofday(&start_time, NULL );
	for(i=0;i<total_block_pr_thread;i++)
        {


            memset(str,'$',final_size);
	    str = str + final_size;

        }
	gettimeofday(&end_time, NULL );

	d1 = (double)start_time.tv_sec + ((double)start_time.tv_usec/1000000);
	d2 = (double)end_time.tv_sec + ((double)end_time.tv_usec/1000000);
	
	gv_exec_time = gv_exec_time + (d2-d1);
	
   	str = temp;
	free(str);
}


void *random_write(void *attr)
{

    int n = 10,i;
    int arr[n];
    int random=0;
    char *str,*temp;
    double d1,d2;
    double time_found,latency_value;
    clock_t start,end;
    double r_throughput;
    int size_leap=0;
 
    int total_block_pr_thread=0;
    total_block_pr_thread = data_process_thread/final_size;
    str=(char*)malloc(data_process_thread);//allocate memory

    
	gettimeofday(&start_time, NULL );
	for(i=0;i<total_block_pr_thread;i++)
        {
	     random = rand()%(total_block_pr_thread);
	     memset(str,'$',final_size);
	     str = str + final_size;
        }
	
	gettimeofday(&end_time, NULL );

	d1 = (double)start_time.tv_sec + ((double)start_time.tv_usec/1000000);
	d2 = (double)end_time.tv_sec + ((double)end_time.tv_usec/1000000);
	
	gv_exec_time = gv_exec_time + (d2-d1);
	
   	str = temp;
	free(str);
}


int main(int argc, char* argv[])
{
    int block_size=0, oper=0, i=0;
    double time_found,latency_value;
    clock_t start1,end1;
    double r_throughput;
    int total_block_pr_thread=0;

    output = fopen("memory_output.txt", "a+");
    if( output == NULL) 
    {
		printf("Failed to open file for writing\n");
		return -1;
    }
	
    

    fprintf(output,"\nOperations: \n1. Sequential Read-Write Access \n2. Sequential Write Access \n3. Random Write Access ");
    //scanf("%d",&oper);
    oper = atoi(argv[1]);

    fprintf(output,"\n Block size: \n1 -> 8B\t\t2 -> 8KB\t\t3 -> 8MB \t\t4 -> 80MB");
    //scanf("%d",&block_size);
    block_size = atoi(argv[2]);

    fprintf(output,"Number of threads 1,2,4,8");
    //scanf("%d",&thread_count);
    thread_count = atoi(argv[3]);	
    pthread_t  p_thread[thread_count];

    data_process_thread = SIZE/thread_count;
    //Assign the final size for execution
    switch(block_size)
        {
                    case 1:
                        final_size = 8;
                        
                        break;
                    case 2:
                        final_size = 1024*8;
                      
                        break;
                    case 3:
                        final_size = 1024 * 1024*8;
                        
                        break;
                    case 4:
                        final_size = 1024 * 1024 * 10 *8;
                        
                        break;
                    default:

                        break;
        }


	total_block_pr_thread = data_process_thread/final_size;
	    
    switch(oper)
        {
                    case 1:
			 //memory read write operations
			
				
			for(i=1;i<=thread_count;i++)
			{
			    pthread_create(&p_thread[i],NULL,threads_RW,NULL);
			    
			}

			for(i=1;i<=thread_count;i++)
			{
			    pthread_join(p_thread[i],NULL);
			}
		
			
			r_throughput = ((SIZE)/(gv_exec_time/thread_count))/(1024*1024);
			fprintf(output,"\nBlock size: %d",final_size);
			fprintf(output,"\tSequential Read and Write");
			fprintf(output,"\tThread: %d",thread_count);
		        if(final_size==8)
			{
				latency_value = (double)((gv_exec_time/thread_count)*final_size*1000)/(SIZE*sizeof(char));
				fprintf(output,"\nLatency : %lf",latency_value);
			}
			fprintf(output,"\nThroughput: %lf", r_throughput);
	
                        break;
                    case 2:
			 //Sequential Write
			for(i=1;i<=thread_count;i++)
			{
			    pthread_create(&p_thread[i],NULL,sequential_write,NULL);
			}
			for(i=1;i<=thread_count;i++)
			{
				  pthread_join(p_thread[i],NULL);
			}
			
			r_throughput = ((SIZE)/(gv_exec_time/thread_count))/(1024*1024);
			fprintf(output,"\nBlock size: %d",final_size);
			fprintf(output,"\tSequential Write");
			fprintf(output,"\tThread: %d",thread_count);
			if(final_size==8)
			{
				
				latency_value = (double)((gv_exec_time/thread_count)*final_size*1000)/(SIZE*sizeof(char));
				fprintf(output,"\nLatency : %lf",latency_value);
			}
			//fprintf(output,"\nExecution time : %lf",gv_exec_time);
			fprintf(output,"\nThroughput: %lf", r_throughput);
	
                        break;
                    case 3:
			//Random Write
			for(i=1;i<=thread_count;i++)
			{
			    pthread_create(&p_thread[i],NULL,random_write,NULL);
			  
			}
			for(i=1;i<=thread_count;i++)
			{
				  pthread_join(p_thread[i],NULL);
			}
			
			r_throughput = ((SIZE)/(gv_exec_time/thread_count))/(1024*1024);

			
			fprintf(output,"\nBlock size: %d",final_size);
			fprintf(output,"\tRandom Write");
			fprintf(output,"\tThread: %d",thread_count);
			if(final_size==8)
			{
				latency_value = (double)((gv_exec_time/thread_count)*final_size*1000)/(SIZE*sizeof(char)); 
				fprintf(output,"\nLatency : %lf",latency_value);
			}
			fprintf(output,"\nThroughput: %lf", r_throughput);
	
	
                        break;
                    default:
                        break;
        }

	
 fclose(output);
 return 0;
}

