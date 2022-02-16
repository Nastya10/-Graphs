#include "hwg.h"

/*--------------------------------------------------------------------------
Функции loop_graph, paint и paint_top отвечают за проверку наличия
циклов в графе по таблице рёбер. Создаётся массив peaks с номерами
вершин, который изначально заполнен нулями. В функцию paint передаётся
текущий элемент и его родитель. Если в таблице рёбер находится сочитание
из текущей вершины и ещё одной, не равной родителю, найденная вершина 
отмечается в массиве peaks минус единицей. И рекурсивно вызываетс алгоритм 
для неё. Если какая-то вершина уже была обозначена минус единицей, то
мы нашли цикл графа. В программе lines - количество строк в таблице рёбер.
Предпологаю, что сложность описанного алгоритма будет lines^2.

Функция kruskal непосредственно отвечает за алгоритм Краскала. Там 
создаётся табличка рёбер graph_table_res, в которую записываются сочетания
вершин с ребром наименьшего веса. Дальше для этой таблички вызываются 
функции, описанные выше, и проверяют на наличие цикла полученную табличку
рёбер. Если цикл есть, то новая пара вершин с минимальным весом ребра 
записывается на место предыдущй пары. Если нет, то добавляется в итоговую
сумму вес записанного ребра и дописывается новая строка с ребром
минимального веса.
--------------------------------------------------------------------------*/

static int paint_top (struct edge *graph_table, const int current, int* peaks, const int lines, const int top, int *flag);

int kruskal (struct edge *graph_table, const int n, const int lines) {
    int i = 0, n_edge_res = 0, min = 0, res = 0;
    struct edge *graph_table_res;

    graph_table_res = (struct edge *) calloc(lines, sizeof(struct edge));

    while (i < lines) {
        min = min_edge(graph_table, lines);

        graph_table_res[n_edge_res].v1 = graph_table[min].v1;
        graph_table_res[n_edge_res].v2 = graph_table[min].v2;
        graph_table_res[n_edge_res].w  = graph_table[min].w;

        if (loop_graph(graph_table_res, n, n_edge_res + 1) == 0) {
            ++n_edge_res;
            res += graph_table[min].w;
        }

        graph_table[min].w = -1;

        ++i;
    }

    free(graph_table_res);

    return res;
}

int min_edge (struct edge *graph_table, const int lines) {
    int i = 0;

    for (; i < lines; ++i) {
        if (graph_table[i].w != -1) {
            break;
        }
    }
    
    return i;
}

int loop_graph (struct edge *graph_table, const int n, const int lines) {
    int res = 0;
    int *peaks = (int *) calloc(n, sizeof(int));

    peaks[graph_table[0].v1] = -1;
    res = paint(graph_table, -1, graph_table[0].v1, peaks, lines);

    free(peaks);

    return res;

}

int paint (struct edge *graph_table, const int parent, const int current, int* peaks, const int lines) {
    int flag = 0;

    for (int i = 0; i < lines; ++i) {
        if (graph_table[i].v1 == current && graph_table[i].v2 != parent) {
            if (paint_top (graph_table, current, peaks, lines, graph_table[i].v2, &flag))
                return 1;
        }
        if (graph_table[i].v2 == current && graph_table[i].v1 != parent) {
            if (paint_top (graph_table, current, peaks, lines, graph_table[i].v1, &flag))
                return 1;
        }
    }
    return flag;
}

static int paint_top (struct edge *graph_table, const int current, int* peaks, const int lines, const int top, int *flag) {

    if (peaks[top] != -1) {
        peaks[top] = -1;
        if (paint(graph_table, current, top, peaks, lines) == 1) {
            *flag = 1;
        }
        return 0;
    }
    else {
        return 1;
    }
} 

int compare (const void *x1, const void *x2) {
    return ((*(struct edge *)x1).w - (*(struct edge *)x2).w);
}

void test_input (struct edge *graph_table, const int n, const int lines) {
    int n_tops = 0, flag1 = 0, flag2 = 0;
    int *tops, *peaks;

    peaks = (int *) calloc(n, sizeof(int));
    tops = (int *) calloc(2 * lines, sizeof(int));

    for (int i = 0; i < lines; ++i) {

        if (graph_table[i].w < 0) {
            printf("Error input: You entered a negative weight\n");
            free(tops);
            free(peaks);
            free(graph_table);
            abort();
        }

        for (int j = 0; j < n_tops; ++j) {
            if (graph_table[i].v1 == tops[j]) {
                flag1 = 1;
            }
            if (graph_table[i].v2 == tops[j]) {
                flag2 = 1;
            }
            if (graph_table[i].v1 == graph_table[i].v2) {
                printf("Error input: You entered an edge between the same vertex\n");
                free(tops);
                free(peaks);
                free(graph_table);
                abort();
            }
        }
        if (flag1 == 0) {
            tops[n_tops] = graph_table[i].v1;
            ++n_tops;
        }
        if (flag2 == 0) {
            tops[n_tops] = graph_table[i].v2;
            ++n_tops;
        }

        flag1 = 0;
        flag2 = 0;
    }

    free(tops);

    if (n_tops > n) {
        printf("Error input: The number of entered vertices is greater %d\n", n);
        free(peaks);
        free(graph_table);
        abort();
    }

    connected_grapht(graph_table, -1, graph_table[0].v1, peaks, lines);

    for (int i = 0; i < n; ++i) {
        if (peaks[i] != -1) {
            printf("Error input: You entered an unrelated graph\n");
            free(peaks);
            free(graph_table);
            abort();
        }
    }

}

void connected_grapht (struct edge *graph_table, const int parent, const int current, int* peaks, const int lines) {

    for (int i = 0; i < lines; ++i) {
        if (graph_table[i].v1 == current && graph_table[i].v2 != parent) {
            if (peaks[graph_table[i].v2] != -1) {
                peaks[graph_table[i].v2] = -1;
                connected_grapht(graph_table, current, graph_table[i].v2, peaks, lines);
            }
        }
        if (graph_table[i].v2 == current && graph_table[i].v1 != parent) {
            if (peaks[graph_table[i].v1] != -1) {
                peaks[graph_table[i].v1] = -1;
                connected_grapht(graph_table, current, graph_table[i].v1, peaks, lines);
            }
        }
    }
}

void input (int *n, struct edge **graph_table, int *lines) {
    int res_scan = 0;

    res_scan = scanf("%d", n);
    if (res_scan != 1) {
        printf("Error input: You did not enter the number of vertices\n");
        abort();
    }

    *graph_table = (struct edge *) calloc((*n) * (*n), sizeof(struct edge));

    while (scanf("%d %d %d", &(*graph_table)[(*lines)].v1, &(*graph_table)[(*lines)].v2, &(*graph_table)[(*lines)].w) == 3) {
        ++(*lines);
    }

    if ((*lines) == 0) {
        printf("Error input: You didn't enter edges\n");
        abort();
    }
}