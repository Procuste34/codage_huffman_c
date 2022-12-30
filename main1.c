#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h> 
// ceil, floor : #include <math.h>

#include "include/traces.h"
#include "include/check.h"

#define MAXIMIER
//#define MINIMIER

#define iLCHILD(i) 			(2*i)+1
#define iRCHILD(i) 			(2*i)+2

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

typedef struct {
	T_elt elt; 
	T_elt * pElt; 
} T_data;

void tassifier(T_data d, int n, int root);
void heap_sort(T_data d, int n);

T_data genData(T_elt e, T_elt *pE) {
	T_data d;
	d.elt = e; 
	d.pElt = pE; 
	return d; 
}

int main()
{
    int array[] = {5, 3, 8, 1, 2, 9, 4, 7, 6};
    int size = 9;

    for (int i = 0; i < size; i++) printf("%d ", array[i]);
    printf("\n");

    T_data d = genData(0, array);

    heap_sort(d, size);

    for (int i = 0; i < size; i++)printf("%d ", array[i]);
    printf("\n");

    return 0;
}

void echanger(T_elt *p, int i, int j){
    T_elt temp = p[i];
    p[i] = p[j];
    p[j] =  temp;
}

int comparer(T_elt a, T_elt b){
    #ifdef MAXIMIER
        return eltCmp(a, b);
    #endif

    #ifdef MINIMIER
        return eltCmp(b, a);
    #endif

    //cas par défaut:
    return (a-b);
}

/*
Modifie en place le tableau pour maintenir la propriété maximier/minimier.

On suppose que la propriété de maximier/minimier est vraie sur le tableau d'entrée à l'exception de root
(donc seul root n'est pas à sa place)
Cette fonction déplace donc root (le fait plonger DANS l'arbre) afin de maintenir la propriété maximier/minimier/

Dans un premier temps, la fonction identifie "extreme", le plus grand/petit des 3 noeuds entre root et ses deux fils fg et fd

    root
   /    \
  fg    fd
  / \   / \
  X  X  X  X

elle échange ensuite root avec extreme et s'appelle récursivement sur le ss-arbre, avec root=extreme
(on continue de faire plonger le root initial DANS l'arbre)
*/
void tassifier(T_data d, int n, int root){
    //identification de extreme
    int *array = d.pElt;

    int extreme = root;
    int left = iLCHILD(root);
    int right = iRCHILD(root);

    if(left < n && comparer(array[left], array[extreme]) > 0){
        extreme = left;
    }

    if(right < n && comparer(array[right], array[extreme]) > 0){
        extreme = right;
    }

    //si extreme=root, alors root est à sa place, il n'y a rien à faire
    //sinon, on continue de le faire plonger dans l'arbre :
    if (extreme != root)
    {
        //on echange + appel rec.
        echanger(array, root, extreme);
        tassifier(genData(0, array), n, extreme);
    }
}

/*
Heap sort sur un T_data.

Dans un premier temps, modifie en place les éléments du tableau pour avoir un maximier/minimier.
Elle effectue alors le heap sort dans un second temps.
*/
void heap_sort(T_data d, int n){
    int *array = d.pElt;

    //construction du maximier/minimier
    //comment on s'y prend ?
    //on part du bas de l'arbre et on le remonte, en appelant tassifier à chaque fois

    //on part du constat suivant : les feuilles du tas se situent des indices floor(n/2)+1 à n
    //or, une feuille en elle-même est déjà un maximier/minimier
    //donc on peut partir des noeuds dont les fils sont des feuilles
    //et remonte jusqu'à l'indice 0.
    //(plus clair avec un papier et un crayon)
    for (int i = n/2-1; i >= 0; i--){
        tassifier(genData(0, array), n, i);
    }

    //heap sort
    for (int i = n - 1; i >= 0; i--){
        //a chaque passage, on sait que l'élément le plus grand/petit est la racine du tas, ie d'indice 0
        //(car on a appeler tassifier avant)

        //ainsi, on echange cet element extreme avec le dernier element de la portion non triee du tas
        //(portion qui commence à i)
        //la encore, plus clair avec un papier et un crayon

        echanger(array, 0, i);
        tassifier(genData(0, array), i, 0);
    }
}
