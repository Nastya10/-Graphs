#include "hwg.h"

int main() {
    int n = 0, i = 0;
    struct edge *graph_table;

    scanf("%d", &n);
    graph_table = (struct edge *) calloc(n*n, sizeof(struct edge));

    while (scanf("%d %d %d", &graph_table[i].v1, &graph_table[i].v2, &graph_table[i].w) == 3)
        ++i;

    qsort(graph_table, i, sizeof(struct edge), compare);

    printf ("%d\n", kruskal(graph_table, n, i));

    free(graph_table);

    return 0;
}