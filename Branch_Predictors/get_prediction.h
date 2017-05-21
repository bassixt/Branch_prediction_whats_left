#ifndef _get_prediction_
#define _get_prediction_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include "common_var.h"
#include "predictor.h"

#define BUFFER_SIZE 126
#define ITERA 42000
#define SIZE 3
#define INITIAL_VALUE 3
#define INITIAL_ADDRESS 0
#define INPUT_FILE "test.txt"
#define OUTPUT_FILE "results.txt"
#define NUMTAGTABLES 4



bool get_prediction(uint64_t PC, struct predictor * tage) //dobbiamo passare la classe predictor
{

bool basePrediction;
uint64_t bimodalIndex   = (PC) % (tage->numBimodalEntries);
uint64_t bimodalCounter = bimodal[bimodalIndex]; 

if(bimodalCounter > BIMODAL_CTR_MAX/2){
    basePrediction =  1; 
  }else{
    basePrediction =  0; 
}

     // Hash to get tag includes info about bank, pc and global history compressed
     // formula given in PPM paper 
     // pc[9:0] xor CSR1 xor (CSR2 << 1)

 for(int i = 0; i < NUMTAGTABLES; i++)
{ 
     tage->tag[i] = PC ^ tage->tagComp[0][i]->compHist ^ (tage->tagComp[1][i]->compHist << 1);
    // These need to be masked
    // 9 bit tags for T0 and T1 // 8 bit tags for T2 and T3
}

tage->tag[0] &= ((1<<9)-1);
tage->tag[1] &= ((1<<9)-1);
tage->tag[2] &= ((1<<9)-1);
tage->tag[3] &= ((1<<9)-1);

// How to get index for each bank ??
// bank 1
// Hash of PC, PC >> index length important , GHR geometric, path info

uint64_t index_mask = ((1<<TAGPREDLOG) - 1);

tage->indexTagPred[0] = PC ^ (PC >> TAGPREDLOG) ^ tage->indexComp[0]->compHist ^ tage->phr ^ (tage->phr >> TAGPREDLOG);
tage->indexTagPred[1] = PC ^ (PC >> (TAGPREDLOG - 1)) ^ tage->indexComp[1]->compHist ^ (tage->phr );
tage->indexTagPred[2] = PC ^ (PC >> (TAGPREDLOG - 2)) ^ tage->indexComp[2]->compHist ^ (tage->phr & 31);
tage->indexTagPred[3] = PC ^ (PC >> (TAGPREDLOG - 3)) ^ tage->indexComp[3]->compHist ^ (tage->phr & 7);

for(int i = 0; i < NUMTAGTABLES; i++)
{
    tage->indexTagPred[i] = tage->indexTagPred[i] & index_mask;
           
}

// get two predictions prime and alt (alternate)
tage->primePred = -1;
tage->altPred = -1;
tage->primeBank = NUMTAGTABLES;
tage->altBank = NUMTAGTABLES;


// See if any tag matches
// T0 with longest history so if hit that awesome

       for(int iterator = 0; iterator < NUMTAGTABLES; iterator++)
       {
           
           
            if(tage->tagPred[iterator][tage->indexTagPred[iterator]]->tag == tage->tag[iterator])
            {
                tage->primeBank = iterator;
                break;
            }  
       }      
            for(int iterator = tage->primeBank + 1; iterator < NUMTAGTABLES; iterator++)
            {
                if(tage->tagPred[iterator][tage->indexTagPred[iterator]]->tag == tage->tag[tage->iterator])
                {
                    tage->altBank = iterator;
                    break;
                }  
            }    
            
       
       
    if(tage->primeBank < NUMTAGTABLES)
    {        
       if(tage->altBank == NUMTAGTABLES)
       {
           tage->altPred = basePrediction;
       }
       else
       {
           if(tage->tagPred[altBank][tage->indexTagPred[tage->altBank]]->ctr >= TAGPRED_CTR_MAX/2)
                tage->altPred = TAKEN;
            else 
                tage->altPred = NOT_TAKEN;
       }
        
        if((tage->tagPred[primeBank][tage->indexTagPred[tage->primeBank]]->ctr  != 3) ||(tage->tagPred[tage->primeBank][tage->indexTagPred[tage->primeBank]]->ctr != 4 ) || (tage->tagPred[tage->primeBank]tage->[indexTagPred[primeBank]]->usefulBits != 0) || (tage->altBetterCount < 8))
        {
            if(tage->tagPred[tage->primeBank][tage->indexTagPred[tage->primeBank]]->ctr >= TAGPRED_CTR_MAX/2)
                tage->primePred = TAKEN;
            else 
                tage->primePred = NOT_TAKEN;
            return tage->primePred;
        }
        else
        {
            return tage->altPred;
        }
    }
    else
    {
        tage->altPred = basePrediction;
        return tage->altPred;
    }
}


#endif
