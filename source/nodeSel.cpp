#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
using namespace std;

#define N 4847571
#define NN 5000000
#define M 68993773
#define MM 70000000
FILE *fp = NULL;

struct edge{
	int h;
	int t;
};
struct edge eg[MM];
int eHead[NN], eTail[NN];
int nFlag[NN], nTag[NN]; // nFlag: state of chosen or not; nTag: the new tag of chosen node;
int nCount = 0;
int LIMIT;
void Init(){	
	fp = fopen("../soc-LiveJournal1.txt","r");
	char tmps[10000];
	for (int i = 0; i < 4; i++){
		fgets(tmps, 1000, fp);
	}
	for (int i = 1; i <= N; i++){
		eHead[i] = 0;
		eTail[i] = -1;
	}
	eHead[1] = 1;
	for (int i = 1; i <= M; i++){
		fscanf(fp, "%d %d", &eg[i].h, &eg[i].t);
		//Set the No. range starting from 1...
		eg[i].h += 1; 
		eg[i].t += 1; 
		if (i != 1 && eg[i].h != eg[i-1].h){
			eHead[eg[i].h] = i;
			eTail[eg[i - 1].h] = i - 1;
		}
	}
	eTail[eg[M].h] = M;

	fclose(fp);
}

int nodeSelect(){
	int tp;
	while (1){
		tp = (rand() % N);
		if (nFlag[tp] == 1) continue;
		if (eHead[tp] >= eTail[tp]) continue;
		return (tp);
	}
}

void seedSearch(int x){
	queue<int> q;
	q.push(x);
	while (!q.empty()){
		int w = q.front();
		for (int i = eHead[w]; i <= eTail[w]; i++){
			int t = eg[i].t;
			if (nFlag[t] == 1) continue;
			q.push(t);
			nFlag[t] = 1;
			nCount++;
			if (nCount == LIMIT) return;
			
		}
		q.pop();
	}
}

void Process(){
	srand(time(NULL));
	while (nCount != LIMIT){
		int nSeed = nodeSelect();
		nCount ++;
		nFlag[nSeed] = 1;
		if (nCount == LIMIT) break;
		seedSearch(nSeed);
		if (nCount == LIMIT) break;
	}

	int tCount = 1;
	for (int i = 0; i < N; i++){
		if (nFlag[i] == 0) continue;
		nTag[i] = tCount;
		tCount++;
	}
	int eNUM = 0;
	for (int i = 1; i <= M; i++){
		int x = eg[i].h;
		int y = eg[i].t;
		if (nFlag[x] == 0 || nFlag[y] == 0) continue;
		eNUM ++;
	}
	fp = fopen("./data/smallG.txt","w");
	fprintf(fp, "%d %d\n", LIMIT, eNUM);
	for (int i = 1; i <= M; i++){
		int x = eg[i].h;
		int y = eg[i].t;
		if (nFlag[x] == 0 || nFlag[y] == 0) continue;
		fprintf(fp, "%d %d\n", nTag[x], nTag[y]);
	}
	fclose(fp);
}

int main(int argv, char* argc[]){
	LIMIT = atoi(argc[1]);
	Init();
	Process();
}