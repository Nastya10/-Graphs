#include "hwg.h"

int main() {
    int n = 0, i = 0, res_scan = 0;
    struct edge *graph_table;

    input(&n, &graph_table, &i);
    test_input(graph_table, n, i);
    qsort(graph_table, i, sizeof(struct edge), compare);

    printf ("%d\n", kruskal(graph_table, n, i));

    free(graph_table);

    return 0;
}