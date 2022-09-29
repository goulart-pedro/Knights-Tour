#include <stdio.h>
#include <stdlib.h>

typedef struct coordinate {
  int x, y;
} coordinate;

// acha uma coordenado no tour
int tour_find(coordinate tour[], int tour_size, coordinate c)
{
  // determina se as coordenadas ja se encontram no tour
  int is_mov_in_tour = 0;
  // percorre o tour
  for(int i=0; i<tour_size; i++) {
    is_mov_in_tour =
     tour[i].x == c.x
     && tour[i].y == c.y;

    if(is_mov_in_tour)
      break;
  }

  return is_mov_in_tour;
}

// determina se as coordenadas nao levam a uma posicao fora do tabuleiro
int are_coord_in_bounds(coordinate test_coord)
{
  return  (test_coord.y >= 0 && test_coord.y < 8)
   && (test_coord.x >= 0 && test_coord.x < 8);
}

// preenche o vetor mov_buffer retorna a quantidade de itens adicionados
int get_next_movements(int tour_size, coordinate tour[], coordinate mov_buffer[], coordinate curr_pos) {
  coordinate moves[] = {
    {1,2}, {2,1}, {2,-1}, {1,-2}, {-1,-2}, {-2,-1}, {-2,1}, {-1,2} 
  };

  int movements_length = 0;
  for(int i=0; i<8; i++) {
    coordinate test_coord = {curr_pos.x + moves[i].x, curr_pos.y + moves[i].y};
    int is_mov_in_tour = tour_find(tour, tour_size, test_coord);
    int coord_in_bounds = are_coord_in_bounds(test_coord);

    if(!is_mov_in_tour && coord_in_bounds) {
      mov_buffer[movements_length++] = test_coord;
    }

  }
  return movements_length;
}

int knight(coordinate tour[], coordinate c, int n, long long int* backtracks, int* fail)
{
  tour[n] = c;
  if(n == 63)
    return 1;

  // definicao de proximos movimentos validos 
  coordinate valid_moves[8];
  int movements_length = get_next_movements(n, tour, valid_moves, c);
  if(movements_length == 0)
  {
    *fail = 1;
    return 0;
  }

  for(int i=0; i<movements_length; i++) {
    if(*fail)
    {
      *fail = 0;
      // *(backtracks)++;
      return 0;
    }

    if(knight(tour, valid_moves[i], ++n, backtracks, fail)) {
      return 1;
    }

    *(backtracks) += 1;
    n--;
  }
  return 0;
}

void board_print(int board[8][8], coordinate tour[])
{
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

int main(int argc, char** argv)
{
  int fail = 0;
  long long int backtracks = 0;
  coordinate tour[64];
  coordinate c = {atoi(argv[1]), atoi(argv[2])};
  if(!knight(tour, c, 0, &backtracks, &fail)) {
    printf("Possibilidades exauridas; Nenhum passeio encontrado.\n");
    return 0;
  }

  // tabuleiro para mostrar o tour
  int board[8][8];
  board_print(board, tour) ;
  printf("Retrocidido: %llu vezes\n", backtracks);
}

