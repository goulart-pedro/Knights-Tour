#include <stdio.h>
#include <stdlib.h>

typedef struct coordinate {
  int x, y;
} coordinate;

// Verifica se uma coordenada esta presente no tour 
// Pre-condicao:  coordenada c eh valida
int is_visited(int tour[8][8], coordinate c)
{
  return tour[c.y][c.x] != -1;
}

// determina se as coordenadas nao levam a uma posicao fora do tabuleiro
int is_coord_in_bounds(coordinate test_coord)
{
  return  (test_coord.y >= 0 && test_coord.y < 8)
   && (test_coord.x >= 0 && test_coord.x < 8);
}

int is_coord_valid(int tour[][8], coordinate test_coord)
{
  int is_mov_in_tour = is_visited(tour, test_coord);
  int coord_in_bounds = is_coord_in_bounds(test_coord);

  return coord_in_bounds && !is_mov_in_tour;
}

// preenche o vetor mov_buffer retorna a quantidade de itens adicionados
int get_neighbours(int tour[8][8], coordinate neighbour_buffer[], coordinate curr_pos) {
  coordinate moves[] = {
    {1,2}, {2,1}, {2,-1}, {1,-2}, {-1,-2}, {-2,-1}, {-2,1}, {-1,2} 
  };

  int neighbour_amount = 0;
  for (int i=0; i<8; i++) {
    coordinate test_coord = {curr_pos.x + moves[i].x, curr_pos.y + moves[i].y};

    if(is_coord_valid(tour, test_coord))
      neighbour_buffer[neighbour_amount++] = test_coord;
  }
  return neighbour_amount;
}

void visit(int tour[][8], coordinate c, int n)
{
  tour[c.y][c.x] = n;
}

// Preenche o tour com movimentos do cavalo pelo algoritmo de Pesquisa por Profundidade
// Retorna 1 se o tour foi completo
// Retorna 0 se o tour for impossivel
int knight(int tour[8][8], coordinate c, int n, long long int* backtracks, long int* visits)
{
  // marca como visitada
  visit(tour, c, n);
  *(visits) += 1;

  // tour completo
  if(n == 64)
    return 1;

  // definicao de proximos movimentos validos 
  coordinate neighbours[8];
  int neighbour_amount = get_neighbours(tour, neighbours, c);

  for (int i=0; i<neighbour_amount; i++)
  {
    int tour_result = knight(tour, neighbours[i], n+1, backtracks, visits);

    if (tour_result == 1)
      return 1;
    else
      *(backtracks) += 1;
  }

  // de-visita
  visit(tour, c, -1);
  return 0;
}

void board_print(int board[8][8])
{
  // printando o tabuleiro
  for (int i=0; i<8; i++) {
    for (int j=0; j<7; j++) {
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
  long int visits = 0;
  long long int backtracks = 0;
  int n = 1;
  coordinate c = {atoi(argv[2]) -1, atoi(argv[1]) -1};

  // inicializacao do tabuleiro
  int board[8][8];
  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
      board[i][j] = -1; // valor '-1' representa uma posicao disponivel

  if(!knight(board, c, n, &backtracks, &visits)) {
    printf("Possibilidades exauridas; Nenhum passeio encontrado.\n");
    return 0;
  }

  // tabuleiro para mostrar o tour
  board_print(board) ;
  printf("%ld %lld\n", visits, backtracks);
}