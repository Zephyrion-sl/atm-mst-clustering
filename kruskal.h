#ifndef KRUSKAL_H
#define KRUSKAL_H
#include "atm.h"

typedef struct {
    int u, v;
    double w;
} Edge;
Edge *build_all_edges(int n, ATM *atm_list, int *edge_count);
Edge *kruskal_mst(Edge *edges, int edge_count, int n, int *mst_edges_out);
void free_edges(Edge *e);
#endif
