#ifndef _UPDATE_PREDICTOR_H_
#define _UPDATE_PREDICTOR_H_

#include"common_var.h" 

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



void update_predictor(uint64_t PC, uint64_t resolveDir, uint64_t predDir, uint64_t branchTarget, predictor * /*dobbiamo passargli la struct*/ ){

	uint64_t strong_old_present = 0;
	uint64_t new_entry = 0;
	if(prime < NUMTAGTABLES){
		//The useful counter u of the provider counter is updated when the alternate
		//prediction altPred is different from the final predicion pred (in this case is predDir)
		//u is incremented when the actual prediction predDir is correct and decremented otherwise
		//update useful counter
		if(predDir != altPred){
			if(predDir == resolveDir){
				predictor.tagPred[primeBank][indexTagPred[primeBank]].usefulBits = SatIncrement(tagPred[primeBank][indexTagPred[primeBank]].usefulBits, BIMODAL_CTR_MAX);
			}
			else{
				predictor.tagPred[primeBank][indexTagPred[primeBank]].usefulBits = SatDecrement(tagPred[primeBank][indexTagPred[primeBank]].usefulBits);

			}
		}
		//update the predictor that provided the prediction
		if(resolveDir){
			predictor.tagPred[primeBank][indexTagPred[primeBank]].ctr = SatIncrement(tagPred[primeBank][indexTagPred[primeBank]].ctr, TAGPRED_CTR_MAX);
		}
		else{
			predictor.tagPred[primeBank][indexTagPred[primeBank]].ctr = SatDecrement(tagPred[primeBank][indexTagPred[primeBank]].ctr);		
		}
	}
	//da commentare per capire cosa stiamo aggiornando nel caso in cui prime >= NUMTAGTABLES e cosa sono questi sconosciuti....
	
	
	else{
		uint64_t bimodalIndex = PC % numBimodalEntries;
		if(resolveDir){
			bimodal[bimodalIndex] = SatIncrement(bimodal[bimodalIndex], BIMODAL_CTR_MAX);
		{
		else{
			bimodal[bimodalIndex] = SatDecrement(bimodal[bimodalIndex]);
		}
	}
	



}







#endif
