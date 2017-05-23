#ifndef _GET_PREDICTION_H_
#define _GET_PREDICTION_H_


#include "common_var.h"


uint64_t get_prediction(uint64_t PC, struct predictor * tage) //dobbiamo passare la classe predictor e il PC
{
//simple bimodal first prediction based on a counter
bool basePrediction;
uint64_t bimodalIndex   = (PC) % (tage->numBimodalEntries);
uint64_t bimodalCounter =tage->bimodal[bimodalIndex]; 

if(bimodalCounter > BIMODAL_CTR_MAX/2){
    basePrediction =  1; 
  }else{
    basePrediction =  0; 
}
// calcolo tag  in base all'algoritmo spiegato nell articolo https://www.jilp.org/vol7/v7paper10.pdf
     // Hash to get tag includes info about bank, pc and global history compressed
     // formula given in PPM paper 
     // pc[9:0] xor CSR1 xor (CSR2 << 1)

 for(int i = 0; i < NUMTAGTABLES; i++)
{ 
     tage->tag[i] = PC ^ tage->tagComp[0][i].compHist ^ (tage->tagComp[1][i].compHist << 1);
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
// funzioni che ti permettono di calcolare l'indirizzo della entry nei vary blocchi
uint64_t index_mask = ((1<<TAGPREDLOG) - 1);

tage->indexTagPred[0] = PC ^ (PC >> TAGPREDLOG) ^ tage->indexComp[0].compHist ^ tage->phr ^ (tage->phr >> TAGPREDLOG);
tage->indexTagPred[1] = PC ^ (PC >> (TAGPREDLOG - 1)) ^ tage->indexComp[1].compHist ^ (tage->phr );
tage->indexTagPred[2] = PC ^ (PC >> (TAGPREDLOG - 2)) ^ tage->indexComp[2].compHist ^ (tage->phr & 31);
tage->indexTagPred[3] = PC ^ (PC >> (TAGPREDLOG - 3)) ^ tage->indexComp[3].compHist ^ (tage->phr & 7);

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
//in primebank metto l indice del bank che ha matchiato con il tag con l'history piuù lunga, mentre in altbank quello con la seconda history più lunga
       for(int iterator = 0; iterator < NUMTAGTABLES; iterator++)
       {
           
           
            if(tage->tagPred[iterator][tage->indexTagPred[iterator]].tag == tage->tag[iterator])
            {
                tage->primeBank = iterator;
                break;
            }  
       }      
            for(int iterator = tage->primeBank + 1; iterator < NUMTAGTABLES; iterator++)
            {
                if(tage->tagPred[iterator][tage->indexTagPred[iterator]].tag == tage->tag[iterator])
                {
                    tage->altBank = iterator;
                    break;
                }  
            }    
            
 //controllo e scelgo quale predizione scegliere tra quelle contenute nelle tabelle      
       
    if(tage->primeBank < NUMTAGTABLES)  //ho trovato almeno un hit
    {        
       if(tage->altBank == NUMTAGTABLES)  //ho trovato solo un hit 
       {
           tage->altPred = basePrediction; //quindi l'altpred è uguale a quella dell bimodal branch predictor
       }
       else        // più di un hit
       {
           if(tage->tagPred[tage->altBank][tage->indexTagPred[tage->altBank]].pred >= TAGPRED_CTR_MAX/2) //controlla la predizione dell altbank 
                tage->altPred = 1; //prima c era scritto taken ma non era definito quindi metto 1
            else 
                tage->altPred = 0;// NOT_TAKEN=0
       }
        
        if((tage->tagPred[tage->primeBank][tage->indexTagPred[tage->primeBank]].pred  != 3) ||(tage->tagPred[tage->primeBank][tage->indexTagPred[tage->primeBank]].pred != 4 ) || (tage->tagPred[tage->primeBank][tage->indexTagPred[tage->primeBank]].u != 0) || (tage->altBetterCount < 8)) //'???????? credo che sia: la prima e la seconda condizione controllo che non sia or weakly taken o weakly not taken ma avere valori più 'certi', poi controlla che l'useful bit non sia zero e l'ultimo boh. in definitiva credo che questo serva per capire se il match è avvenuto con una new entry. se è cosi ( in questo caso else *) come viene spiegato in questo paper https://www.jilp.org/vol8/v8paper1.pdf al punto 3.2.4, è più efficente prendere la predizione del altbank e non quella del bank con la history più lunga
        {
            if(tage->tagPred[tage->primeBank][tage->indexTagPred[tage->primeBank]].pred >= TAGPRED_CTR_MAX/2)
                tage->primePred = 1;
            else 
                tage->primePred = 0;
            return tage->primePred;    //ritorno la predizione con la history piu lunga
        }
        else //*
        {
            return tage->altPred;
        }
    }
    else 
    {
        tage->altPred = basePrediction;  //nessun match
        return tage->altPred;
    }
}


#endif
