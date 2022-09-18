#include <stdio.h>
#include <stdlib.h>

typedef struct coordinate {
  int x, y;
} coordinate;

// preenche o vetor valid_movements e retorna a quantidade de itens adicionados
int get_valid_movements(int n, coordinate tour[], coordinate valid_movements[], coordinate c) {
  int moves_y[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
  int moves_x[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

  int mov_insertion_idx = 0;
  int movements_length = 0;
  // percorre as opcoes de variacao de posicao
  for(int i=0; i<8; i++) {
    // determina se as coordenadas ja se encontram no tour
    int is_mov_in_tour = 0;
    // percorre o tour
    for(int j=0; j<n; j++) {
      is_mov_in_tour = 
        tour[j].x == c.x + moves_x[i] &&
        tour[j].y == c.y + moves_y[i];

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
      valid_movements[mov_insertion_idx++] = new_move;
    }
  
  }  
  return movements_length;
}

int knight(coordinate tour[], coordinate c, int n)
{
  tour[n] = c;
  if(n == 63)
    return 1;

  // definicao de proximos movimentos validos 
  coordinate valid_moves[8];
  int movements_length = get_valid_movements(n, tour, valid_moves, c);

  for(int i=0; i<movements_length; i++) {
    if(knight(tour, valid_moves[i], ++n)) {
      return 1;
    }
    n--;
  }
  return 0;
}


int main(int argc, char** argv)
{
  coordinate tour[64];
  coordinate c = {atoi(argv[1]), atoi(argv[2])};
  if(!knight(tour, c, 0)) {
    printf("Possibilidades exauridas; Nenhum passeio encontrado.\n");
    return 0;
  }

  // tabuleiro para mostrar o tour
  int board[8][8];
  for(int i=0; i<64; i++){
    board[tour[i].x][tour[i].y] = i+1;
  }

  // printando o tabuleiro
  for(int i=0; i<8; i++) {
    for(int j=0; j<7; j++) {
      // verifica se o numero tem apenas um digito
      // se sim, printa o numero com um zero a frente
      if(board[i][j] /10 < 1)
        printf("0%d ", board[i][j]);
      else  
        printf("%d ", board[i][j]);
    }
    if(board[i][7] /10 < 1)
      printf("0%d\n", board[i][7]);
    else  
      printf("%d\n", board[i][7]);
  }
  
}

