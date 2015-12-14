#include "baseAlgo.h"
vector< mapping > sortList[MAX_THREAD];
pthread_t threads[MAX_THREAD];


// Get a maxmatch of the neighbors of x and y.
double maxMatch(int x, int y, int id){
	int use[2][MAX_DGE];
	double sum = 0.0;
	int n1, n2, num, outCnt;
	// Match the forward-edge neighbors
	memset(use, 0, sizeof(use));
	outCnt = 0;
	n1 = edges_G1[x].size();
	n2 = edges_G2[y].size();
	for (int i = 0; i < n1; i++)
		for (int j = 0; j < n2; j++){
			int t1 = edges_G1[x][i];
			int t2 = edges_G2[y][j];
			sortList[id].push_back(mapping(i, j, score[t1][t2]));
		}
	num = sortList[id].size();
	sort(sortList[id].begin(), sortList[id].begin() + num, cmp);
	for (int i = 0; i < num; i++){
		if (n1 == 0 || n2 == 0) break;
		int x = sortList[id][i].f1;
		int y = sortList[id][i].f2;
		if (use[0][x] || use[1][y]) continue;
		outCnt++;
		use[0][x] = use[1][y] = 1;
		n1 --;
		n2 --;
		sum += sortList[id][i].v;
	}

	sortList[id].clear();
	
	// Match the backward-edge neighbors
	memset(use, 0, sizeof(use));
	outCnt = 0;
	n1 = rev_edges_G1[x].size();
	n2 = rev_edges_G2[y].size();
	for (int i = 0; i < n1; i++)
		for (int j = 0; j < n2; j++){
			int t1 = rev_edges_G1[x][i];
			int t2 = rev_edges_G2[y][j];
			sortList[id].push_back(mapping(i, j, score[t1][t2]));
		}
	num = sortList[id].size();
	sort(sortList[id].begin(), sortList[id].begin() + num, cmp);
	for (int i = 0; i < num; i++){
		if (n1 == 0 || n2 == 0) break;
		int x = sortList[id][i].f1;
		int y = sortList[id][i].f2;
		if (use[0][x] || use[1][y]) continue;
		outCnt++;
		use[0][x] = use[1][y] = 1;
		n1 --;
		n2 --;
		sum += sortList[id][i].v;
	}
	sortList[id].clear();
	
	//A naive thought to fix the sim matrix;
	double L1 = degree_G1[x];
	double L2 = degree_G2[y];
	double diff;
	if (L1 > L2)
		diff = L1 / L2;
	else
		diff = L2 / L1;
	diff = log(diff + 1) / log(2);
	//return sum / diff;
	return sum;

}


void* threadTask(void* args){
	int iterNum = iterGlobal + 1;
	int threadNum = (*((int *)args)) + 1;
	int count = 0;
	for (int i = threadNum; i <= n[0]; i += THREAD){
		for (int j = 1; j <= n[1]; j++){
			tempScore[i][j] = maxMatch(i, j, threadNum);
		}
		count ++;
		if ((count % 10) == 0) 
			printf("ROUND %02d, THREAD %02d AT %05d, PROGRESS: %06.02f %%\n", iterNum, threadNum, i, ((double)i*100)/n[0]);		
	}
	return NULL;
}

// Matrix update using multithreading.
void Cal(){
	int index[MAX_THREAD];
	int iterNum = iterGlobal;
	for (int i = 0; i < THREAD; i++){
		index[i]= i;
		pthread_create(&threads[i], NULL, threadTask, (void *)&index[i]);
	}
	for (int i = 0; i < THREAD; i++){
		pthread_join(threads[i], NULL);
	}
	printf("[STAGE 1] UPDATING SIMILARITY MATRIX\n");
	for (int i = 1; i <= n[0]; i++){
		for (int j = 1; j <= n[1]; j++){
			score[i][j] = tempScore[i][j];
		}
	}
	printf("[STAGE 1] MATRIX UPDATED \n");
}

// Normalize the matrix so that it can converge.
void Norm(){
	double max = 0;
	for (int i = 1; i <= n[0]; i++){
		for (int j = 1; j <= n[1]; j++){
			if (max < score[i][j]) max = score[i][j];
		}
	}
	printf("MAX SCORE IN THIS ROUND %0.5f\n", max);
	for (int i = 1; i <= n[0]; i++){
		for (int j = 1; j <= n[1]; j++){
			score[i][j] /= max;
		}
	}
}