#include <stdio.h>
#include <stdlib.h>

typedef struct coordinate {
  int x, y;
} coordinate;

// Verifica se uma coordenada esta presente no tour 
// Pre-condicao:  coordenada c eh valida
int is_in_tour(int tour[8][8], coordinate c)
{
  return tour[c.y][c.x] != -1;
}

// determina se as coordenadas nao levam a uma posicao fora do tabuleiro
int are_coord_in_bounds(coordinate test_coord)
{
  return  (test_coord.y >= 0 && test_coord.y < 8)
   && (test_coord.x >= 0 && test_coord.x < 8);
}

int is_mov_valid(int tour[][8], coordinate test_coord)
{
  int is_mov_in_tour = is_in_tour(tour, test_coord);
  int coord_in_bounds = are_coord_in_bounds(test_coord);

  return coord_in_bounds && !is_mov_in_tour;
}

// preenche o vetor mov_buffer retorna a quantidade de itens adicionados
int get_next_movements(int tour_size, int tour[8][8], coordinate mov_buffer[], coordinate curr_pos) {
  coordinate moves[] = {
    {1,2}, {2,1}, {2,-1}, {1,-2}, {-1,-2}, {-2,-1}, {-2,1}, {-1,2} 
  };

  int movements_length = 0;
  for (int i=0; i<8; i++) {
    coordinate test_coord = {curr_pos.x + moves[i].x, curr_pos.y + moves[i].y};
    if(is_mov_valid(tour, test_coord))
      mov_buffer[movements_length++] = test_coord;
  }
  return movements_length;
}

int knight(int tour[8][8], coordinate c, int n, long long int* backtracks, int* fail)
{
  tour[c.y][c.x] = n;
  if(n == 63)
    return 1;

  // definicao de proximos movimentos validos 
  coordinate valid_moves[8];
  int movements_length = get_next_movements(n, tour, valid_moves, c);

  for (int i=0; i<movements_length; i++) {
    if(*fail)
    {
      *fail = 0;
      tour[c.y][c.x] = -1;
      *(backtracks) += 1;
      return 0;
    }

    if(knight(tour, valid_moves[i], ++n, backtracks, fail)) {
      return 1;
    }

    *(backtracks) += 1;
    n--;
  }

  tour[c.y][c.x] = -1;
  *fail = 1;
  return 0;
}

void board_print(int board[8][8])
{
  // printando o tabuleiro
  for (int i=0; i<8; i++) {
    for (int j=0; j<7; j++) {
      // incrementando para converter de indice 0 para 1
      // board[i][j]++;

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
  // -1 para conversao de indice 1 para 0
  // coordinate c = {atoi(argv[1] -1), atoi(argv[2] -1)};
  coordinate c = {atoi(argv[2]), atoi(argv[1])};
  // inicializacao do tabuleiro
  int board[8][8];
  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
      board[i][j] = -1; // valor '-1' representa uma posicao disponivel

  if(!knight(board, c, 0, &backtracks, &fail)) {
    printf("Possibilidades exauridas; Nenhum passeio encontrado.\n");
    return 0;
  }

  // tabuleiro para mostrar o tour
  board_print(board) ;
  printf("Retrocedido: %llu vezes\n", backtracks);
}