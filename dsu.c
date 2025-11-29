#include <stdlib.h>
#include "dsu.h"
DSU *dsu_create(int n){
    DSU *d = (DSU*)malloc(sizeof(DSU));
    d->n = n;
    d->parent = (int*)malloc(n * sizeof(int));
    d->rank = (int*)malloc(n * sizeof(int));
    for(int i=0;i<n;i++){ d->parent[i]=i; d->rank[i]=0;}
    return d;
}
void dsu_free(DSU *d){ free(d->parent); free(d->rank); free(d); }
int dsu_find(DSU *d, int x){
    if(d->parent[x]!=x) d->parent[x] = dsu_find(d, d->parent[x]);
    return d->parent[x];
}
int dsu_union(DSU *d, int x, int y){
    int rx = dsu_find(d,x), ry = dsu_find(d,y);
    if(rx==ry) return 0;
    if(d->rank[rx] < d->rank[ry]) d->parent[rx] = ry;
    else if(d->rank[ry] < d->rank[rx]) d->parent[ry] = rx;
    else { d->parent[ry] = rx; d->rank[rx]++; }
    return 1;
}
