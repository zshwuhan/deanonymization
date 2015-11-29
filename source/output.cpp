#include "baseAlgo.h"
int ndiff = 0, total_diff = 0;
std::vector< std::vector<float> > weight;
std::vector< mapping > temp;

// Get the complete max match between the two graphs. 
// Note that if the flag is set to 1, the following code will use a boost trick.
void matrix_mapping(int iter, int flag){
	//priority_queue<mapping> HEAP;
	std::vector< mapping > HEAP;

	int reset_tag = 0;
	int use[2][N];
	FILE *fp = NULL;

	mapNum = 0.0;
	correctCounter = 0;
  numCounter = 0;

	// Push the similarity matrix to the heap	
	printf("[STAGE 2] PUSHING THE HEAP\n");
	for (int i = 1; i <= n[0]; i++)
		for (int j = 1; j <= n[1]; j++)
			HEAP.push_back(mapping(i, j, score[i][j]));			
	
	sort(HEAP.begin(), HEAP.begin()+HEAP.size(), cmp);

	printf("[STAGE 2] HEAP PUSHING DONE\n");
	
	for (int i = 1; i <= n[0]; i++){
		use[0][i] = 0;
		use[1][i] = 0;
	}

	fp = fopen("./result/mappingIter.txt", "w");

	// Output the mapping base on the similarity score in reverse order.
	printf("[STAGE 2] MAPPING ACROSS GRAPHS\n");
	for (int i = 0; i < HEAP.size(); i++){
 		mapping t0 = HEAP[i];
 		 		
 		// If one node of this pair has been used then pass.
		if (use[0][t0.f1] == 1 || use[1][t0.f2] == 1 )
			continue;

 		// Mark this pair as used.
		use[0][t0.f1] = 1;
		use[1][t0.f2] = 1;

		node_match[t0.f1] = t0.f2;
		rev_node_match[t0.f2] = t0.f1;

		// In our case we consider it a correct mapping
		if (corNode[t0.f2] == t0.f1 && t0.f1 <= OVERLAP){
			mapNum += 1;
			correctCounter++;
		} else {
			total_diff++;
			if (tempScore[t0.f1][corNode[t0.f2]]) ndiff++;
		}

		// Increase the total output mapping number.
		numCounter++;
	
		// Output the mapping info for debugging.
		fprintf(fp, "G1: %05d GA: %04d G2: %04d D: %04d P: %08.4f Total: %08.4f%%\n", 
			t0.f1, t0.f2, corNode[t0.f2], 
			degree_G1[t0.f1], t0.v, mapNum*100/numCounter); 

    // If the degree of the node is too small...skip

	 	if (ALGO != 0)
	 		if (numCounter >= (0.05 * n[0]))
	 			break;
	 		/*
	 		if ((degree_G1[t0.f1] <= 10) || (degree_G2[t0.f2] <= 10)){
	 			temp.push_back(mapping(t0.f1, t0.f2, 0));
	 			if (corNode[t0.f2] == t0.f1 && t0.f1 <= OVERLAP) 
	 				reset_tag++;
	 		}
	 		*/
	 		 		

	}
	/*
	if (ALGO != 0){
		correctCounter -= reset_tag;
		mapNum -= reset_tag;
		numCounter -= temp.size();
		for (int i = 0; i < temp.size(); i++){
			use[0][temp[i].f1] = 0;
			use[1][temp[i].f2] = 0;
			node_match[temp[i].f1] = 0;
			rev_node_match[temp[i].f2] = 0;
		}
	}
	*/
	
	fprintf(fp,"Correct Mapping Number In This Round Is %04d\n",correctCounter);
	printf("[STAGE 2] COREECT MAPPING NUMBER: %04d\n",correctCounter);
	fclose(fp);

	printf("Among all wrong mappings %04d/%04d=%05.2f%% are actually the same.\n", 
		ndiff, total_diff, (float)ndiff*100.0/(float)total_diff);
		
}
float MAX(float x, float y){
	if (x < y) 
		return y;
	else
		return x;
}

void mapping_induction(){
	std::vector< mapping > HEAP;

	int REFINE_ITER = 0;
	int REFINE_TIME = 0;
	int CONST = 10;
	FILE *fp = fopen("./result/induction.txt","w");
	float MINNUM = 1e20;

	printf("Entering mapping_induction\n");

	weight.resize(n[0]+1);
	for (int i = 1; i <= n[0]; i++)
		weight[i].resize(n[1]+1);

	for (int i = 1; i <= n[0]; i++)
		for (int j = 1; j <= n[1]; j++)
			if ((MINNUM > score[i][j]) && (score[i][j] > 0))
				MINNUM = score[i][j];

refine:	
	
	HEAP.clear();

	for (int i = 1; i <= n[0]; i++)
		for (int j = 1; j <= n[1]; j++)
			weight[i][j] = 0;
		
	for (int i = 1; i <= n[0]; i++){
		if (node_match[i]) continue;
		int node = i;

		// node->neb && n0->node_match[neb]
		for (int j = 0; j < edges_G1[node].size(); j++){
			int neb = edges_G1[node][j];
			if (node_match[neb] > 0){
				for (int k = 0; k < rev_edges_G2[node_match[neb]].size(); k++){
					int n0 = rev_edges_G2[node_match[neb]][k];
					if (rev_node_match[n0] > 0) continue;
					if (ALGO == 2)
						weight[node][n0] += MAX(score[neb][node_match[neb]], MINNUM);
					else
						weight[node][n0] += score[neb][node_match[neb]];
				}
			}
		}

		// neb->node && node_match[neb]->n0
		for (int j = 0; j < rev_edges_G1[node].size(); j++){
			int neb = rev_edges_G1[node][j];
			if (node_match[neb] > 0){
				for (int k = 0; k < edges_G2[node_match[neb]].size(); k++){
					int n0 = edges_G2[node_match[neb]][k];
					if (rev_node_match[n0] > 0) continue;
					if (ALGO == 2)
						weight[node][n0] += MAX(score[neb][node_match[neb]], MINNUM);
					else
						weight[node][n0] += score[neb][node_match[neb]];
				}
			}
		}


	}

	for (int i = 1; i <= n[0]; i++)
		for (int j = 1; j <= n[1]; j++){
			if (node_match[i] > 0 || rev_node_match[j] > 0) continue;
			if (weight[i][j] == 0) continue; 
			HEAP.push_back(mapping(i, j, weight[i][j]));
		}
	
	sort(HEAP.begin(), HEAP.begin()+HEAP.size(), cmp);

	printf("HEAP size: %lu\n", HEAP.size());

	for (int i = 0; i < HEAP.size(); i++){
		mapping topMapping = HEAP[i];
		
		if (node_match[topMapping.f1] > 0 || rev_node_match[topMapping.f2] > 0) 
			continue;

		if (ALGO == 2)
			REFINE_ITER++;
			if ((REFINE_ITER >= CONST)) {
				REFINE_ITER = 0;
				REFINE_TIME ++;
				printf("REFINE_ITER: %d\n", REFINE_TIME);
				goto refine;
			}

		node_match[topMapping.f1] = topMapping.f2;
		rev_node_match[topMapping.f2] = topMapping.f1;

		// Check whether we get the correct mapping
 		if (corNode[topMapping.f2] == topMapping.f1 && topMapping.f1<=OVERLAP){
 			mapNum++;
 			correctCounter++;
 		} else{
 			total_diff++;
 			if (tempScore[topMapping.f1][corNode[topMapping.f2]]) ndiff++; 			
 		}

 		// Increase the total output mapping number.
 		numCounter++;

		fprintf(fp, "G1: %04d G2: %04d D: %04d M: %04d P: %04.2f Total: %08.4f%%\n", 
			topMapping.f1, corNode[topMapping.f2], 
			degree_G1[topMapping.f1], 
			topMapping.f2, topMapping.v, mapNum*100/numCounter); 

	}
	fclose(fp);

	printf("Among all wrong mappings %04d/%04d=%06.2f%% are actually the same.\n", 
		ndiff, total_diff, (float)ndiff*100.0/(float)total_diff);

}

int node_cmp(int x, int y){
	if (edges_G1[x].size() != edges_G1[y].size()) 
		return 0;
	if (rev_edges_G1[x].size() != rev_edges_G1[y].size())
		return 0;
	for (int i = 0; i < edges_G1[x].size(); i++)
		if (edges_G1[x][i] != edges_G1[y][i])
			return 0;
	for (int i = 0; i < rev_edges_G1[x].size(); i++)
		if (rev_edges_G1[x][i] != rev_edges_G1[y][i])
			return 0;
	return 1;
}

void node_info(){

	for (int i = 1; i <= n[0]; i++)
		for (int j = 1; j <= n[0]; j++)
			tempScore[i][j] = 0;

	for (int i = 1; i < n[0]; i++)
		for (int j = i + 1; j <= n[0]; j++)
			if (node_cmp(i, j))
				tempScore[i][j] = tempScore[j][i] = 1;
		
}

void matrix_dump(){

	fp = fopen("./result/temp_matrix.txt","w");

	for (int i = 1; i <= n[0]; i++)
		for (int j = 1; j <= n[1]; j++)
			fprintf(fp, "%f ", score[i][j]);

	fclose(fp);
}

void matrix_retrieve(){
	int r;

	fp = fopen("./result/temp_matrix.txt","r");

	for (int i = 1; i <= n[0]; i++)
		for (int j = 1; j <= n[1]; j++)
			r = fscanf(fp, "%f ", &score[i][j]);

	fclose(fp);
	printf("%d %d\n", n[0], n[1]);
}
