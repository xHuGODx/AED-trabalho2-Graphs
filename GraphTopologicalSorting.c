//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Topological Sorting
//

#include "GraphTopologicalSorting.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersQueue.h"
#include "instrumentation.h"

void InstrInit(void) { ///
  InstrCalibrate();
  InstrName[0] = "Iteracoes";
  InstrName[1] = "DegreeUptd";
  InstrName[2] = "InDegreeComp";
  InstrName[3] = "DAGcess";
}

#define ITER InstrCount[0]
#define DEGREEUPDT InstrCount[1]
#define DEGREECOMP InstrCount[2] 
#define DAGCESS InstrCount[3]

struct _GraphTopoSort {
  int* marked;                     // Aux array
  unsigned int* numIncomingEdges;  // Aux array
  unsigned int* vertexSequence;    // The result
  int validResult;                 // 0 or 1
  unsigned int numVertices;        // From the graph
  Graph* graph;
};

// AUXILIARY FUNCTION
// Allocate memory for the struct
// And for its array fields
// Initialize all struct fields
//
static GraphTopoSort* _create(Graph* g) {
  assert(g != NULL);

  GraphTopoSort* p = malloc(sizeof(struct _GraphTopoSort));

  // TO BE COMPLETED
  p->marked = calloc(GraphGetNumVertices(g), sizeof(int));
  p->numIncomingEdges = calloc(GraphGetNumVertices(g), sizeof(int));
  for (unsigned int i = 0; i < GraphGetNumVertices(g); i++){
    p->numIncomingEdges[i] = GraphGetVertexInDegree(g, i);
  }
  p->vertexSequence = calloc(GraphGetNumVertices(g), sizeof(int));
  p->validResult = 0;
  p->numVertices = GraphGetNumVertices(g);
  p->graph = g; 

  return p;
}

//AUXILIARY FUNCTION
//Find next vertex with no incoming edges that hasn't been marked yet.
//Returns id of the vertex.
//If there is no match then the number of vertices is returned (it isn't an index of the list)
static unsigned int _FindNextInc(GraphTopoSort* g, Graph* gr){
  for (unsigned int i = 0; i < g->numVertices; i++){
    DEGREECOMP++;
    ITER++;
    if (GraphGetVertexInDegree(gr, i) == 0 && g->marked[i] == 0){
      return i;
    }
  }
  return g->numVertices;
}

//
// Computing the topological sorting, if any, using the 1st algorithm:
// 1 - Create a copy of the graph
// 2 - Successively identify vertices without incoming edges and remove their
//     outgoing edges
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort* GraphTopoSortComputeV1(Graph* g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort* topoSort = _create(g);

  // Build the topological sorting

  Graph* gCopy = GraphCopy(g); //Copying the original graph
  unsigned int counter = 0; //used to keep track of the sequence of the result
  unsigned int i; 
  do {
    i = _FindNextInc(topoSort, gCopy);
    if (i == topoSort->numVertices){
      break;
    }
    topoSort->marked[i] = 1;
    topoSort->vertexSequence[counter++] = i;
    unsigned int* adj = GraphGetAdjacentsTo(gCopy, i);
    for (unsigned int j = 1; j <= adj[0]; j++){
      ITER++;
      GraphRemoveEdge(gCopy, i, adj[j]);
    }
    free(adj);
  } while (i < topoSort->numVertices && counter < topoSort->numVertices);
  topoSort->validResult = (counter == topoSort->numVertices) ? 1 : 0;
  GraphDestroy(&gCopy);
  return topoSort;
}


//AUXILIARY FUNCTION
//Find next vertex with no incoming edges that hasn't been marked yet.
//Returns id of the vertex.
//If there is no match then the number of vertices is returned (it isn't an index of the list)
static unsigned int _FindNextInd(GraphTopoSort* g){
  for (unsigned int i = 0; i < g->numVertices; i++){
    DEGREECOMP++;
    ITER++;
    if (g->numIncomingEdges[i]==0 && g->marked[i] == 0){
      return i;
    }
  }
  return g->numVertices;
}

//
// Computing the topological sorting, if any, using the 2nd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort* GraphTopoSortComputeV2(Graph* g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort* topoSort = _create(g);

  // Build the topological sorting

  // TO BE COMPLETED
  //...

  unsigned int counter = 0; //used to keep track of the sequence of the result
  unsigned int i;
  do {
    i = _FindNextInd(topoSort);
    topoSort->marked[i] = 1;
    topoSort->vertexSequence[counter++] = i;
    unsigned int* adj = GraphGetAdjacentsTo(g, i);
    for (unsigned int j = 1; j <= adj[0]; j++){
      ITER++;
      DEGREEUPDT++;
      topoSort->numIncomingEdges[adj[j]]--;
    }
    free(adj);
  } while (i < topoSort->numVertices && counter < topoSort->numVertices);
  topoSort->validResult = (counter == topoSort->numVertices) ? 1 : 0;
  return topoSort;
}

//
// Computing the topological sorting, if any, using the 3rd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//

/*Registar num array auxiliar numEdgesPerVertex o InDegree de cada vértice
Criar FILA vazia e inserir na FILA os vértices v com numEdgesPerVertex[v] == 0
Enquanto a FILA não for vazia
v = retirar próximo vértice da FILA
Imprimir o seu ID
Para cada vértice w adjacente a v
numEdgesPerVertex[w] --
Se numEdgesPerVertex[w] == 0 Então Inserir w na FILA
*/

GraphTopoSort* GraphTopoSortComputeV3(Graph* g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  unsigned int counter = 0;

  unsigned int numVertices = GraphGetNumVertices(g);

  // Create and initialize the struct
  GraphTopoSort* topoSort = _create(g);

  
  
  // Create a queue and enqueue all vertices with in-degree 0
  Queue* queue = QueueCreate(numVertices);
  for (unsigned int i = 0; i < numVertices; i++) {
    DEGREECOMP++;
    ITER++;
    if (topoSort->numIncomingEdges[i] == 0) {
      QueueEnqueue(queue, i);
    }
  }

  // Process vertices in the queue
  while (!QueueIsEmpty(queue)) {
    unsigned int v = QueueDequeue(queue);

    // Add the vertex ID to the topoSort
    topoSort->vertexSequence[counter++] = v;

    // Decrease in-degree of all vertices adjacent to v

    unsigned int* adj = GraphGetAdjacentsTo(g, v);
    for (unsigned int j = 1; j <= adj[0]; j++){
      ITER++;
      DEGREEUPDT++;
      topoSort->numIncomingEdges[adj[j]]--;
      DEGREECOMP++;
      if (topoSort->numIncomingEdges[adj[j]] == 0){
        QueueEnqueue(queue, adj[j]);
      }
    }
    free(adj);
  }

  // Clean up
  QueueDestroy(&queue);
  topoSort->validResult = (counter == topoSort->numVertices) ? 1 : 0;
  return topoSort;
}

void GraphTopoSortDestroy(GraphTopoSort** p) {
  assert(*p != NULL);

  GraphTopoSort* aux = *p;

  free(aux->marked);
  free(aux->numIncomingEdges);
  free(aux->vertexSequence);

  free(*p);
  *p = NULL;
}

//
// A valid sorting was computed?
//
int GraphTopoSortIsValid(const GraphTopoSort* p) { return p->validResult; }

//
// Getting an array containing the topological sequence of vertex indices
// Or NULL, if no sequence could be computed
// MEMORY IS ALLOCATED FOR THE RESULTING ARRAY
//
unsigned int* GraphTopoSortGetSequence(const GraphTopoSort* p) {
  assert(p != NULL);
  return (p->validResult == 1) ? p->vertexSequence : NULL;
}

// DISPLAYING on the console

//
// The toplogical sequence of vertex indices, if it could be computed
//
void GraphTopoSortDisplaySequence(const GraphTopoSort* p) {
  assert(p != NULL);

  if (p->validResult == 0) {
    printf(" *** The topological sorting could not be computed!! *** \n");
    return;
  }

  printf("Topological Sorting - Vertex indices:\n");
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
    printf("%d ", p->vertexSequence[i]);
  }
  printf("\n");
}

//
// The toplogical sequence of vertex indices, if it could be computed
// Followed by the digraph displayed using the adjecency lists
// Adjacency lists are presented in topologic sorted order
//
void GraphTopoSortDisplay(const GraphTopoSort* p) {
  assert(p != NULL);

  // The topological order
  GraphTopoSortDisplaySequence(p);

  if (p->validResult == 0) {
    return;
  }

  // The Digraph
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
    GraphListAdjacents(p->graph, p->vertexSequence[i]);
  }
  printf("\n");
}