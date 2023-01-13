#include "tri_utils.h"

T_data genData(T_elt e, T_elt *pE){
	T_data d;
	d.elt = e; 
	d.pElt = pE; 
	return d; 
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
    //on part du bas de l'arbre et on le remonte, 
    //en appelant tassifier à chaque fois

    //on part du constat suivant : les feuilles du tas se situent des 
    //indices floor(n/2)+1 à n
    //or, une feuille en elle-même est déjà un maximier/minimier
    //donc on peut partir des noeuds dont les fils sont des feuilles
    //et remonte jusqu'à l'indice 0.
    //(plus clair avec un papier et un crayon)
    for (int i = n/2-1; i >= 0; i--){
        tassifier(genData(0, array), n, i);
    }

    //heap sort
    for (int i = n - 1; i >= 0; i--){
        //a chaque passage, on sait que l'élément le plus grand/petit est 
        //la racine du tas, ie d'indice 0
        //(car on a appeler tassifier avant)

        //ainsi, on echange cet element extreme avec le dernier element de 
        //la portion non triee du tas
        //(portion qui commence à i)
        //la encore, plus clair avec un papier et un crayon

        echanger(array, 0, i);
        tassifier(genData(0, array), i, 0);
    }
}
