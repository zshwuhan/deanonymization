#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include <string>
using namespace std;
#define N 10005
#define M 1000000
FILE *fp = NULL;
int LIMIT, n, m;

struct edge{
	int h;
	int t;
};
struct edge eg[M];
int eHead[N], eTail[N];
int nFlag[N], nTag[N];
int nCount = 0;
int n1, n2;

void Init(){
	fp = fopen("./data/smallG.txt","r");
	fscanf(fp, "%d %d", &n, &m);
	for (int i = 1; i <= n; i++){
		eHead[i] = 0;
		eTail[i] = -1;
	}
	eHead[1] = 1;
	for (int i = 1; i <= m; i++){
		fscanf(fp, "%d %d", &eg[i].h, &eg[i].t);
		if (i != 1 && eg[i].h != eg[i - 1].h){
			eHead[eg[i].h] = i;
			eTail[eg[i - 1].h] = i - 1;
		}
	}
	eTail[eg[M].h] = m;
	fclose(fp);
}
int nodeSelect(){
	int tp;
	while (1){
		tp = (rand() % n);
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
			if (eHead[w] == 0) continue;
			int t = eg[i].t;
			if (nFlag[t] == 1) continue;
			q.push(t);
			nFlag[t] = 1;
			nCount++;
			if (t == 0) printf("%d %d %d\n", w, t, nCount);
			nTag[t] = nCount;
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
		nTag[nSeed] = nCount;
		nFlag[nSeed] = 1;
		if (nCount == LIMIT) break;
		seedSearch(nSeed);
		if (nCount == LIMIT) break;

	}
	int tCount1 = n1-LIMIT;
	int tCount2 = n2-LIMIT;
	for (int i = 1; i <= n; i++){
		if (nFlag[i] == 1) continue;
		if (tCount1 == 0){
			nFlag[i] = 3;
			nTag[i] = tCount2 + LIMIT;
			tCount2 --;
			continue;
		}
		if (tCount2 == 0){
			nFlag[i] = 2;
			nTag[i] = tCount1 + LIMIT;
			tCount1 --;
			continue;
		}
		int temp = rand() % 2;
		if (temp){
			nFlag[i] = 2;
			nTag[i] = tCount1 + LIMIT;
			tCount1 --;
		} else{
			nFlag[i] = 3;
			nTag[i] = tCount2 + LIMIT;
			tCount2 --;
		}
	}
	int m1 = 0, m2 = 0;
	for (int i = 1; i <= m; i++){
		int del1 = 1, del2 = 1;
		if (nFlag[eg[i].h] == 2 || nFlag[eg[i].t] == 2) del2 =0;
		if (nFlag[eg[i].h] == 3 || nFlag[eg[i].t] == 3) del1 =0;
		m1 += del1;
		m2 += del2;
	}
	FILE *fw1 = fopen("./data/graph1.txt","w");
	FILE *fw2 = fopen("./data/graph2.txt","w");
	fprintf(fw1,"%d %d\n", n1, m1);
	fprintf(fw2,"%d %d\n", n2, m2);
	for (int i = 1; i <= m; i++){
		int del1 = 1, del2 = 1;
		if (nFlag[eg[i].h] == 2 || nFlag[eg[i].t] == 2) del2 = 0;
		if (nFlag[eg[i].h] == 3 || nFlag[eg[i].t] == 3) del1 = 0;
		if (del1 == 1){
			fprintf(fw1,"%d %d\n", nTag[eg[i].h], nTag[eg[i].t]);
		}
		if (del2 == 1){
			fprintf(fw2,"%d %d\n", nTag[eg[i].h], nTag[eg[i].t]);
		}
	}	

}
int main(int argv, char* argc[]){
	Init();

	LIMIT = atoi(argc[1]);
	n1 = (n - LIMIT) / 2 + LIMIT;
	n2 = n - n1 + LIMIT;
	Process();

}