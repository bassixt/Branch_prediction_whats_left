#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>


#define BUFFER_SIZE 126
#define ITERA 42000
#define SIZE 3
#define INITIAL_VALUE 3
#define INITIAL_ADDRESS 0

int acquire(FILE *fp, uint64_t * buffer)
{     
      if(fread(buffer, sizeof(uint64_t), SIZE, fp) != SIZE)
        return -1;
      else
        return 0;
  
}
//correct[0] statistics on correctness of decision
//correct[1] statistics on correctness of target address
bool * bimodal_implementation(uint64_t * table, uint64_t * btb, uint64_t index, uint64_t taken, uint64_t target_addr) 
{
    bool *correct = malloc(8*sizeof(bool));
    int state = table[index] % 4;
    correct[0]=0;
    correct[1]=0;
    if (taken == 1) {
        switch (state) {
            case 0:
                table[index]++;
                break;
            case 1:
                table[index]++;
              	if(btb[index]!=target_addr)
              		{
              			btb[index]=target_addr;
              		}
                break;
            case 2:
                table[index]++;
              	if(btb[index] == target_addr)
              		{
              			correct[1] = 1;              			
              		}
              	else
              		{
              			btb[index]=target_addr;
              		}	              	               
                correct[0] = 1;
                break;
            case 3:
              	if(btb[index] == target_addr)
              		{
              			correct[1] = 1;              			
              		}
              	else
              		{
              			btb[index]=target_addr;
              		}	
              	correct[0] = 1;
                break;
        }
    } else {
        switch (state) {
            case 0:
                correct[0] = 1;
                break;
            case 1:
                correct[0] = 1;
                table[index]--;
                break;
            case 2:
                table[index]--;
                break;
            case 3:
                table[index]--;
                break;
        }
    }
    //printf("taken = %" PRIu64 ", state = %d\n", taken, state);
    return correct;
}


int main (void)
{
	//int i;
	//  int j;
	uint64_t buffer_read[SIZE];
	FILE *fp;
	FILE *scr;
	fp = fopen("test.txt","r");
	scr = fopen("results.txt","w");
	int iCount = 0;
	int bimodal2bit8 = 0, bimodal2bit16 = 0, bimodal2bit32 = 0, bimodal2bit128 = 0, 
	    bimodal2bit256 = 0, bimodal2bit512 = 0, bimodal2bit1024 = 0;
	int bimodal2bit8_addr = 0, bimodal2bit16_addr = 0, bimodal2bit32_addr = 0, bimodal2bit128_addr = 0, 
	    bimodal2bit256_addr = 0, bimodal2bit512_addr = 0, bimodal2bit1024_addr = 0;	    
	bool * bimod_tmp8;
	bool * bimod_tmp16;
	bool * bimod_tmp32;
	bool * bimod_tmp128;
	bool * bimod_tmp256;
	bool * bimod_tmp512;
	bool * bimod_tmp1024;
	//Different implementations: table to store prediction outcome
	//00 = strongly not taken;  10 weakly taken;
	//01 = weakly not taken;    11 strongly taken;

	uint64_t *table8 = malloc(8*sizeof(uint64_t));
	uint64_t *table16 = malloc(16*sizeof(uint64_t));
	uint64_t *table32 = malloc(32*sizeof(uint64_t));
	uint64_t *table128 = malloc(128*sizeof(uint64_t));
	uint64_t *table256 = malloc(256*sizeof(uint64_t));
	uint64_t *table512 = malloc(512*sizeof(uint64_t));
	uint64_t *table1024 = malloc(1024*sizeof(uint64_t));

	//Various length Branch Target Buffers
	uint64_t *btb8 = malloc(8*sizeof(uint64_t));
	uint64_t *btb16 = malloc(16*sizeof(uint64_t));
	uint64_t *btb32 = malloc(32*sizeof(uint64_t));
	uint64_t *btb128 = malloc(128*sizeof(uint64_t));
	uint64_t *btb256 = malloc(256*sizeof(uint64_t));
	uint64_t *btb512 = malloc(512*sizeof(uint64_t));
	uint64_t *btb1024 = malloc(1024*sizeof(uint64_t));

    for (int i = 0; i < 8; i++) {
        table8[i] = table16[i] = table32[i] = table128[i] = table256[i] = table512[i] = table1024[i] = INITIAL_VALUE;
        btb8[i] = btb16[i] = btb32[i] = btb128[i] = btb256[i] = btb512[i] = btb1024[i] = INITIAL_ADDRESS;

    }
    for (int i = 8; i < 16; i++) {
        table16[i] = table32[i] = table128[i] = table256[i] = table512[i] = table1024[i] = INITIAL_VALUE;
        btb16[i] = btb32[i] = btb128[i] = btb256[i] = btb512[i] = btb1024[i] = INITIAL_ADDRESS;

    }
    for (int i = 16; i < 32; i++) {
        table32[i] = table128[i] = table256[i] = table512[i] = table1024[i] = INITIAL_VALUE;
        btb32[i] = btb128[i] = btb256[i] = btb512[i] = btb1024[i] = INITIAL_ADDRESS;

    }
    for (int i = 32; i < 128; i++) {
        table128[i] = table256[i] = table512[i] = table1024[i] = INITIAL_VALUE;
        btb128[i] = btb256[i] = btb512[i] = btb1024[i] = INITIAL_ADDRESS;

    }
    for (int i = 128; i < 256; i++) {
        table256[i] = table512[i] = table1024[i] = INITIAL_VALUE;
        btb256[i] = btb512[i] = btb1024[i] = INITIAL_ADDRESS;
 
    }
    for (int i = 256; i < 512; i++) {
        table512[i] = table1024[i] = INITIAL_VALUE;
        btb512[i] = btb1024[i] = INITIAL_ADDRESS;

   	}
    for (int i = 512; i < 1024; i++) {
        table1024[i] = INITIAL_VALUE;
        btb1024[i] = INITIAL_ADDRESS;

    }
  
  //fseek(fp, SEEK_SET, 0);
  /*for(j=0;j<ITERA;j++)
	{
    if(acquire(fp,buffer_read) == -1)
    {
      printf("sbagliato\n");
      return -1;
    }
   
    else{
      //printf("ero qui\n");
     fprintf(scr,"PC%" PRIu64 "ADDR%" PRIu64 "T/NT%" PRIu64 "\n", buffer_read[0],buffer_read[1],buffer_read[2]);
    }
	}*/
  	while(acquire(fp,buffer_read) != -1)
    {
        bimod_tmp8 = bimodal_implementation(table8, btb8, buffer_read[0] % 8, buffer_read[2], buffer_read[1]);
        bimod_tmp16 = bimodal_implementation(table16, btb16, buffer_read[0] % 16, buffer_read[2], buffer_read[1]);
        bimod_tmp32 = bimodal_implementation(table32, btb32, buffer_read[0] % 32, buffer_read[2],buffer_read[1]);
        bimod_tmp128 = bimodal_implementation(table128, btb128, buffer_read[0] % 128, buffer_read[2], buffer_read[1]);
        bimod_tmp256 = bimodal_implementation(table256, btb256, buffer_read[0] % 256, buffer_read[2], buffer_read[1]);
        bimod_tmp512 = bimodal_implementation(table512, btb512, buffer_read[0] % 512, buffer_read[2], buffer_read[1]);
        bimod_tmp1024 = bimodal_implementation(table1024, btb1024, buffer_read[0] % 1024, buffer_read[2], buffer_read[1]);
       
       	//printf("Bimodal temp 8 %d\n", bimod_tmp8[0]);
        //statistics counters for T/NT
        bimodal2bit8 += bimod_tmp8[0];
        bimodal2bit16 += bimod_tmp16[0];
        bimodal2bit32 += bimod_tmp32[0];
        bimodal2bit128 += bimod_tmp128[0];
        bimodal2bit256 += bimod_tmp256[0];
        bimodal2bit512 += bimod_tmp512[0];
        bimodal2bit1024 += bimod_tmp1024[0];
        //statistics counters for addresses
        bimodal2bit8_addr += bimod_tmp8[1];
        bimodal2bit16_addr += bimod_tmp16[1];
        bimodal2bit32_addr += bimod_tmp32[1];
        bimodal2bit128_addr += bimod_tmp128[1];
        bimodal2bit256_addr += bimod_tmp256[1];
        bimodal2bit512_addr += bimod_tmp512[1];
        bimodal2bit1024_addr += bimod_tmp1024[1];



        iCount++; //Instruction Count
      //fprintf(scr,"PC%" PRIu64 "ADDR%" PRIu64 "T/NT%" PRIu64 "\n", buffer_read[0],buffer_read[1],buffer_read[2]);
    }
   	fclose(fp);
  
    float accurate2bit8 =((float)bimodal2bit8 * 100 / iCount);
    float MPKBr_1K_8bit =((1-(float)bimodal2bit8/iCount)*1000);
    float accurate2bit16 =((float)bimodal2bit16 * 100 / iCount);
    float accurate2bit32 = ((float)bimodal2bit32 * 100 / iCount);
    float accurate2bit128 = ((float)bimodal2bit128 * 100 / iCount);
    float accurate2bit256 = ((float)bimodal2bit256 * 100 / iCount);
    float accurate2bit512 = ((float)bimodal2bit512 * 100 / iCount);
    float accurate2bit1024 = ((float)bimodal2bit1024 * 100 / iCount);
    fprintf(scr,"%f,%f,%f,%f,%f,%f,%f\n",accurate2bit8,accurate2bit16,accurate2bit32,accurate2bit128,accurate2bit256,accurate2bit512,accurate2bit1024);
    printf("%f,%f,%f,%f,%f,%f,%f\n",accurate2bit8,accurate2bit16,accurate2bit32,accurate2bit128,accurate2bit256,accurate2bit512,accurate2bit1024 );
    printf("MPKBr_1K : %f\n", MPKBr_1K_8bit);
    fclose(scr);
    free(table8);
    free(table16);
    free(table32);
    free(table128);
    free(table256);
    free(table512);
    free(table1024);
    free(btb8);
    free(btb16);
    free(btb32);
    free(btb128);
    free(btb256);
    free(btb512);
    free(btb1024);
  	return 0;
}
