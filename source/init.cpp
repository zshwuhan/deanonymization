#include "baseAlgo.h"
vector< vector<int> > edges_G1, rev_edges_G1, edges_G2, rev_edges_G2;
vector<int> degree_G1, degree_G2;

void main_init(){
	int nb_count = 0;
	int r;

	fp = fopen("./data/graph1.txt","r");
	r = fscanf(fp, "%d %d", &n[0], &m[0]);

	edges_G1.resize(n[0]+1);
	rev_edges_G1.resize(n[0]+1);
	degree_G1.resize(n[0]+1);

	for (int i = 1; i <= m[0]; i++){
		int x, y;
		r = fscanf(fp,"%d %d", &x, &y);
		if (x != y) {
			edges_G1[x].push_back(y);
			rev_edges_G1[y].push_back(x);
		}
	}
	fclose(fp);
	for (int i = 1; i <= n[0]; i++)
		degree_G1[i] = edges_G1[i].size() + rev_edges_G1[i].size();

	for (int i = 1; i <= n[0]; i++)
		if (degree_G1[i] > nb_count) 
			nb_count = degree_G1[i];

	fp = fopen("./data/graphTarget.txt","r");
	r = fscanf(fp, "%d %d", &n[1], &m[1]);

	edges_G2.resize(n[1]+1);
	rev_edges_G2.resize(n[1]+1);
	degree_G2.resize(n[1]+1);

	for (int i = 1; i <= m[1]; i++){
		int x, y;
		r = fscanf(fp,"%d %d", &x, &y);
		if (x != y) {
			edges_G2[x].push_back(y);
			rev_edges_G2[y].push_back(x);
		}
	}
	fclose(fp);
	for (int i = 1; i <= n[1]; i++)
		degree_G2[i] = edges_G2[i].size() + rev_edges_G2[i].size();

	for (int i = 1; i <= n[1]; i++)
		if (degree_G2[i] > nb_count) 
			nb_count = degree_G2[i];
	printf("MAXNEIGHBOR=%d\n",nb_count);

	score.resize(n[0]+1);
	tempScore.resize(n[0]+1);
	for (int i = 1; i <= n[0]; i++){
		score[i].resize(n[1]+1);
		tempScore[i].resize(n[1]+1);
		for (int j = 1; j <= n[1]; j++)
			score[i][j] = 1.0;
	}

	fp = fopen("./data/pair.txt","r");
	for (int i = 1; i <= n[0]; i++){
		int x, y;
		r = fscanf(fp,"%d %d", &x, &y);
		corNode[x] = y;
	}
	fclose(fp);
}



int cmp(const mapping &a, const mapping &b){
	if (a.v > b.v) 
		return 1;
	else
		return 0;
}

int getMax(int x, int y){
	if (x > y) return x;
	return y;
}

