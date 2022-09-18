#include <stdio.h>

typedef struct coordinate {
  int x, y;
} coordinate;

// preenche o vetor valid_movements e retorna a quantidade de itens adicionados
int get_valid_movements(int n, coordinate tour[], coordinate valid_movements[], coordinate c) {
  int moves_y[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
  int moves_x[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

  int movements_length = 0;
  for(int i=0; i<8; i++) {
    // determina se as coordenadas ja se encontram no tour
    int is_mov_in_tour = 0;
    for(int j=0; j<n; j++) {
      is_mov_in_tour = 
        tour[j].x == c.x + moves_x[i] 
        && tour[j].y == c.y + moves_y[i];

      if(is_mov_in_tour)
        break;
    }

    // determina se as coordenadas nao levam a uma posicao fora do tabuleiro
    // caso verdadeiro, adiciona as coordenadas aos movimentos validos
    if(
        !is_mov_in_tour &&
        (c.y + moves_y[i] >= 0 && c.y + moves_y[i] < 8) && 
        (c.x + moves_x[i] >= 0 && c.x + moves_x[i] < 8)
      ) {
      movements_length++;
      coordinate new_move;
      new_move.x = c.x + moves_x[i];
      new_move.y = c.y + moves_y[i];
      valid_movements[i] = new_move;
    }
  
  }  
  return movements_length;
}

void knight(coordinate tour[], coordinate c, int n, int* stop)
{
  tour[n] = c;
  // ate 62 pois a primeira coordenada eh preenchida por padrao
  if(n == 62)
    *stop = 1;

  if(*stop)
    return;

  // definicao de proximos movimentos validos 
  coordinate valid_moves[8];
  int movements_length = get_valid_movements(n, tour, valid_moves, c);

  // guarda para caminho sem fim
  if(movements_length == 0)
    return;

  for(int i=0; i<movements_length; i++) {
    knight(tour, valid_moves[i], ++n, stop);
    n--;
  }

  // caso a iteracao inteira tenha dado errado
  // aparentemente inutil ???
  // n--;
}


int main()
{
  int stop = 0;
  coordinate tour[64];
  coordinate c = {0, 0};
  knight(tour, c, 0, &stop);
  for(int i=0; i<63; i++){
    printf("[COORDINATE]\t[%d %d]\n", tour[i].x, tour[i].y);
  }

}

