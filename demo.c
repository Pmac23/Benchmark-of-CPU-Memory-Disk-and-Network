#include<stdio.h>


int main()
{
	FILE* fptr;
	fptr = fopen("demo1.txt","w+");
	if(fptr == NULL)
	 printf("\n not created");
	else
	{
		char a = 'a';
	 	unsigned long j =0;
		for(j=0;j<10E8;j++)
			fputc(a,fptr);
		fclose(fptr);
	}
	return 0;
}


