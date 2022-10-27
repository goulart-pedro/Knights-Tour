#include "passeio.h"
#include <stdlib.h>

int main(int argc, char** argv)
{
  for (int i=1; i<9; i++)
    for(int j=1; j<9; j++)
      passeio(i,j);
}