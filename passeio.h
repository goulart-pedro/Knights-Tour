#include <stdio.h>
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
  return board[c.y][c.x] != -1;
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
  board[c.y][c.x] = n;
}

// Heuristica:
// Ordena os movimentos pela maior distancia euclidiana de suas coordenadas ate o centro do tabuleiro
// Utilizando Selection Sort
void order_by_max_distance(coordinate neighbours[], int neighbour_amount)
{
    int i, j, min_idx;
    double max_center_distance = sqrt(pow(neighbours[0].x - 3.5, 2) + pow(neighbours[0].y - 3.5, 2))  ;
    for (i = 0; i < neighbour_amount-1; i++)
    {
        min_idx = i;
        for (j = i+1; j < neighbour_amount; j++)
        {
          double curr_center_distance = sqrt(pow(neighbours[j].x - 3.5, 2) + pow(neighbours[j].y -3.5, 2));
          if (curr_center_distance > max_center_distance)
          {
            min_idx = j;
            max_center_distance = curr_center_distance;
          }
        }
        
        coordinate temp = neighbours[min_idx];
        neighbours[min_idx] = neighbours[i];
        neighbours[i] = temp;
    }
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
  order_by_max_distance(neighbours, neighbour_amount);

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

void board_print(int board[8][8])
{
  // printando o tabuleiro
  for (int i=0; i<8; i++) {
    for (int j=0; j<7; j++) {
      // printa o numero com um zero a frente
      printf("%.2d ", board[i][j]);
    }
    printf("%.2d\n", board[i][7]);
  }
}

void passeio(int x, int y)
{
  int board[8][8];
  for (int i=0; i<8; ++i)
    for (int j=0; j<8; ++j)
      board[i][j] = -1; // -1 representa uma posicao ainda nao acessada
  
  horse h = {0, 0, 0};
  int n = 1;

  coordinate c = {x, y};
  int tour_result = !tour(board, c, n, &h);
  if(tour_result == 1) {
    printf("Possibilidades exauridas; Nenhum passeio encontrado.\n");
    return;
  }

  // tabuleiro para mostrar o tour
  board_print(board);
  printf("%ld %ld\n", h.visits, h.backtracks);
}