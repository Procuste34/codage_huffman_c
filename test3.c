#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 100

int main(void) {
  char str[MAX_LENGTH];
  char *ptr;
  printf("Enter a string: ");
  fgets(str, MAX_LENGTH, stdin);
  ptr = strtok(str, "\n");
  printf("You entered: '%s'\n", ptr);
  
  return 0;
}