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
#define NUMTAGTABLES 4

//defines in cc file
#define BIMODAL_CTR_MAX  3
#define BIMODAL_CTR_INIT 2
#define TAGPRED_CTR_MAX  7
#define TAGPRED_CTR_INIT 0
#define BIMODALLOG   14
#define NUMTAGTABLES 4
#define TAGPREDLOG 12

//see figure 1 of a 256Kbits L-Tage branch predictor

struct component{
	int32_t pred;  //prediction
	uint32_t tag;  //tag
	int32_t u;     //useful bit
};

typedef struct component * Ts;

//what in the paper is called h[0:L(i)] to store
//the history to be passed to components

struct CompressedHist{
	uint64_t geomLength;
	uint64_t targetLength;
	uint64_t compHist;
};


struct predictor{

	uint64_t ghr; //global history table
	uint64_t pht; //pattern history table (local)
	uint32_t bimodal[1 << BIMODALLOG]; //num pht bimodalentries
	uint32_t historyLength;
	uint32_t num_pht_bimodal_entries;
	struct component * components[NUMTAGTABLES];
	uint32_t number_tag_entries;// Next to initiating the taggedPredictors
	uint32_t geometric[NUMTAGTABLES];
		


};