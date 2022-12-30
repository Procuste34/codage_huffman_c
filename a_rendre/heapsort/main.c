#include <stdio.h>
#include <assert.h>
#include <math.h> 
// ceil, floor : #include <math.h>

#include "../include/traces.h"
#include "../include/check.h"

#include "elt.h"
#include "tri_utils.h"

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
