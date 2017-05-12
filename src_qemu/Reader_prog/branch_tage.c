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
#define INPUT_FILE "test.txt"
#define OUTPUT_FILE "results.txt"

struct component{
	int32_t pred;  //prediction
	uint32_t tag;  //tag
	int32_t u;     //useful bit
};

typedef struct component * Ts;

struct predictor{

	uint64_t ghr; //global history table
	uint64_t pht; //pattern history table (local)
	uint32_t *bimodal;
	uint32_t historyLength;
	uint32_t num_pht_bimodal_entries;
	uint32_t bimodalLog;
	//to be continued


}

uint64_t compute_history(uint64_t ghr,uint64_t * geometricLength,uint64_t targetLength){ //History to pass to the hash fucntion

//TODO CALCOLARE L'HISTORY 

return h; //return history to be hash with pc
}


int acquire(FILE *fp, uint64_t * buffer){

      if(fread(buffer, sizeof(uint64_t), SIZE, fp) != SIZE)
        return -1;
      else
        return 0;

}


link add_node(uint64_t PC,uint64_t target_addr,uint64_t decision)
{
  link local_link = malloc(sizeof(struct node));
  local_link->PC=PC;
  local_link->state=INITIAL_VALUE;
  local_link->target_addr=target_addr;
  local_link->next=NULL;
  return local_link;
}


bool bimodal_implementation(link * table, uint64_t PC, uint64_t taken, uint64_t target_addr)
{
    bool correct;
    correct=0;
    if(table[APHash(PC)%1024]==NULL)
      {
        table[APHash(PC)%1024]=add_node(PC, target_addr, taken);
        correct = 0;
      }
    else
      {
        link temp_node = table[APHash(PC)%1024];
        while(temp_node->next != NULL && temp_node->PC != PC)
        {
              temp_node = temp_node -> next;
        }
        if(temp_node->PC==PC) //Struct pointed by hash table has the right PC then control if the decision and target address was good
        {
              if (taken == 1) {
                    switch (table[APHash(PC)%1024]->state) {
                        case 0:
                            table[APHash(PC)%1024]->state++;
                            break;
                        case 1:
                            table[APHash(PC)%1024]->state++;
                            /*if(table[APHash(PC)%1024]->targetAddr;!=target_addr)
                              {
                                table.targetAddr[index]=target_addr;
                              }*/
                            break;
                        case 2:
                            table[APHash(PC)%1024]->state++;
                            /*if(table[APHash(PC)%1024]->target_addr == target_addr)
                              {
                                  correct=1;
                              }*/
                              correct=1;
                            /*else
                              {
                                table.targetAddr[index]=target_addr;
                              } */
                             break;
                        case 3:
                            /*if(table[APHash(PC)%1024]->target_addr == target_addr)
                              {
                                correct = 1;
                              }*/
                              correct = 1;
                            /*else
                              {
                                table.targetAddr[index]=target_addr;
                              } */
                            break;
                    }
                } else {
                    switch (table[APHash(PC)%1024]->state) {
                        case 0:
                            correct = 1;
                            break;
                        case 1:
                            correct = 1;
                            table[APHash(PC)%1024]->state--;
                            break;
                        case 2:
                            table[APHash(PC)%1024]->state--;
                            break;
                        case 3:
                            table[APHash(PC)%1024]->state--;
                            break;
                    }
                }

        }
        else
        {
          temp_node->next=add_node(PC,target_addr, taken);
          correct = 0;
        }

      }

    //printf("taken = %" PRIu64 ", state = %d\n", taken, state);
    return correct;
}


int main (void)
{
	uint64_t buffer_read[SIZE];
	FILE *fp;
	FILE *scr;
	fp = fopen(INPUT_FILE,"r");
	scr = fopen(OUTPUT_FILE,"a");
	int iCount = 0;
	int  bimodal2bit1024 = 0;
	bool bimod_tmp1024;
	// table to store prediction outcome
	//00 = strongly not taken;  10 weakly taken;
	//01 = weakly not taken;    11 strongly taken;


	//uint64_t *table1024 = malloc(1024*sizeof(uint64_t));

	//Various length Branch Target Buffers

	//uint64_t *btb1024 = malloc(1024*sizeof(uint64_t));
  //link table1024[1024]={0};
  Ts T[4]={0};
  /*
  for(iCount=0; iCount<1024;iCount++)
    printf("%p\n",table1024[iCount]);
  iCount;
  */

   /* for (int i = 0; i < 1024; i++)
        {
            table1024[i] = INITIAL_VALUE;
            btb1024[i] = INITIAL_ADDRESS;
        }*/


  	while(acquire(fp,buffer_read) != -1)
    {
        //buffer_read0->PC; buffer_read1->Target_Address; buffer_read2->Decision;
        bimod_tmp1024 = bimodal_implementation(table1024, buffer_read[0], buffer_read[2], buffer_read[1]);

       	//printf("Bimodal temp 8 %d\n", bimod_tmp8[0]);
        //statistics counters for T/NT

        bimodal2bit1024 += bimod_tmp1024;
        //statistics counters for addresses





        iCount++; //Instruction Count
      //fprintf(scr,"PC%" PRIu64 "ADDR%" PRIu64 "T/NT%" PRIu64 "\n", buffer_read[0],buffer_read[1],buffer_read[2]);
    }
   	fclose(fp);


    float MPKBr_1K_1024bit =((1-(float)bimodal2bit1024/iCount)*1000);
    float accurate2bit1024 = ((float)bimodal2bit1024 * 100 / iCount);
    fprintf(scr,"accurate2bit1024: %f  MPKBr_1K_1024bit: %f #Instructions: %d\n",accurate2bit1024,MPKBr_1K_1024bit,iCount);
    printf("%f\n",accurate2bit1024 );
    printf("MPKBr_1K : %f\n", MPKBr_1K_1024bit);
    printf("#Instructions : %d\n",iCount );
    fclose(scr);

    //free(table1024);

    //free(btb1024);
  	return 0;
}
