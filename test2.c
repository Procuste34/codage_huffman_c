#include <stdio.h>
#include <stdlib.h>

//pas bon car pas une redirection

int main(int argc, char *argv[]) {
    printf("nombre d'arguments : %d\n", argc);

  if (argc < 2) {
    printf("Error: no file specified\n");
    return 1;
  }

  // Open the file for reading
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Error: unable to open file\n");
    return 1;
  }

  // Read the contents of the file and print them to the console
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    putchar(ch);
  }

  // Close the file and return
  fclose(file);
  return 0;
}