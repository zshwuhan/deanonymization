
#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <pthread.h>
#include <algorithm>
#include <memory.h>
#include <math.h>
#include <map>
#define N 11000
#define M 250000
#define ITER_NUM 5
#define MAX_DGE 2000
#define MAX_THREAD 10
using namespace std;

struct mapping{
  int f1, f2;
  double v;
  
  friend bool operator < (mapping n1, mapping n2){
    return n1.v < n2.v;
  }

  mapping(){
  }    

  mapping(int x, int y, double z){
  	f1 = x;
  	f2 = y;
  	v = z;
  }

  mapping& operator= (const mapping& s){
  	f1 = s.f1;
  	f2 = s.f2;
  	v = s.v;
  	return *this;
  }
};

extern int n[2],m[2];
extern FILE *fp;
extern int OVERLAP, THREAD, ITERFLAG;
extern int ITER_ALGO, PICK_ALGO;
extern int iterGlobal;
extern vector< vector<int> > edges_G1, rev_edges_G1, edges_G2, rev_edges_G2;
extern vector< vector<double> > score, tempScore;
extern vector<int> degree_G1, degree_G2;
extern int corNode[N], node_match[N], rev_node_match[N];
extern int numCounter, correctCounter;
extern double mapNum;

extern void matrix_dump();
extern void matrix_retrieve();
extern int cmp(const mapping &a, const mapping &b);
extern void main_init();
//extern int getMax(int x, int y);
extern void Norm();
extern void Cal();
//extern void* threadTask(void* args);
//extern double maxMatch(int x, int y, int id);
extern void matrix_mapping(int iter, int flag);
extern void node_info();
extern void mapping_induction();

#endif
