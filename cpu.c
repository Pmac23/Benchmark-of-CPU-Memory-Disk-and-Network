//Program to find the benchmark of CPU
//Authors : Prajakta Pardeshi, Priyanka Makhijani

#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include <sys/time.h>
#include<math.h>
#include<immintrin.h>

#define iteration 1e9
int thread_count;
clock_t start, end;
double time_taken, tt;

// function for IOPS
void *avx_threads_IOPS(void * attr)
{
	int j=0;
       //printf("\nFLOPS");
	__m256i vec1 = _mm256_set_epi32(2,3,4,6,9,11,13,15);
	__m256i vec2 = _mm256_set_epi32(1,7,8,10,12,14,16,17);
	__m256i result_add, result_sub;
        start = clock();
        printf("start %lf\n", start);
	for( j = 0; j<iteration/thread_count;j++)
	{
		result_add = _mm256_add_epi32(vec1,vec2);
		result_sub = _mm256_sub_epi32(vec1,vec2);
		
	} 
        end = clock();
        printf("end-- %lf\n", end);
        time_taken = (double)(end-start)/CLOCKS_PER_SEC;
        tt += time_taken;
}

void *threads_IOPS(void * attr)
{
        int j=0;
       //printf("\nFLOPS");
        int  vec1 = 1536;
        int vec2 = 2536;
        int result_add, result_sub;
        start = clock();
	printf("%lf", start);
        for( j = 0; j<iteration/thread_count;j++)
        {
                result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
 		result_add = vec1+vec2;
                result_sub = vec1-vec2;


        } 
        end = clock();
	printf("%lf",end);
        time_taken = (double)(end-start)/CLOCKS_PER_SEC;
        tt += time_taken;
}



// function for OPS
void *avx_threads_FLOPS(void *attr)
{
	int j=0;
       //printf("\nFLOPS");
	
	__m256d vec1 = _mm256_set_pd(2.22222,3.33333,4.44444,6.6666);
	__m256d vec2 = _mm256_set_pd(1.111,7.7777,8.8888,10.1111);
	__m256d result_add, result_sub, result_mul,result_div;
	start = clock();
	for(j = 0; j<iteration/thread_count;j++)
	{
		result_add = _mm256_add_pd(vec1,vec2);
		//result_mul = _mm256_mul_pd(vec1,vec2);
		result_sub = _mm256_sub_pd(vec1,vec2);
		//result_div = _mm256_div_pd(vec1,vec2);
	}
        end = clock();
	time_taken = (double)(end-start)/CLOCKS_PER_SEC;
        tt += time_taken;
	
}

void *threads_FLOPS(void * attr)
{
        int j=0;
       //printf("\nFLOPS");
        double  vec1 = 1536.1536;
        double vec2 = 2536.2536;
        double result_add, result_sub;
        start = clock();
        for( j = 0; j<iteration/thread_count;j++)
        {
                result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;
		result_add = vec1+vec2;
                result_sub = vec1-vec2;


        }
        end = clock();
        time_taken = (double)(end-start)/CLOCKS_PER_SEC;
        tt += time_taken;
}


int main(int argc, char* argv[])
{
	int  ch=0;
	int i;
	//int thread_count;


	float timeTaken=0;
	float time_sum=0;
       
        FILE *resultdiskiop = fopen("resultdiskiop.dat", "a");
        if(resultdiskiop == NULL)
        {
        	printf("\nFile not found");
        }
        FILE *resultdiskgflop = fopen("resultdiskgflop.dat", "a");
        if(resultdiskgflop == NULL)
        {
                printf("\nFile not found");
        }
	FILE *resultdiskiopavx = fopen("resultdiskiopavx.dat", "a");
        if(resultdiskiopavx == NULL)
        {
                printf("\nFile not found");
        }
        FILE *resultdiskgflopavx = fopen("resultdiskgflopavx.dat", "a");
        if(resultdiskgflopavx == NULL)
        {
                printf("\nFile not found");
        }




	// to calculate the elapsed time
	struct timeval begin_time;
	struct timeval last_time; 
	

	printf("\nSelect one of the folliwng operations:-");
        
	printf("\n\t1. Integer operations per second - GIOPS");
	printf("\n\t2. Integer operations per second - GIOPS with AVX");
        printf("\n\t3. Double precision floating point operations per second - GFLOPS");
	printf("\n\t4. Double precision floating point operations per second - GFLOPS  with AVX");
	//scanf("%d", &ch);
	ch = atoi(argv[1]);
	//create threads
	printf("\nPlease enter the number of threads to execute- 1,2,4,8 ");
	//scanf("%d", &thread_count);
        thread_count = atoi(argv[2]);
	//create the threads with the count entered above
	pthread_t f_threads[thread_count];

	
	//clock_t start = clock();
	if(ch>0 && thread_count>0)
	{
		// create threads for IOPS
		if(ch==1)
		{
			for(i=1;i<=thread_count;i++)
			{
				pthread_create(&f_threads[i],NULL,threads_IOPS,NULL);
				pthread_join(f_threads[i],NULL);

			}
			
		}
         else  if(ch==2)
                {
                        for(i=1;i<=thread_count;i++)
                        {
                                pthread_create(&f_threads[i],NULL,avx_threads_IOPS,NULL);
                                pthread_join(f_threads[i],NULL);

                        }

                }
   else if(ch==3)
                {
                        for(i=1;i<=thread_count;i++)
                        {
                                pthread_create(&f_threads[i],NULL,threads_FLOPS,NULL);
                                pthread_join(f_threads[i],NULL);

                        }

                }

		else if(ch == 4)
		{
			for(i=1;i<=thread_count;i++)
			{
				pthread_create(&f_threads[i],NULL,avx_threads_FLOPS,NULL);
				pthread_join(f_threads[i],NULL);

			}
			
		}
	}
	
	
        printf("total_time %lf",tt);
        
	double final_op=(double)(iteration*16*thread_count)/(tt);
        printf("final_op: %lf",final_op);
	switch(ch)
	{
		case 1:
			printf(" GIOPS value -  %f",(double)final_op/1e9);
			//printf(" IOPS value - Giga Hertz is %f",final_op/1000000000);
			fprintf(resultdiskiop,"%lf", final_op/1000000000);
			fprintf(resultdiskiop," ");
                	fprintf(resultdiskiop, "%d", thread_count);
			fprintf(resultdiskiop,"\n");
			break;
		case 2:
                        printf(" GIOPS value with AVX -  %f",(double)final_op/1e9);
                        //printf(" IOPS value - Giga Hertz is %f",final_op/1000000000);
                        fprintf(resultdiskiopavx,"%lf", final_op/1000000000);
                        fprintf(resultdiskiopavx," ");
                        fprintf(resultdiskiopavx, "%d", thread_count);
                        fprintf(resultdiskiopavx,"\n");
                        break;

		case 3:
			//printf(" FLOPS value -  %f",(double)final_op);
		      	printf(" GFLOPS value -  %lf",final_op/1000000000);		               fprintf(resultdiskgflop,"%lf", final_op/1000000000);
                        fprintf(resultdiskgflop," ");
			fprintf(resultdiskgflop,"%d", thread_count);
			fprintf(resultdiskgflop,"\n");
			break;
		case 4:
                        //printf(" FLOPS value -  %f",(double)final_op);
                        printf(" GFLOPS value -  %lf",final_op/1000000000);                             fprintf(resultdiskgflopavx,"%lf", final_op/1000000000);
                        fprintf(resultdiskgflopavx," ");
                        fprintf(resultdiskgflopavx,"%d", thread_count);
                        fprintf(resultdiskgflopavx,"\n");
                        break;

		default:
			break;
	}

	
        fclose(resultdiskiop);
	fclose(resultdiskgflop);
	fclose(resultdiskiopavx);
        fclose(resultdiskgflopavx);

	return 0;
}

