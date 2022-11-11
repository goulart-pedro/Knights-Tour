#include <stdio.h>
#include <stdlib.h>

#define N 8

typedef struct
{
  int x, y;
} coordinate;

typedef struct
{
  long int visits;
  long int backtracks;
  int was_leaf_found; // indica se e necessario retroceder pois o tour e impossivel
} horse;

// substitui a funcao pow()
// necessario para nao incluir a math.h
int power2(int x)
{
  return x*x;
}

// Verifica se uma coordenada esta presente no tour 
// Pre-condicao:  coordenada c eh valida
int is_visited(int board[N][N], coordinate c)
{
  return board[c.x][c.y] != -1;
}

// determina se as coordenadas nao levam a uma posicao fora do tabuleiro
int is_coord_in_bounds(coordinate test_coord)
{
  return  (test_coord.y >= 0 && test_coord.y < N)
   && (test_coord.x >= 0 && test_coord.x < N);
}

int is_coord_valid(int board[N][N], coordinate test_coord)
{
  int is_mov_in_tour = is_visited(board, test_coord);
  int coord_in_bounds = is_coord_in_bounds(test_coord);

  return coord_in_bounds && !is_mov_in_tour;
}

// preenche o vetor mov_buffer retorna a quantidade de itens adicionados
int get_neighbours(int board[N][N], coordinate neighbour_buffer[], coordinate curr_pos) {
  coordinate moves[] = {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

  int neighbour_amount = 0;
  for (int i=0; i<8; i++) {
    coordinate test_coord = {curr_pos.x + moves[i].x, curr_pos.y + moves[i].y};

    if(is_coord_valid(board, test_coord))
      neighbour_buffer[neighbour_amount++] = test_coord;
  }
  return neighbour_amount;
}

void visit(int board[][N], coordinate c, int n)
{
  board[c.x][c.y] = n;
}

int compare_coordinates(const void* a, const void *b)
{
  coordinate *curr = (coordinate *)a,
             *next = (coordinate *)b;

  double board_center = (double)((N - 1)) / 2,
         curr_norm    = power2(curr->x - board_center) + power2(curr->y - board_center),
         next_norm    = power2(next->x - board_center) + power2(next->y - board_center);

  return next_norm - curr_norm;
}

// Heuristica - Regra de Warnsdorff
// Ordena os movimentos pela maior distancia euclidiana de suas coordenadas ate o centro do tabuleiro
// Utilizando Quick Sort
void warnsdorff(coordinate neighbours[], int neighbour_amount)
{
  qsort(neighbours, neighbour_amount, sizeof(coordinate), compare_coordinates);
}
// Preenche o tour com movimentos do cavalo pelo algoritmo de Pesquisa por Profundidade
// Retorna 1 se o tour foi completo
// Retorna 0 se o tour for impossivel
int tour(int board[N][N], coordinate c, int n, horse *h)
{
  // marca como visitada
  visit(board, c, n);
  h->visits++;

  // tour completo
  if (n == N*N)
    return 1;

  // definicao de proximos movimentos validos 
  coordinate neighbours[8];
  int neighbour_amount = get_neighbours(board, neighbours, c);
  warnsdorff(neighbours, neighbour_amount);

  if (neighbour_amount == 0) h->was_leaf_found = 1;

  for (int i=0; i<neighbour_amount; i++)
  {
    int tour_result = tour(board, neighbours[i], n+1, h);
    if (tour_result == 1)
      return 1;

    // pruning
    // caso verdadeiro, interrompe a busca pelos vizinhos de c
    // e retorna a casa que chamou este nivel
    if(h->was_leaf_found)
    {
      h->was_leaf_found = 0;
      h->backtracks++;
      break;
    }
    
    h->backtracks++;
  }

  // de-visita
  visit(board, c, -1);
  return 0;
}

void print_results(int board[N][N], horse h, FILE* output_file)
{
  // printando o tabuleiro
  for (int i=0; i<N; i++) {
    for (int j=0; j<N-1; j++) {
      // printa o numero com um zero a frente
      fprintf(output_file, "%.2d ", board[i][j]);
    }
    fprintf(output_file, "%.2d\n", board[i][N-1]);
  }
  fprintf(output_file, "%ld %ld\n", h.visits, h.backtracks);
}

void passeio(int x, int y)
{
  int board[N][N];
  for (int i=0; i<N; ++i)
    for (int j=0; j<N; ++j)
      board[i][j] = -1; // -1 representa uma posicao ainda nao acessada
  
  horse h = {0, 0, 0};
  int n = 1;

  FILE* saida = fopen("saida.txt", "a");
  coordinate c = {x-1, y-1};
  int tour_result = !tour(board, c, n, &h);
  if(tour_result == 1) {
    fprintf(saida, "Possibilidades exauridas; Nenhum passeio encontrado.\n");
    fclose(saida);
    return;
  }

  print_results(board, h, saida);
  fclose(saida);
}