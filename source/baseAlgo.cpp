#include "baseAlgo.h"

int n[2],m[2];
FILE *fp = NULL;
int OVERLAP, THREAD, ITERFLAG, ALGO;
//float score[N][N], tempScore[N][N];
std::vector< std::vector<float> > score, tempScore;
int corNode[N];
int iterGlobal;
int node_match[N];
int rev_node_match[N];
int correctCounter;
int numCounter;
float mapNum;

////////////////////////////////////////////////////////////////

void main_routine(){
	// Basic iterations
	if (ITERFLAG == 1){
		for (int iter = 0; iter < ITER_NUM; iter++){
			iterGlobal = iter;
			Cal();
			printf("[INFO] ROUND %02d: CALCULATION DONE\n", iter+1);
			Norm();
			printf("[INFO] ROUND %02d: NORMALIZATION DONE\n", iter+1);
		}
		matrix_dump();
	} else
		matrix_retrieve();
	
	
	//Compute the matrix of same node pair;
	node_info();

	//The greedy approach of matrix mapping
	matrix_mapping(ITER_NUM, 0);

	//Using the mapped pairs to induce other mapping
	if (ALGO != 0)
		mapping_induction();
	
}

int main(int argv, char* argc[]){
	main_init();
	OVERLAP = atoi(argc[1]);
	THREAD = atoi(argc[2]);
	ITERFLAG = atoi(argc[3]);
	ALGO = atoi(argc[4]);
	printf("Version: %d\n", ALGO);
	printf("Init done...\n");
	main_routine();
}
