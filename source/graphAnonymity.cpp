#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define N 100000
#define M 1000000
int n, m;
int TYPE;
FILE *fp = NULL;
struct edge{
	int h;
	int t;
	int valid;
};
struct edge eg[M];
int nTag[N];
int pnum;
int naive_cnt = 10000;
void init(){
	fp = fopen("./data/graph2.txt","r");
	fscanf(fp, "%d %d", &n, &m);
	for (int i = 1; i <= m; i++){
		fscanf(fp, "%d %d", &eg[i].h, &eg[i].t);
		eg[i].valid = 1;
	}
	for (int i = 1; i <= n; i++){
		nTag[i] = i;
	}
	fclose(fp);
}
void naive(){
	srand(time(NULL));
	int tCount = naive_cnt;
	int x, y;
	int tmp;
	while(tCount--){
		x = rand() % n + 1;
		y = rand() % n + 1;
		if (x == y) continue;
		tmp = nTag[x];
		nTag[x] = nTag[y];
		nTag[y] = tmp;
	}
}
void perturb(){
	//printf("%d\n", pnum);
	srand(time(NULL));
	int cnt = pnum;
	while(cnt--){
		retry:
			int x = rand() % m + 1;
			if (eg[x].valid == 0)
				goto retry;
			//printf("%d %d\n", x, cnt);
			eg[x].valid = 0;
	}

}
void output(){
	fp = fopen("./data/pair.txt","w");
	for (int i = 1; i <= n; i++){
		fprintf(fp, "%d %d\n", nTag[i], i);
	}
	fclose(fp);
	fp = fopen("./data/graphTarget.txt","w");
	fprintf(fp, "%d %d\n", n, m - pnum);
	for (int i = 1; i <= m; i++){
		if (eg[i].valid)
			fprintf(fp, "%d %d\n", nTag[eg[i].h], nTag[eg[i].t]);
	}
	fclose(fp);
}
int main(int argv, char* argc[]){
	TYPE = atoi(argc[1]);
	init();
	//printf("%d\n", TYPE);
	switch (TYPE){
		case 1:
			pnum = (int)(0.1*m);
			perturb();
			break;
		default:
			naive();
	}
	output();
}