#include "elt.h"

#define MAXIMIER
//#define MINIMIER

typedef struct {
	T_elt elt; 
	T_elt * pElt; 
} T_data;

T_data genData(T_elt e, T_elt *pE);

void echanger(T_elt *p, int i, int j);
int comparer(T_elt a, T_elt b);

void tassifier(T_data d, int n, int root);
void heap_sort(T_data d, int n);