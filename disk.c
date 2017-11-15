
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<malloc.h>
#include<string.h>
#include<time.h>
#include<sys/time.h>

long int block_size=0;
FILE *fptr, *fptr1, *resultdiskreadwrite8b, *resultdiskreadwrite8kb, *resultdiskreadwrite8mb, *resultdiskreadwrite80mb, *resultdiskseqread8b,*resultdiskseqread8kb, *resultdiskseqread8mb, *resultdiskseqread80mb, *resultdiskrandread8b, *resultdiskrandread8kb, *resultdiskrandread8mb, *resultdiskrandread80mb;
clock_t start_time, end_time;
unsigned long file_size=0;
double time_taken=0, latency=0.0, throughput, tt=0;
int thread_count=1;
unsigned long fsize()
{

    fseek(fptr, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(fptr);
    fseek(fptr,0,0);
    return len;
}

void *readandwrite(void *attr)
{
    char* mydata = (char*)malloc(block_size);
    char c;
    unsigned long  i=0;
    file_size = fsize();
    start_time = clock();
    printf("%ld\n", start_time);
    while(i<(file_size/(block_size*thread_count)))
    {

        fread(mydata,block_size,1,fptr);
        fflush(fptr);
        fwrite(mydata,block_size,1,fptr1);
        i++;

    }
    end_time = clock();
    printf("%ld\n", end_time);
    time_taken =(double) (end_time-start_time)/CLOCKS_PER_SEC;
    printf("%ld", time_taken);
    tt += time_taken;
}
void *seq_read(void *attr)
{
	char* mydata = (char*)malloc(block_size);
	//char c;

	//fseek(fptr,0,0);
    unsigned long  i=0;
    file_size = fsize();
    start_time = clock();
    printf("%ld\n", start_time);

    while(i<(file_size/(block_size*thread_count)))
    {

        fread(mydata,block_size,1,fptr);
        fflush(fptr);
         i++;

    }
    end_time = clock();
    printf("%ld\n", end_time);
    time_taken = (double) (end_time-start_time)/CLOCKS_PER_SEC;
    printf("%lf\n", time_taken);
    tt += time_taken;

}
void *rand_read(void *attr)
{
	char* mydata =(char*) malloc(block_size);
	//char c;
	unsigned long  i=0;
        file_size = fsize();
	int randn;
	fseek(fptr,0,0);
    start_time = clock();
    printf("%ld\n", start_time);
    //file_size = fsize(fptr);
    while(i<(file_size/(block_size*thread_count)))
    {
        randn = rand()%block_size;
        fread(mydata,block_size,1,fptr);
        fseek(fptr,randn,SEEK_SET);
        fflush(fptr);
        i++;

    }
    end_time = clock();
    printf("%ld\n", end_time);
    time_taken = (double) (end_time-start_time)/CLOCKS_PER_SEC;
    //tt += time_taken;  
    printf("%ld", time_taken);
    tt += time_taken;


}
int main(int argc, char* argv[])
{
  int num;
  
  fptr = fopen("demo1.txt","r+");
  fptr1 = fopen("sample1.txt","w");
  //char* mydata = malloc(1*block_size);
   int oper=0, i=0;
   setbuf(fptr, NULL);
  unsigned int final_size=0;
  char a = 'a';

   printf("\nOperations: \n1. Read-Write Access \n2. Sequential Read Access \n3. Random Read Access");
   oper = atoi(argv[1]);

   printf("\n Block size:\n Select one of the following - \n1 -> 8B=8\t\t2 -> 8KB=8*1024\t\t3 -> 8MB=8*1048576 \t\t4 -> 80MB= 8*10485760)");
  final_size = atoi(argv[2]);

  printf("Enter number of threads 1,2,4,8");
  thread_count = atoi(argv[3]);

  if(!fptr)
  {
   printf("\n File Not created");
   exit(0);
  }

 
  
  pthread_t  p_thread[thread_count];

    //Assign the final size for execution
  switch(final_size)
  {
                    case 1:
                        block_size = 8;
                        printf("--%ld",block_size);
                        break;
                    case 2:
                        block_size = 8*1024;
                        printf("--%ld",block_size);
                        break;
                    case 3:
                        block_size = 8*1024 * 1024;
                        printf("--%ld",block_size);
                        break;
                    case 4:
                        block_size = 8*1024 * 1024 * 10;
                        printf("--%ld",block_size);
                        break;
                    default:
                        break;
  }



  switch(oper)
  {
                    case 1:
                        for(i=1;i<=thread_count;i++)
                        {
                            pthread_create(&p_thread[i],NULL,readandwrite,NULL);
                            pthread_join(p_thread[i],NULL);
                        }
                        break;
                    case 2:
                        for(i=1;i<=thread_count;i++)
                        {
                            pthread_create(&p_thread[i],NULL,seq_read,NULL);
                            pthread_join(p_thread[i],NULL);
                        }
                        break;
                    case 3:
                         for(i=1;i<=thread_count;i++)
                        {
                            pthread_create(&p_thread[i],NULL,rand_read,NULL);
                            pthread_join(p_thread[i],NULL);
                        }
                        break;
                    default:
                        break;
  }


  printf("\nThread create done");

  printf("\nThread join done");

	//pthread_exit(NULL);
  printf("time taken -- %lf\n", tt);
  if(final_size == 1)
  	latency = ((tt/thread_count)/(file_size/block_size))*1000;
  else if(final_size == 2 || final_size == 3 || final_size == 4)
	throughput = ((file_size/(tt/thread_count))/(1024*1024));
  printf("throughput -- %lf", throughput);
  if(oper == 1 && final_size == 1)
  {
   	resultdiskreadwrite8b = fopen("resultdiskreadwrite8b", "a");
	if(resultdiskreadwrite8b == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskreadwrite8b, "%d", thread_count);
		fprintf(resultdiskreadwrite8b, " ");
		fprintf(resultdiskreadwrite8b, "%lf", latency);
		fprintf(resultdiskreadwrite8b,"\n");
		fclose(resultdiskreadwrite8b);
	}
  }
  else if(oper == 1 && final_size == 2)
  {
   	resultdiskreadwrite8kb = fopen("resultdiskreadwrite8kb", "a");
	if(resultdiskreadwrite8kb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskreadwrite8kb, "%d", thread_count);
		fprintf(resultdiskreadwrite8kb, " ");
		fprintf(resultdiskreadwrite8kb, "%lf", throughput);
		fprintf(resultdiskreadwrite8kb,"\n");
		fclose(resultdiskreadwrite8kb);
	}
  }
  else if(oper == 1 && final_size == 3)
  {
   	resultdiskreadwrite8mb = fopen("resultdiskreadwrite8mb", "a");
	if(resultdiskreadwrite8mb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskreadwrite8mb, "%d", thread_count);
		fprintf(resultdiskreadwrite8mb, " ");
		fprintf(resultdiskreadwrite8mb, "%lf", throughput);
		fprintf(resultdiskreadwrite8mb,"\n");
		fclose(resultdiskreadwrite8mb);
	}
  }
  else if(oper == 1 && final_size == 4)
  {
   	resultdiskreadwrite80mb = fopen("resultdiskreadwrite80mb", "a");
	if(resultdiskreadwrite80mb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskreadwrite80mb, "%d", thread_count);
		fprintf(resultdiskreadwrite80mb, " ");
		fprintf(resultdiskreadwrite80mb, "%lf", throughput);
		fprintf(resultdiskreadwrite80mb,"\n");
		fclose(resultdiskreadwrite80mb);
	}
  }
  else if(oper == 2 && final_size == 1)
  {
   	resultdiskseqread8b = fopen("resultdiskseqread8b", "a");
	if(resultdiskseqread8b == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskseqread8b, "%d", thread_count);
		fprintf(resultdiskseqread8b, " ");
		fprintf(resultdiskseqread8b, "%lf", latency);
		fprintf(resultdiskseqread8b,"\n");
		fclose(resultdiskseqread8b);
	}
  }
  else if(oper == 2 && final_size == 2)
  {
   	resultdiskseqread8kb = fopen("resultdiskseqread8kb", "a");
	if(resultdiskseqread8kb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskseqread8kb, "%d", thread_count);
		fprintf(resultdiskseqread8kb, " ");
		fprintf(resultdiskseqread8kb, "%lf", throughput);
		fprintf(resultdiskseqread8kb,"\n");
		fclose(resultdiskseqread8kb);
	}
  }
  else if(oper == 2 && final_size == 3)
  {
   	resultdiskseqread8mb = fopen("resultdiskseqread8mb", "a");
	if(resultdiskseqread8mb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskseqread8mb, "%d", thread_count);
		fprintf(resultdiskseqread8mb, " ");
		fprintf(resultdiskseqread8mb, "%lf", throughput);
		fprintf(resultdiskseqread8mb,"\n");
		fclose(resultdiskseqread8mb);
	}
  }
  else if(oper == 2 && final_size == 4)
  {
   	resultdiskseqread80mb = fopen("resultdiskseqread80mb", "a");
	if(resultdiskseqread80mb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskseqread80mb, "%d", thread_count);
		fprintf(resultdiskseqread80mb, " ");
		fprintf(resultdiskseqread80mb, "%lf", throughput);
		fprintf(resultdiskseqread80mb,"\n");
		fclose(resultdiskseqread80mb);
	}
  }
  else if(oper == 3 && final_size == 1)
  {
   	resultdiskrandread8b = fopen("resultdiskrandread8b", "a");
	if(resultdiskrandread8b == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskrandread8b, "%d", thread_count);
		fprintf(resultdiskrandread8b, " ");
		fprintf(resultdiskrandread8b, "%lf", latency);
		fprintf(resultdiskrandread8b,"\n");
		fclose(resultdiskrandread8b);
	}
  }
  else if(oper == 3 && final_size == 2)
  {
   	resultdiskrandread8kb = fopen("resultdiskrandread8kb", "a");
	if(resultdiskrandread8kb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskrandread8kb, "%d", thread_count);
		fprintf(resultdiskrandread8kb, " ");
		fprintf(resultdiskrandread8kb, "%lf", throughput);
		fprintf(resultdiskrandread8kb,"\n");
		fclose(resultdiskrandread8kb);
	}
  }
  else if(oper == 3 && final_size == 3)
  {
   	resultdiskrandread8mb = fopen("resultdiskrandread8mb", "a");
	if(resultdiskrandread8mb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskrandread8mb, "%d", thread_count);
		fprintf(resultdiskrandread8mb, " ");
		fprintf(resultdiskrandread8mb, "%lf", throughput);
		fprintf(resultdiskrandread8mb,"\n");
		fclose(resultdiskrandread8mb);
	}
  }
  else if(oper == 3 && final_size == 4)
  {
   	resultdiskrandread80mb = fopen("resultdiskrandread80mb", "a");
	if(resultdiskrandread80mb == NULL)
		printf("\nFile not created");
	else
	{
		fprintf(resultdiskrandread80mb, "%d", thread_count);
		fprintf(resultdiskrandread80mb, " ");
		fprintf(resultdiskrandread80mb, "%lf", throughput);
		fprintf(resultdiskrandread80mb,"\n");
		fclose(resultdiskrandread80mb);
	}
  }
  
  
  fclose(fptr);
  fclose(fptr1);
  return 0;
}

