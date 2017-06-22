#ifndef _UPDATE_PREDICTOR_H_
#define _UPDATE_PREDICTOR_H_

#include "common_var.h"
 

//used to increment or decrement usefulbits

uint64_t SatIncrement(uint64_t usefulBits, uint64_t max_val){
	if(usefulBits >= max_val){
		return max_val;
	}
	else{
		return usefulBits + 1;	
	}		
}

uint64_t SatDecrement(uint64_t usefulBits){
	if(usefulBits == 0){
		return 0;
	}
	else{
		return usefulBits - 1;	
	}		
}



void update_predictor(uint64_t PC, uint64_t resolveDir, uint64_t predDir, uint64_t branchTarget, struct predictor * predict){
	uint64_t strong_old_present = 0;
	uint64_t new_entry = 0;

	//primeBank set to 4; if 4 no match found in the 4 components (0-3) 4 compontents (4) no match  
	if(predict->primeBank < NUMTAGTABLES){
		//The useful counter u of the provider counter is updated when the alternate
		//prediction altPred is different from the final predicion pred (in this case is predDir)
		//u is incremented when the actual prediction predDir is correct and decremented otherwise
		//update useful counter
		if(predDir != predict->altPred){ //see if the actual prediction is equal to the one contained in the altPred
			if(predDir == resolveDir){ //if preDir == real result taken increment useful counter
				predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].u = SatIncrement(predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].u, BIMODAL_CTR_MAX);
			}
			else{ //otherwise decrement the useful counter
				predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].u = SatDecrement(predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].u);

			}
		}
		//update the predictor that provided the prediction
		if(resolveDir){ //if taken increment 3bit counter that gives the prediction in the component with longest history 
			predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].pred = SatIncrement(predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].pred, TAGPRED_CTR_MAX);
		}
		else{//not taken decrement it.
			predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].pred = SatDecrement(predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].pred);		
		}
	}
	//No match primebank=4 
	//Update the base predictor that here is called bimodal
	else{
		uint64_t bimodalIndex = PC % predict->numBimodalEntries;
		if(resolveDir){
			predict->bimodal[bimodalIndex] = SatIncrement(predict->bimodal[bimodalIndex], BIMODAL_CTR_MAX);
		}
		else{
			predict->bimodal[bimodalIndex] = SatDecrement(predict->bimodal[bimodalIndex]);
		}
	}

	//This is the check done to know wheter the current entry that gave the prediction is a new allocation or not
	//if a match is found between 4 components
	if (predict->primeBank < NUMTAGTABLES){
	//if the useful counter is zero and the primeBank provide a weakly taken or weakly not taken that means that the prediction is given by
	//altPred	    
		if((predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].u == 0) &&((predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].pred  == 3) || (predict->tagPred[predict->primeBank][predict->indexTagPred[predict->primeBank]].pred  == 4))){			//if prediction of primeBank is different by prediction given by altPred
                	new_entry = 1;
			if(predict->primePred != predict->altPred){
				//if the prediciton of altPred is equal to the real one
		   		if(predict->altPred == resolveDir){  //increment the alt better count 
					if(predict->altBetterCount < 15){  
                            			predict->altBetterCount++;
                        		}    
		      		}
		    		else if(predict->altBetterCount > 0){
                        		predict->altBetterCount--;
                    		}
                	}
	    	}
	}


        // If primeBank gave the prediction or the primeBank is new and gave a wrong prediction 
	//see on the paper at: THE OVERALL PREDICTION IS INCORRECT
        if((!new_entry) || (new_entry && (predict->primePred != resolveDir))){    
        	//IF predicted outcome is different from the real and the provider component is not the one with the longest history 
		//allocate a new entry if there exist some component k with longer history which useful counter is 0
		if(((predDir != resolveDir) & (predict->primeBank > 0))){                   	
	        	for (int i = 0; i < predict->primeBank; i++){
		        	if (predict->tagPred[i][predict->indexTagPred[i]].u == 0);
                	        strong_old_present = 1;
	       		}
		       	// If no new entry allocated decrement the counter of all components
		       	if (strong_old_present == 0){
				for (int i = predict->primeBank - 1; i >= 0; i--){
					predict->tagPred[i][predict->indexTagPred[i]].u--;
		                }
		        }// If a new entry can be allocated avoid ping-phenomenon and initialize the allocated entry
		        else{
		        	srand(time(NULL));
		        	int randNo = rand() % 100;
		        	int count = 0;
		        	int bank_store[NUMTAGTABLES - 1] = {-1, -1, -1};
		        	int matchBank = 0;
		        	for (int i = 0; i < predict->primeBank; i++){
		                	if (predict->tagPred[i][predict->indexTagPred[i]].u == 0){
		                		count++;
		           		     	bank_store[i] = i;
		          	        }
		        	}  
		        	if(count == 1){
		            		matchBank = bank_store[0];
		        	}
		        	else if(count > 1){
		             		if(randNo > 33 && randNo <= 99){
		                		matchBank = bank_store[(count-1)];
		            		}
		            		else{
		               			matchBank = bank_store[(count-2)];
		            		}	   
		        	}
				for (int i = matchBank; i > -1; i--){
					if ((predict->tagPred[i][predict->indexTagPred[i]].u == 0)){
				        	if(resolveDir){    
				            		predict->tagPred[i][predict->indexTagPred[i]].pred = 4;
				        	}
				       	        else{
				       	 		predict->tagPred[i][predict->indexTagPred[i]].pred = 3;
				        	}    
				                predict->tagPred[i][predict->indexTagPred[i]].tag = predict->tag[i];
				            	predict->tagPred[i][predict->indexTagPred[i]].u = 0;
						break; // Only 1 entry allocated
				     	}
			       }	
			}
		}
	}
	
    	// Periodic Useful bit Reset Logic ( Important so as to optimize compared to PPM paper)
	predict->clock++;
        //for every 256 K instruction 1st MSB than LSB
	if(predict->clock == (256*1024)){
        	// reset clock
       		predict->clock = 0;
            	if(predict->clock_flip == 1){
                	// this is the 1st time
               		predict->clock_flip = 0;
            	}
            	else{
                	predict->clock_flip = 1;
            	}
	    	if(predict->clock_flip == 1){// MSB turn
                	for (int jj = 0; jj < NUMTAGTABLES; jj++){    
	            		for (uint32_t ii = 0; ii < predict->numberTagEntries; ii++){
	                		predict->tagPred[jj][ii].u = predict->tagPred[jj][ii].u & 1;
	            		}
                	}
            	}    
            	else{// LSB turn
                	for (int jj = 0; jj < NUMTAGTABLES; jj++){    
                       		for (uint32_t ii = 0; ii < predict->numberTagEntries; ii++){
                           		predict->tagPred[jj][ii].u = predict->tagPred[jj][ii].u & 2;
                       		}
                   	}
		}

	}
	
  	// update the GHR
	
	for(int kk=0; kk<GHRLNGTH; kk++){
		predict->ghr[kk+1]=predict->ghr[kk];
  		//predict->ghr = (predict->ghr << 1);
	}
	predict->ghr[0]=0;
  	if(resolveDir == 1 ){// 1=TAKEN
    		predict->ghr[0]=1; 
  	}
	for (uint64_t i = 0; i < NUMTAGTABLES; i++){                
        	compute_history(predict,i,NUMTAGTABLES+1); //if 3d parameter is <0 uses indexComp instead of tagComp
		compute_history(predict,0,i);
		compute_history(predict,1,i);            	
		//indexComp[i].updateCompHist(predictor->ghr);		
		//predictor->tagComp[0][i].updateCompHist(predictor->ghr);
            	//predictor->tagComp[1][i].updateCompHist(predictor->ghr);
        }
  	// PHR update is simple, jus take the last bit ??
    	// Always Limited to 16 bits as per paper.
    	predict-> phr = (predict-> phr << 1); 
    	if(PC & 1){
        	predict-> phr = predict-> phr + 1;
    	}
    	predict-> phr = (predict-> phr & ((1 << 16) - 1));
}
#endif
