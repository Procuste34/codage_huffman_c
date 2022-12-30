#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "include/traces.h"
#include "include/check.h"

#define MAXCARS 128
#define MAX_STRING_LENGTH 500

#define HEAP_ALLOCATION_OFFSET 5

#define iPARENT(i) 			(i-1)/2
#define iLCHILD(i) 			(2*i)+1
#define iRCHILD(i) 			(2*i)+2
#define iLASTINTERNAL(n)	n/2 -1
#define isINTERNAL(i,n) 	(2*i<(n-1))
#define isLEAF(i,n) 			(2*i>=(n-1))
#define isINTREE(i,n)		(i<n)
#define isROOT(i)				(i==0)
#define nbINTERNALS(n) 		n/2
#define nbLEAVES(n) 			ceil((double)n/2)
#define VALP(pHeap, i)		pHeap->tree[i]		
#define VAL(heap, i)			heap.tree[i]

#define VALP_ih(pHeap, i)		pHeap->data[pHeap->tree[i]]
#define VAL_ih(heap, i)			heap.data[heap.tree[i]]

// ****************** fonctions annexes ****************************************************** //

void reverse_string(char* str);
int comparer(int a, int b);

// ****************** type et fonctions de manipulation de tas (indirect) ******************** //

typedef struct {
	unsigned int nbElt;
	unsigned char * tree;
	int * data;
} T_indirectHeap;

T_indirectHeap * newHeap();
void buildHeap(T_indirectHeap *p);
void siftDown(T_indirectHeap *p, int k);
void siftUp(T_indirectHeap *p, int k);
void swap(T_indirectHeap *p, int i, int j);
void showHeap(T_indirectHeap *p);
void showHeap_rec(T_indirectHeap *p, int root, int indent);
unsigned char removeMax(T_indirectHeap *p);

// ****************** fonctions pour le codage de Huffman ********************************* //

T_indirectHeap * creer_tas(char *str, int *nb_car_uniques);
void construit_arbre_codage(int *huffmanTree, T_indirectHeap *ih, int nb_car_uniques);
void calculer_codes(int *huffmanTree, char codes[][MAXCARS]);

// **************** fonctions pour gérer les entetes du codage de Huffman (pb3) ********** //

typedef struct{
    char *parcours_prefixe;
    char *caracteres;
} T_entete;

T_entete * init_entete();

void huffmanTree_to_entete(int *huffmanTree, int nb_car_uniques, T_entete *entete);
void parcours_rec(int *tree, int root, T_entete *entete);

void entete_to_huffmanTree(int *huffmanTree, T_entete *entete); //a completer
void parcours2(int *tree, char *parcours_prefixe, char *caracteres);
void parcours2_rec(int *tree, char *parcours_prefixe, char *caracteres, int appel_fg, int *j, int *i, int *compteur_car, int pere);