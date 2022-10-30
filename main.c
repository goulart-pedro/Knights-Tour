#include "passeio.h"
#include <stdlib.h>

int main(int argc, char** argv)
{
  // passeio(atoi(argv[1]) ,atoi(argv[2]));
  for (int i=1; i<=N; ++i)
    for (int j=1; j<=N; ++j)
      passeio(i, j);
}