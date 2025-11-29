#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "kruskal.h"
#include "distance.h"
#include "dsu.h"
#include "atm.h"

static int cmp_edge_inc(const void *a, const void *b){
    double da = ((Edge*)a)->w;
    double db = ((Edge*)b)->w;
    if(da < db) return -1;
    if(da > db) return 1;
    return 0;
}

Edge *build_all_edges(int n, ATM *atm_list, int *edge_count){
    int m = n*(n-1)/2;
    Edge *edges = (Edge*)malloc(sizeof(Edge)*m);
    int idx=0;
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            double d = haversine_km(atm_list[i].lat, atm_list[i].lon, atm_list[j].lat, atm_list[j].lon);
            edges[idx].u = i; edges[idx].v = j; edges[idx].w = d; idx++;
        }
    }
    *edge_count = m;
    return edges;
}

Edge *kruskal_mst(Edge *edges, int edge_count, int n, int *mst_edges_out){
    Edge *mst = (Edge*)malloc(sizeof(Edge)*(n-1));
    qsort(edges, edge_count, sizeof(Edge), cmp_edge_inc);
    DSU *d = dsu_create(n);
    int added = 0;
    for(int i=0;i<edge_count && added < n-1;i++){
        if(dsu_union(d, edges[i].u, edges[i].v)){
            mst[added++] = edges[i];
        }
    }
    dsu_free(d);
    *mst_edges_out = added;
    return mst;
}

void free_edges(Edge *e){ free(e); }
