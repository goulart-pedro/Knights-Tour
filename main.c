#include "passeio.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  int pos_x = atoi(argv[1]),
      pos_y = atoi(argv[2]);

  clock_t c;
  c = clock();
  passeio(pos_x, pos_y);

  c = clock() - c;
  printf("geral: %f\n", (double)c/CLOCKS_PER_SEC);
}
