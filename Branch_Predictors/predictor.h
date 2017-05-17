#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include "common_var.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void main_predictor(void){
	struct predictor pred_str;//predictor structure declaration
//(int *)malloc(sizeof(int));
	pred_str.num_pht_bimodal_entries = (1 << BIMODALLOG);
	pred_str.number_tag_entries = (1 << TAGPREDLOG);
	for(uint32_t ii=0; ii < pred_str.num_pht_bimodal_entries; ii++)
	{
	  pred_str.bimodal[ii]=BIMODAL_CTR_INIT;
	}
	 for(uint32_t ii = 0; ii < NUMTAGTABLES ; ii++)
    {
       components[ii] = (struct component *)malloc(sizeof(struct component) * number_tag_entries);
    }//funziona?? 
    //da continuare
    for(uint32_t ii = 0; ii < NUMTAGTABLES; ii++)
    {
        for(UINT32 j =0; j < numTagPredEntries; j++)
        {
            tagPred[ii][j].ctr = 0;
            tagPred[ii][j].tag = 0;
            tagPred[ii][j].usefulBits = 0;
        }
      
    }

}
#endif

