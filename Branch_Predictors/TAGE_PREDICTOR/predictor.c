#include "common_var.h"
#include "get_prediction.h"
#include "update_predictor.h"
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

int main(void){
		
	uint64_t buffer_read[SIZE];
	FILE *fp;
	FILE *scr;
	fp = fopen(INPUT_FILE,"r");
	scr = fopen(OUTPUT_FILE,"w");
	double  iCount = 0; 		// counter for total instructions
	double  tage_pred = 0;	// accumulator for total good predictions
	uint64_t tmp_tage_pred;		// temporary variable to be added to accumulator		
	
	//predictor structure declaration
	struct predictor pred_str;
	// First initiating Bimodal Table
   	// Its a simple 2 bit counter table
	pred_str.numBimodalEntries = (1 << BIMODALLOG);
	pred_str.numberTagEntries = (1 << TAGPREDLOG);
	// Initialize ctr to weakly taken
	for(uint32_t ii=0; ii < pred_str.numBimodalEntries; ii++){
		pred_str.bimodal[ii]=BIMODAL_CTR_INIT;
	}
	// Instantiate 4 components with a budget of memory allocated equal to 2^12
	for(uint32_t ii = 0; ii < NUMTAGTABLES ; ii++){
       		pred_str.tagPred[ii] = (struct component *)malloc(sizeof(struct component) * pred_str.numberTagEntries);
    	}
	// Initialize all components
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
	// Initialize primePred and altPred to -1; (-1) means that
	// the prediction wasn't given by nor the provider component neither alternate component
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
	// Clock is used to periodically reset the useful counter as explained in the paper.
      	pred_str.clock = 0;
       	pred_str.clock_flip = 1;
       	pred_str.phr = 0;
       	pred_str.altBetterCount = 8;
	// Initialize ghr
	for(int kk=0; kk<GHRLNGTH; kk++){
		pred_str.ghr[kk]=0;
	}
	printf("METTILA PRIMA DEL WHILE\n");
	// Performing predicitons reading from file
	while(acquire(fp,buffer_read) != -1){
		tmp_tage_pred = get_prediction(buffer_read[0], &pred_str);
		update_predictor(buffer_read[0], buffer_read[1], tmp_tage_pred, buffer_read[2], &pred_str);
		if(tmp_tage_pred == buffer_read[1]){
			tage_pred += 1;		
		}
		iCount ++;
	}
	
	fclose(fp);
	printf("PRIMA DELLA FLOAT\n");
	double MPKBr =((1-tage_pred/iCount)*1000);
  	double accurateTage = (tage_pred * 100 / iCount);
    	fprintf(scr,"%f\n",accurateTage);
	printf("Results with Tage Predictor and TOT#INSTR: %f\n", iCount);
    	printf("Accuracy : %f\n", accurateTage );
    	printf("MPKBr_1K : %f\n", MPKBr);
    	fclose(scr);

	
	// Deallocate all allocated items
	for(uint32_t ii = 0; ii < NUMTAGTABLES ; ii++){
		free(pred_str.tagPred[ii]);
	}
	return 0;
}


