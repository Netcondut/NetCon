#include "../include/Error.h"

void fatalError(char *message)
{
  fprintf(stderr, "main: %s\n", message);
}
