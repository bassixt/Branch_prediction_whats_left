#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#define BUFFER_SIZE 384
#define ITERA 100


uint64_t print_with_buffer(uint64_t pc, uint64_t addr)
{
  uint64_t buff[BUFFER_SIZE];
  static int added_length = 0;	
  static int cicle = 0;
  FILE *fp;
  fp = fopen("test.txt", "a");
  if(cicle<4)
		   	{
			   	if(added_length < BUFFER_SIZE-3)
			   	{
			   		buff[added_length] = pc;
			   		buff[added_length+1] = addr;
			   		buff[added_length+2] = 0;
			   		added_length = added_length + 3;
			   		printf("%d\n",added_length);
			   	}
			   	else
			   	{
			   		buff[added_length] = pc;
			   		buff[added_length+1] = addr;
			   		buff[added_length+2] = 0;
			   		fwrite(buff , sizeof(uint64_t) , BUFFER_SIZE  , fp );							//write on file
				 	added_length=0;
				 	cicle = cicle + 1;
			   	}		   	
			}
	fclose(fp);
	return 0;
}



int main (void)
{	
  int i=0;
  //int j;
  //int added_length;
  //int cicle	;
  //uint64_t a=0;
  //uint64_t b=1;
  //uint64_t c=2;
  uint64_t pc=0;
  uint64_t addr=1;
  //uint64_t buffer_read[BUFFER_SIZE];
  //uint64_t buff[BUFFER_SIZE];
  //FILE *fp;
  //fp = fopen("test.txt", "a");
  /*for(j=0;j<ITERA;j++)
	{	  
	  for(i=0;i<BUFFER_SIZE;i+=3)
	  {

	  	buffer_read[i] = a;
   		buffer_read[i+1] = b;
   		buffer_read[i+2] = c;
   		a+=3;
   		b+=3;
   		c+=3;
	  }
	  fwrite(buffer_read , sizeof(uint64_t) , BUFFER_SIZE * 3 , fp );							//write on file
	} */
for(;;)
{	if(i<100000)
	{	
		print_with_buffer(pc,addr);
		pc=pc+3;
	    addr=addr+3;
	    i=i+1;
	}
	else
		break;
}

   return 0;
}

/*
 static int added_length=0;
  char buffer[BUFFER_SIZE];
  if(strlen(intermediate)+added_length<=BUFFER_SIZE)
    {
      added_length += sprintf(buffer+added_length, "%s", intermediate);
      printf("%d\n",added_length );
    }
  else
    {
      printf("\n\n\n Value printed \n\n\n");
      printf("%s\n",buffer);
      added_length=0;
      added_length += sprintf(buffer+added_length, "%s", intermediate);
    }
    */