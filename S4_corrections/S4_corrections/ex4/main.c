#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h> 
// ceil, floor : #include <math.h>

#include "../include/traces.h" 
#include "../include/check.h" 

#ifndef _ELT_H_
	#define _ELT_H_ // prévient les includes multiples

	/////////////////////////////////////////////////////////////////////////
	// CHOISIR ICI CE QUE REPRESENTE LE TYPE T_elt 
	//#define ELT_CHAR		// un caractère ?
	#define ELT_INT		// un entier (choix par défaut) ?
	//#define ELT_STRING	// une chaîne de caractères ?
	/////////////////////////////////////////////////////////////////////////

	//https://stackoverflow.com/questions/2998864/how-to-add-a-or-condition-in-ifdef

	#if ! defined(ELT_CHAR) && ! defined(ELT_INT) && ! defined(ELT_STRING)
	#define ELT_INT
	#endif

	#ifdef ELT_CHAR
	typedef char T_elt;
	#endif

	#ifdef ELT_INT
	typedef int T_elt; 
	#endif

	#ifdef ELT_STRING
	typedef char * T_elt; 
	#endif

	// valable pour tous les types de T_elt 
	char * toString(T_elt e); 
	T_elt genElt(void); 
	int eltCmp(T_elt e1, T_elt e2);
#endif 

#ifdef ELT_CHAR
// T_elt est un char... 
char * toString(T_elt e) {
	// Il faut transformer un char en chaine... 
	static char buffer[2]; 
	buffer[0] = e; 
	buffer[1] = '\0';
	return buffer; 
	// Si on allouait de la mémoire pour buffer, il faudrait penser à la libérer... 
	// => Risque de fuite de mémoire... 
	// On ne peut pas non plus allouer un tableau static sur la pile ! 
	// => On utilise un buffer déclaré comme variable statique !
	// Dans ce cas, deux appels à toString renverraient la même adresse...
}

T_elt genElt(void) {
	static int indice = 0; 
	// on va égrainer les caractères alphabétiques de 'a' à 'z'
	// de manière circulaire pour ne pas déborder... 
	return 'a' + (indice++%26); 
}

int eltCmp(T_elt e1, T_elt e2) {
	return e1-e2;
}
#endif

#ifdef ELT_INT
// T_elt est un int... 
char * toString(T_elt e) {
	// Il faut transformer un int en chaine... 
	static char buffer[11]; // nbr max de chiffres nécessaires ? 
	// 2^31 vaut 2147483648...
	sprintf(buffer, "%d",e); 
	return buffer; 
	// cf. remarques précédentes
}

T_elt genElt(void) {
	static int indice = 0; 
	return indice++; 
}

int eltCmp(T_elt e1, T_elt e2) {
	return e1-e2;
}
#endif

#ifdef ELT_STRING
// T_elt est un char * ...
char * toString(T_elt e) {
	return e; // c'est déjà une chaine ! 
}

T_elt genElt(void) {
	static int indice = 0; 
	// produire une chaine aléatoire... 
	// On va écrire elt_<numéro>
	char buffer[15]; // elt_+<11 chiffres>
	sprintf(buffer, "elt_%d",indice++); 
	return strdup(buffer); 
} 

int eltCmp(T_elt e1, T_elt e2) {
	return strcmp(e1,e2);
}
#endif


#define HEAP_ALLOCATION_OFFSET 5

typedef struct {
	unsigned int nbElt;
	unsigned int nbMaxElt;
	T_elt * tree;	
} T_heap;

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

T_heap * newHeap(unsigned int nbMaxElt);
void freeHeap(T_heap *p); 
T_heap * initHeap(T_elt t[], int n) ;
void showHeap(T_heap *p);
void showHeap_rec(T_heap *p, int root, int indent); 

void swap(T_heap *p, int i, int j);
void siftUp(T_heap *p, int k);
void addElt(T_heap *p, T_elt e);
void buildHeapV1(T_heap * p);

void siftDown(T_heap *p, int k);
T_elt getMax(const T_heap *p);
T_elt removeMax(T_heap *p);
void buildHeapV2(T_heap * p); 

void heapSortV1(T_heap *p);
void heapSortV2(T_heap *p);


#define MAXCARS 128

typedef struct {
	unsigned int nbElt;
	unsigned char * tree; //avant : unsigned char // change... a voir
	int * data;
} T_indirectHeap;


int comparer(int a, int b);
T_indirectHeap * newHeapV3(unsigned int nbMaxElt);
void buildHeapV3(T_indirectHeap * p);
void siftDownV3(T_indirectHeap *p, int k);
void swapV3(T_indirectHeap *p, int i, int j);
void showHeapV3(T_indirectHeap *p);
void showHeap_recV3(T_indirectHeap *p, int root, int indent);
unsigned char removeMaxV3(T_indirectHeap *p);

int main(void){

	//T_heap * maximier = NULL; 
	
	//int ex4[] = {-5, -3, -1, -1, -1}; 
	
	/*
	// tri pas tas V1 
	maximier = initHeap(ex4, 13);
	showHeap(maximier);
	heapSortV1(maximier);
	maximier->nbElt = 13; 
	showHeap(maximier);
	*/
	
	/*
	// tri par tas V2
	//free(maximier);
	maximier = initHeap(ex4, 5);
	showHeap(maximier);

	buildHeapV2(maximier); 
	maximier->nbElt = 5; //a quoi ca sert
	showHeap(maximier);

	T_elt e = removeMax(maximier);
	showHeap(maximier);
	*/

	printf("******************************************\n");
	
	T_indirectHeap * ih = newHeapV3(5*MAXCARS); //todo, arbitraire
	ih->nbElt = 5;	

	for (int i = 0; i < MAXCARS; i++)
	{
		ih->tree[i] = 0;
	}
	ih->tree[0] = 65;
	ih->tree[1] = 66;
	ih->tree[2] = 67;
	ih->tree[3] = 68;
	ih->tree[4] = 82;

	for (int i = 0; i < 2*MAXCARS-1; i++)
	{
		ih->data[i] = 0;
	}
	ih->data[65] = -5;
	ih->data[66] = -3;
	ih->data[67] = -1;
	ih->data[68] = -1;
	ih->data[82] = -1;

	buildHeapV3(ih); // fait un maximier mais en prenant compte des occ. et non du code ASCII!!! donc c parfait
	showHeapV3(ih);

	//todo : finir de reimplementer en V3 (indirect heap), a voir quelles fonctions reimplemnter
	//continuer l'exemple ABRACADRA a la main ici dans le main (retirer les noeuds, ajouter un noeud etc etc) (fonctions pour retirer, inserer etc)
	//gerer le huffmantree, ie l'arbre de codage (liste des -256... ca a l'air ok)

	//1ere extraction
	unsigned char elt = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire :%s\n", elt);
	//printf("occurence du retirer : %d\n", ih->data[])

	//2eme extraction
	unsigned char elt2 = removeMaxV3(ih);
	showHeapV3(ih);

	//printf("retire : %s\n", toString(elt2));

	//insertion

	
}

int comparer(int a, int b){
	return a-b;
}

T_indirectHeap * newHeapV3(unsigned int nbMaxElt){
	T_indirectHeap * pAux;

	CHECK_IF(pAux = malloc(sizeof(T_indirectHeap)), NULL, "erreur malloc");
	CHECK_IF(pAux->tree = malloc(nbMaxElt * sizeof(unsigned char)), NULL, "erreur malloc");
	CHECK_IF(pAux->data = malloc(nbMaxElt * sizeof(int)), NULL, "erreur malloc");

	pAux->nbElt = 0;
	
	return pAux;
}


void buildHeapV3(T_indirectHeap * p){
	int k;
	int n;
	assert(p!=NULL);
	n = p->nbElt; 
	for(k=iLASTINTERNAL(n); k>=0; k--) {
		siftDownV3(p, k);
	}
}

void siftDownV3(T_indirectHeap *p, int k){
	int n; 
	assert(p!=NULL);
	n = p->nbElt; 
	int i; 
	if (!isINTREE(k,n)) return; 
	 
	while ( ! isLEAF(k,n) ) {
		if (isINTREE(iRCHILD(k),n) && (comparer(VALP_ih(p, iRCHILD(k)), VALP_ih(p,iLCHILD(k))) > 0)) i = iRCHILD(k); 
		else i = iLCHILD(k); 
		
		if (comparer(VALP_ih(p,k), VALP_ih(p,i)) < 0) {
			swapV3(p, k, i);
			k = i; 
		} else break;  
	}
}

void swapV3(T_indirectHeap *p, int i, int j) {
	unsigned char aux; 
	assert(p!=NULL);
	aux = p->tree[i]; 
	p->tree[i] = p->tree[j];
	p->tree[j] = aux;
}

void showHeapV3(T_indirectHeap *p) {
	assert(p!=NULL);
	printf("Affichage du tas (nbElt : %d)\n",p->nbElt);
	showHeap_recV3(p, 0, 0); 
}

void showHeap_recV3(T_indirectHeap *p, int root, int indent) {
	// affiche récursivement en mode pseudo graphique 
	// ordre de récurrence  : taille du sous-arbre 
	int i; 
	assert(p!=NULL);
	if (! isINTREE(root,p->nbElt)) return;  
	
	showHeap_recV3(p, iRCHILD(root), indent+1);
	for(i=0;i<indent;i++) {
		printf("\t"); 
	}
	printf("%s(i:%d)\n", VALP(p, root), root);
	showHeap_recV3(p, iLCHILD(root), indent+1);
}


unsigned char removeMaxV3(T_indirectHeap *p) {
	unsigned char aux;
	assert(p!=NULL);
	aux = p->tree[0];
	swapV3(p, 0, p->nbElt-1);
	p->nbElt--;
	siftDownV3(p, 0);
	return aux;
}






























T_heap * newHeap(unsigned int nbMaxElt){
	T_heap * pAux;
	CHECK_IF(pAux = malloc(sizeof(T_heap)), NULL, "erreur malloc");
	CHECK_IF(pAux->tree = malloc(nbMaxElt * sizeof(T_elt)), NULL, "erreur malloc");
	pAux->nbElt = 0; 
	pAux->nbMaxElt = nbMaxElt; 
	
	return pAux; 
}

void freeHeap(T_heap *p) {
	assert(p!=NULL);
	free(p->tree);
	free(p);
}


T_heap * initHeap(T_elt t[], int n) {
	T_heap *pAux = newHeap(n + HEAP_ALLOCATION_OFFSET); 
	memcpy(pAux->tree, t, n * sizeof(T_elt)); 
	pAux->nbMaxElt = n + HEAP_ALLOCATION_OFFSET;
	pAux->nbElt = n; 
	return pAux; 
}

void showHeap(T_heap *p) {
	assert(p!=NULL);
	printf("Affichage du tas (nbElt : %d)\n",p->nbElt);
	showHeap_rec(p,0,0); 
}

void showHeap_rec(T_heap *p, int root, int indent) {
	// affiche récursivement en mode pseudo graphique 
	// ordre de récurrence  : taille du sous-arbre 
	int i; 
	assert(p!=NULL);
	if (! isINTREE(root,p->nbElt)) return;  
	
	showHeap_rec(p, iRCHILD(root), indent+1);
	for(i=0;i<indent;i++) {
		printf("\t"); 
	}
	printf("%s(i:%d)\n",toString(VALP(p,root)), root);
	showHeap_rec(p, iLCHILD(root), indent+1);
}

void swap(T_heap *p, int i, int j) {
	T_elt aux; 
	assert(p!=NULL);
	aux = p->tree[i]; 
	p->tree[i] = p->tree[j];
	p->tree[j] = aux; 
}


void siftUp(T_heap *p, int k) {
	assert(p!=NULL);
	while ( !isROOT(k) && (eltCmp(VALP(p,k),VALP(p,iPARENT(k))) > 0 ) ) {
		swap(p,k,iPARENT(k)); 
		k = iPARENT(k); 
	}
}

void addElt(T_heap *p, T_elt e) {
	assert(p!=NULL);
	p->tree[p->nbElt] = e; 
	p->nbElt++; 
	siftUp(p,p->nbElt-1);
}

void buildHeapV1(T_heap * p){
	int k; 
	assert(p!=NULL);
	for(k=1;k<p->nbElt; k++) {
		siftUp(p, k);
	}
}

void siftDown(T_heap *p, int k) {

	int n; 
	assert(p!=NULL);
	n = p->nbElt; 
	int i; 
	if (! isINTREE(k,n)) return; 
	 
	while ( ! isLEAF(k,n) ) {
		if (isINTREE(iRCHILD(k),n) && (eltCmp(VALP(p,iRCHILD(k)),VALP(p,iLCHILD(k))) > 0)  ) i = iRCHILD(k); 
		else i = iLCHILD(k); 
		
		if (eltCmp(VALP(p,k),VALP(p,i)) < 0) {
			swap(p, k,i);
			k = i; 
		} else break;  
		
	}
}


T_elt getMax(const T_heap *p){
	assert(p!=NULL);
	return p->tree[0];
}


T_elt removeMax(T_heap *p) {
	T_elt aux; 
	assert(p!=NULL);
	aux = p->tree[0]; 
	swap(p,0,p->nbElt-1);
	p->nbElt--; 
	siftDown(p,0);
	return aux; 

}





void buildHeapV2(T_heap * p){
	int k; 
	int n; 
	assert(p!=NULL);
	n =p->nbElt; 
	for(k=iLASTINTERNAL(n); k>=0; k--) {
		siftDown(p,k);
	}
}


void heapSortV1(T_heap *p) {
	assert(p!=NULL);
	buildHeapV1(p);
	while(p->nbElt>1) {
		swap(p,0,p->nbElt-1); 
		p->nbElt--; 
		siftDown(p,0);
	}
}

void heapSortV2(T_heap *p) {
	assert(p!=NULL);
	buildHeapV2(p);
	while(p->nbElt>1) {
		removeMax(p); 
	}
}









