
#include <stdio.h>
#include <time.h>
#include<math.h>
#include<pthread.h>


const double NUM_SECONDS = 1.0000000;
unsigned long count = 0;
//const int TOT_TIME = 600;
void *threads_IOPS(void * attr)
{
    FILE *resultcpuv2iop = fopen("resultcpuv2iop.dat", "a");
    if(resultcpuv2iop == NULL)
    {
    	printf("\nFile not found");
    }
    

    double time_counter = 0.0;

    clock_t this_time = clock();
    clock_t last_time;
    int tt =0;
    //unsigned long  j=2900;
    //printf("Gran = %ld\n", NUM_SECONDS * CLOCKS_PER_SEC);
    int a,d;
    int b = 2233;
    int c = 3322;
    //int count = 0;

    while(tt<=75)
    {
        a = b+c;
        d = c-b;

        if(fmod(time_counter,NUM_SECONDS)==0.00)
        {

            //printf("%ld operations in %lf second", count, time_counter/CLOCKS_PER_SEC);
            //this_time = clock();
            tt++;
            fprintf(resultcpuv2iop,"%ld",count);
            //count = 0;
	    fprintf(resultcpuv2iop, "\n"); 
            count = 0;

	}
        count = count+3;

        last_time = clock();
        time_counter = (double)(last_time - this_time)/CLOCKS_PER_SEC;


    }
    fclose(resultcpuv2iop);

    
}

void *threads_FLOPS(void * attr)
{
    
    FILE *resultcpuv2gflop = fopen("resultcpuv2gflop.dat", "a");
    if(resultcpuv2gflop == NULL)
    {
        printf("\nFile not found");
    }
    unsigned long count = 0;

    double time_counter = 0.0;

    clock_t this_time = clock();
    clock_t last_time;
    int tt =0;
    double a;
    double b = 2233.5454;
    double c = 3322.7878;

    while(tt<=75)
    {
        a = b+c;
        //d = c-b;

        if(fmod(time_counter,NUM_SECONDS)==0.00)
        {

            tt++;
            fprintf(resultcpuv2gflop,"%ld", count);
            //count = 0;
	    fprintf(resultcpuv2gflop, "\n");
	    count = 0;
 
        }
        count = count +3;
        //if(tt == 600)
            //break;

        //printf("\nNow I am here ");

        //tt++;
        //count += 4;
        last_time = clock();
        time_counter = (double)(last_time - this_time)/CLOCKS_PER_SEC;


        //printf("--%lf--\n ",time_counter);
        //j--;
    }
    fclose(resultcpuv2gflop);

    //return 0;
}

int main(int argc, char* argv[])
{
 int  ch=0;
	int i;
	

	printf("\nSelect one of the folliwng operations:-");
        
	printf("\n\t1. Integer operations per second - GIOPS");
	//printf("\n\t2. Integer operations per second - GIOPS with AVX");
        printf("\n\t2. Double precision floating point operations per second - GFLOPS");
	//printf("\n\t4. Double precision floating point operations per second - GFLOPS  with AVX");
	//scanf("%d", &ch);
	ch = atoi(argv[1]);


	//create the threads with the count entered above
	pthread_t f_threads[8];

	
	//clock_t start = clock();
	if(ch>0)
	{
		// create threads for IOPS
		if(ch==1)
		{
			for(i=1;i<=8;i++)
			{
				pthread_create(&f_threads[i],NULL,threads_IOPS,NULL);
				pthread_join(f_threads[i],NULL);

			}
			
		}
	}
         else  if(ch==2)
                {
                        for(i=1;i<=8;i++)
                        {
                                pthread_create(&f_threads[i],NULL,threads_FLOPS,NULL);
                                pthread_join(f_threads[i],NULL);

                        }

                }

	return 0;
}
	

	
	


