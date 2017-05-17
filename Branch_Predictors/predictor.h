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
       pred_str.tagPred[ii] = (struct component *)malloc(sizeof(struct component) * pred_str.number_tag_entries);
    }//funziona?? 
    //da continuare
    for(uint32_t ii = 0; ii < NUMTAGTABLES; ii++)
    {
        for(uint32_t j =0; j < pred_str.number_tag_entries; j++)
        {
            pred_str.tagPred[ii][j].pred = 0;
            pred_str.tagPred[ii][j].tag = 0;
            pred_str.tagPred[ii][j].u = 0;
        }
      
    }
	
	pred_str.geometric[0] = 130;
	pred_str.geometric[1] = 44;
	pred_str.geometric[2] = 15;
	pred_str.geometric[3] = 5;
	
	
}
#endif

