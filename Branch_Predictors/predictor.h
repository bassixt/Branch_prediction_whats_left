#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include "common_var.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/////////////// STORAGE BUDGET JUSTIFICATION ////////////////
// Total storage budget: 32KB + 17 bits
// Total PHT counters: 2^17 
// Total PHT size = 2^17 * 2 bits/counter = 2^18 bits = 32KB
// GHR size: 17 bits
// Total Size = PHT size + GHR size
/////////////////////////////////////////////////////////////

void main_predictor(void){
	struct predictor pred_str;//predictor structure declaration
	/// First initiating Bimodal Table
   // Its a simple 2 bit counter table
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
	
	// Geometric lengths of history taken to consider correlation of different age.
    // Table 0 with the longest history as per PPM code
	pred_str.geometric[0] = 130;
	pred_str.geometric[1] = 44;
	pred_str.geometric[2] = 15;
	pred_str.geometric[3] = 5;
	/*this gives 3.41 MPKI !!
     * geometric[0] = 200;
    geometric[1] = 80;
    geometric[2] = 20;
    geometric[3] = 5;*/
    
	
}
#endif

