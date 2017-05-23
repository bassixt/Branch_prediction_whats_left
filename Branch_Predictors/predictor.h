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
	// First initiating Bimodal Table
   	// Its a simple 2 bit counter table
	pred_str.numBimodalEntries = (1 << BIMODALLOG);
	pred_str.numberTagEntries = (1 << TAGPREDLOG);
	for(uint32_t ii=0; ii < pred_str.numBimodalEntries; ii++){
		pred_str.bimodal[ii]=BIMODAL_CTR_INIT;
	}
	for(uint32_t ii = 0; ii < NUMTAGTABLES ; ii++){
       		pred_str.tagPred[ii] = (struct component *)malloc(sizeof(struct component) * pred_str.numberTagEntries);
    	}
	for(uint32_t ii = 0; ii < NUMTAGTABLES; ii++){
		for(uint32_t j =0; j < pred_str.numberTagEntries; j++){
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
	// Initializing Compressed Buffers.
	// first for index of the the tagged tables
	for(int i = 0; i < NUMTAGTABLES; i++){
		pred_str.indexComp[i].compHist = 0;
		pred_str.indexComp[i].geomLength = pred_str.geometric[i]; 
		pred_str.indexComp[i].targetLength = TAGPREDLOG;
	}
	    
	// next for the tag themselves
	// The tables have different tag lengths
	// T2 and T3 have tag length -> 8
	// T0 and T1 have tag length -> 9
	// second index indicates the Bank no.
	// Reason for using two -> PPM paper... single 
	// CSR is sensitive to periodic patterns in global history which is a common case
	for(int j = 0; j < 2 ; j++){
		for(int i=0; i < NUMTAGTABLES; i++){
			pred_str.tagComp[j][i].compHist = 0;
			pred_str.tagComp[j][i].geomLength = pred_str.geometric[i];
			if(j == 0){
		        	if(i < 2)
		        		pred_str.tagComp[j][i].targetLength = 9 ;
		        	else
		        		pred_str.tagComp[j][i].targetLength = 9 ;    
		   	}
		    	else{
		        	if(i < 2)
		        		pred_str.tagComp[j][i].targetLength = 8 ;
		       	 	else
		        		pred_str.tagComp[j][i].targetLength = 8 ;
		    	}
		}   
	}    
       	// Preditions banks and prediction values 
      	pred_str.primePred = -1;
       	pred_str.altPred = -1;
       	pred_str.primeBank = NUMTAGTABLES;
       	pred_str.altBank = NUMTAGTABLES;
       	for(int i=0; i < NUMTAGTABLES; i++){    
       		pred_str.indexTagPred[i] = 0;
	}
	for(int i=0; i < NUMTAGTABLES; i++){    
    		pred_str.tag[i] = 0;
	}
      	pred_str.clock = 0;
       	pred_str.clock_flip = 1;
       	pred_str.phr = 0;
       	pred_str.altBetterCount = 8;
	//pred_str.ghr.reset(); bisogna fare il free dei ptr
	for(uint32_t ii = 0; ii < NUMTAGTABLES ; ii++){
		free(pred_str.tagPred);
	}
}
#endif

