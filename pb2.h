#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "include/traces.h"
#include "include/check.h"

#define MAXCARS 128
#define MAX_STRING_LENGTH 500

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

void reverse_string(char *str);
int comparer(int a, int b);

// ****************** type et fonctions de manipulation de tas (indirect) ******************** //

typedef struct {
	unsigned int nbElt;
	unsigned char *tree;
	int *data;
} T_indirectHeap;

T_indirectHeap * newHeap();
void buildHeap(T_indirectHeap *p);
void siftDown(T_indirectHeap *p, int k);
void siftUp(T_indirectHeap *p, int k);
void swap(T_indirectHeap *p, int i, int j);
void showHeap(T_indirectHeap *p);
void showHeap_rec(T_indirectHeap *p, int root, int indent);
unsigned char removeMax(T_indirectHeap *p);

// ****************** fonctions pour générer les fichiers de visualisation ***************** //

void genere_minimier_viz(unsigned char *tree, int nbElt, char *nom_fichier);
void genere_minimier_viz_rec(unsigned char *tree, int nbElt, int root, char *nom_fichier);

void genere_arbre_codage_viz(int *tree, int root, char *nom_fichier);
void genere_arbre_codage_viz_rec(int *tree, int root, char *nom_fichier);

void genere_fichier_viz(T_indirectHeap *p, int *tree, int top_noeud_tree, int etape);

// ****************** fonctions pour le codage de Huffman ********************************* //

T_indirectHeap * creer_tas(char *str, int *nb_car_uniques);
void construit_arbre_codage(int *huffmanTree, T_indirectHeap *ih, int nb_car_uniques);
void calculer_codes(int *huffmanTree, T_indirectHeap *ih, int *occurences, int *longueurs, char codes[][MAXCARS]);