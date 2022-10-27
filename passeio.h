#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
  int x, y;
} coordinate;

typedef struct
{
  long int visits;
  long int backtracks;
  int fail;
} horse;

// Verifica se uma coordenada esta presente no tour 
// Pre-condicao:  coordenada c eh valida
int is_visited(int board[8][8], coordinate c)
{
  return board[c.x][c.y] != -1;
}

// determina se as coordenadas nao levam a uma posicao fora do tabuleiro
int is_coord_in_bounds(coordinate test_coord)
{
  return  (test_coord.y >= 0 && test_coord.y < 8)
   && (test_coord.x >= 0 && test_coord.x < 8);
}

int is_coord_valid(int board[][8], coordinate test_coord)
{
  int is_mov_in_tour = is_visited(board, test_coord);
  int coord_in_bounds = is_coord_in_bounds(test_coord);

  return coord_in_bounds && !is_mov_in_tour;
}

// preenche o vetor mov_buffer retorna a quantidade de itens adicionados
int get_neighbours(int board[8][8], coordinate neighbour_buffer[], coordinate curr_pos) {
  coordinate moves[] = {
    {1,2}, {2,1}, {2,-1}, {1,-2}, {-1,-2}, {-2,-1}, {-2,1}, {-1,2} 
  };

  int neighbour_amount = 0;
  for (int i=0; i<8; i++) {
    coordinate test_coord = {curr_pos.x + moves[i].x, curr_pos.y + moves[i].y};

    if(is_coord_valid(board, test_coord))
      neighbour_buffer[neighbour_amount++] = test_coord;
  }
  return neighbour_amount;
}

void visit(int board[][8], coordinate c, int n)
{
  board[c.x][c.y] = n;
}

int compare_coordinate(const void* a, const void *b)
{
  coordinate *curr = (coordinate *)a,
             *next = (coordinate *)b;

  double curr_norm = pow(curr->x - 3.5, 2) + pow(curr->y - 3.5, 2),
         next_norm = pow(next->x - 3.5, 2) + pow(next->y - 3.5, 2);

  return next_norm - curr_norm;
}

// Heuristica - Regra de Warsndorff
// Ordena os movimentos pela maior distancia euclidiana de suas coordenadas ate o centro do tabuleiro
// Utilizando Quick Sort
void warnsdorff(coordinate neighbours[], int neighbour_amount)
{
  qsort(neighbours, neighbour_amount, sizeof(coordinate), compare_coordinate);
}
// Preenche o tour com movimentos do cavalo pelo algoritmo de Pesquisa por Profundidade
// Retorna 1 se o tour foi completo
// Retorna 0 se o tour for impossivel
int tour(int board[8][8], coordinate c, int n, horse *h)
{
  // marca como visitada
  visit(board, c, n);
  h->visits++;

  // tour completo
  if(n == 64)
    return 1;

  // definicao de proximos movimentos validos 
  coordinate neighbours[8];
  int neighbour_amount = get_neighbours(board, neighbours, c);
  warnsdorff(neighbours, neighbour_amount);

  for (int i=0; i<neighbour_amount; i++)
  {
    int tour_result = tour(board, neighbours[i], n+1, h);
    if (tour_result == 1)
      return 1;
    else
      h->backtracks++;
  }

  // de-visita
  visit(board, c, -1);
  return 0;
}

void print_results(int board[8][8], horse h)
{
  FILE* saida = fopen("saida.txt", "a");
  // printando o tabuleiro
  for (int i=0; i<8; i++) {
    for (int j=0; j<7; j++) {
      // printa o numero com um zero a frente
      fprintf(saida, "%.2d ", board[i][j]);
    }
    fprintf(saida, "%.2d\n", board[i][7]);
  }
  fprintf(saida, "%ld %ld\n", h.visits, h.backtracks);
  fclose(saida);
}

void passeio(int x, int y)
{
  int board[8][8];
  for (int i=0; i<8; ++i)
    for (int j=0; j<8; ++j)
      board[i][j] = -1; // -1 representa uma posicao ainda nao acessada
  
  horse h = {0, 0, 0};
  int n = 1;

  coordinate c = {x-1, y-1};
  int tour_result = !tour(board, c, n, &h);
  if(tour_result == 1) {
    printf("Possibilidades exauridas; Nenhum passeio encontrado.\n");
    return;
  }

  print_results(board, h);
}