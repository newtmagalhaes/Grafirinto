#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _vertex
{
  struct _vertex **edgeArr;
  unsigned edgeArrLength;
  char *valueID;
  int exit;
} Vertex;

typedef struct _graph
{
  struct _vertex **vertexArr;
  unsigned vertexArrLength;
} Graph;

// Checa se há item duplicado
int checkDuplicate(int arr[], int size, int value)
{
  for (int i = 0; i < size; i++)
  {
    if (arr[i] == value)
    {
      return 1;
    }
  }
  return 0;
}

// Cria um Vétice novo.
Vertex *newVertex(char *newValueID)
{
  Vertex *vertex = (Vertex *)malloc(sizeof(Vertex));
  vertex->valueID = newValueID;
  vertex->exit = 0;
  return vertex;
}

// Cria uma aresta entre os vértices A e B
void newEdge(Vertex *A, Vertex *B)
{
  int auxA = A->edgeArrLength;
  int auxB = B->edgeArrLength;

  // Realocamos
  A->edgeArr = (Vertex **)realloc(A->edgeArr, (A->edgeArrLength + 1) * sizeof(Vertex));
  A->edgeArr[auxA] = B;

  B->edgeArr = (Vertex **)realloc(B->edgeArr, (B->edgeArrLength + 1) * sizeof(Vertex));
  B->edgeArr[auxB] = A;

  A->edgeArrLength++;
  B->edgeArrLength++;
}

// Cria múltiplas arestas entre o vértice target os vértices em vertexArr
void newMultipleEdge(Vertex *target, Vertex *vertexArr[], unsigned vertexArrLength)
{
  for (unsigned i = 0; i < vertexArrLength; i++)
  {
    newEdge(target, vertexArr[i]);
  }
}

Graph *createGraph(unsigned difficult)
{
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  printf("Debug.");

  graph->vertexArrLength = difficult * 3;

  // Array de vértices principais, intuito de debug.
  Vertex **principais = (Vertex **)malloc(graph->vertexArrLength * sizeof(Vertex *));

  srand(NULL);

  // Variáveis para ver se há repetição.
  int integerNames[graph->vertexArrLength];
  int integersNamesSize = 0;

  // Cria os vertices com nome de caverna_ + random.
  for (int i = 0; i < difficult; i++)
  {
    int rand;
    // Impede repetição
    do
    {
      rand = (int)(random() % 10000);
    } while (checkDuplicate(integerNames, integersNamesSize, rand));

    integerNames[integersNamesSize] = rand;
    integersNamesSize++;

    char *name;
    sprintf(name, "caverna_%d", rand);
    principais[i] = newVertex(name);
  }

  // Cria a sequência principal.
  for (int i = 1; i < difficult; i++)
  {
    Vertex *atual = principais[i - 1];
    printf("Valor: %s\n", atual->valueID);
    newEdge(atual, principais[i]);
  }

  // Atribui o último
  principais[difficult - 1]->exit = 1;

  // Cria os Vertices de distração
  for (int i = difficult; i < graph->vertexArrLength; i++)
  {
    int rand;
    // Impede repetição
    do
    {
      rand = (int)(random() % 10000);
    } while (checkDuplicate(integerNames, integersNamesSize, rand));

    integerNames[integersNamesSize] = rand;
    integersNamesSize++;

    char *name;
    sprintf(name, "caverna_%d", rand);
    principais[i] = newVertex(name);
  }

  // Colocar os vértices de distração de forma randômica nos vertices principais
  for (int i = difficult; i < graph->vertexArrLength; i++)
  {
    for (int j = 0; j < graph->vertexArrLength; j++)
    {
      if (i != j)
      {
        int chance = (int)random() % 100;
        if (chance < 50)
        {
          // Impedir duplicadas.
          newEdge(principais[i], principais[j]);
        }
      }
    }
  }

  graph->vertexArr = principais;
  return graph;
}

// Libera da memória o Grafo atual.
void freeGraph(Graph *graph)
{
  int aux = graph->vertexArrLength;
  for (int i = 0; i < aux; i++)
  {
    Vertex *vert = graph->vertexArr[i];
    free(vert->edgeArr);
    free(vert);
  }
  free(graph);
}
