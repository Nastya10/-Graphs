#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct edge {
    int v1; int v2;
    int w;
};

int compare (const void *x1, const void *x2);
int min_edge (struct edge *graph_table, const int lines);
int kruskal (struct edge *graph_table, const int n, const int lines);
int loop_graph (struct edge *graph_table, const int n, const int lines);
int paint (struct edge *graph_table, const int n, const int parent, const int current, int* peaks, const int lines);
