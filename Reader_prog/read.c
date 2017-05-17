#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#define BUFFER_SIZE 126
#define ITERA 1000

int main (void)
{
  int i;
  int j;
  uint64_t buffer_read[BUFFER_SIZE];
  char buff0[100];
  char buff1[100];
  char buff2[100];
  FILE *fp;
  FILE *scr;
  fp = fopen("test.txt","r");
  scr = fopen("leggi.txt","w");
  //fseek(fp, SEEK_SET, 0);
  for(j=0;j<ITERA;j++)
	{

	  fread(buffer_read, sizeof(uint64_t), BUFFER_SIZE, fp);
	  for(i=0;i<BUFFER_SIZE;i=i+3)
	  { 
	  	sprintf(buff0, "%" PRIu64, buffer_read[i]);
	  	sprintf(buff1, "%" PRIu64, buffer_read[i+1]);
		sprintf(buff2, "%" PRIu64, buffer_read[i+2]);

	  	fprintf(scr,"%s %s %s\n",buff0,buff1,buff2);
	  	//printf("%" PRIu64 "\n",buffer_read[i]);
	    //printf("%" PRIu64 "\n",buffer_read[i+1]);
	    //printf("%" PRIu64 "\n",buffer_read[i+2]);
	  }
	} 
   fclose(fp);
   fclose(scr);
   return 0;
}

