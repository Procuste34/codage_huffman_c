#include <stdio.h>
#include <stdlib.h>

void heapify(int *array, int size, int root);
void heap_sort(int *array, int size);

int main()
{
    int array[] = {5, 3, 8, 1, 2, 9, 4, 7, 6};
    int size = sizeof(array) / sizeof(array[0]);

    for (int i = 0; i < size; i++)printf("%d ", array[i]);
    printf("\n");

    heap_sort(array, size);

    for (int i = 0; i < size; i++)printf("%d ", array[i]);
    printf("\n");

    return 0;
}

void heapify(int *array, int size, int root)
{
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    if (left < size && array[left] > array[largest])
    {
        largest = left;
    }

    if (right < size && array[right] > array[largest])
    {
        largest = right;
    }

    if (largest != root)
    {
        int temp = array[root];
        array[root] = array[largest];
        array[largest] = temp;

        heapify(array, size, largest);
    }
}

void heap_sort(int *array, int size)
{
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(array, size, i);
    }

    for (int i = size - 1; i >= 0; i--)
    {
        int temp = array[0];
        array[0] = array[i];
        array[i] = temp;

        heapify(array, i, 0);
    }
}
