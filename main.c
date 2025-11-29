#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "atm.h"
#include "io.h"
#include "kruskal.h"
#include "distance.h"
#include "dsu.h"

// Simple structure for MST edges and cluster extraction
int cmp_edge_desc(const void *a, const void *b){
    double da = ((Edge*)a)->w;
    double db = ((Edge*)b)->w;
    if(da < db) return 1;
    if(da > db) return -1;
    return 0;
}

int main(int argc, char **argv){
    char *input = NULL; char *out = NULL;
    int k = 4;
    int opt;
    static struct option long_options[] = {
        {"input", required_argument, 0, 'i'},
        {"clusters", required_argument, 0, 'k'},
        {"output", required_argument, 0, 'o'},
        {0,0,0,0}
    };
    while((opt = getopt_long(argc, argv, "i:k:o:", long_options, NULL)) != -1){
        switch(opt){
            case 'i': input = strdup(optarg); break;
            case 'k': k = atoi(optarg); break;
            case 'o': out = strdup(optarg); break;
            default: fprintf(stderr, "Usage: %s --input file --clusters k --output file\n", argv[0]); return 1;
        }
    }
    if(!input){ fprintf(stderr,"--input required\n"); return 1; }
    if(!out) out = strdup("result_clusters.csv");

    ATM *atms = NULL;
    int n = read_atm_csv(input, &atms);
    if(n <= 0){ fprintf(stderr,"failed to read ATMs (code %d)\n", n); return 2; }

    int edge_count;
    Edge *edges = build_all_edges(n, atms, &edge_count);
    int mst_edge_count;
    Edge *mst = kruskal_mst(edges, edge_count, n, &mst_edge_count);
    // compute total MST length
    double total_mst = 0.0;
    for(int i=0;i<mst_edge_count;i++) total_mst += mst[i].w;

    // To form k clusters: remove k-1 largest edges from MST
    // Sort MST edges descending and remove first k-1 (i.e., mark them as separators)
    Edge *mst_copy = malloc(sizeof(Edge)*mst_edge_count);
    memcpy(mst_copy, mst, sizeof(Edge)*mst_edge_count);
    qsort(mst_copy, mst_edge_count, sizeof(Edge), cmp_edge_desc);
    int separators = (k-1 < mst_edge_count) ? (k-1) : 0;

    // Use DSU to union MST edges except top separators
    DSU *uf = dsu_create(n);
    for(int i=0;i<mst_edge_count;i++){
        int skip = 0;
        for(int s=0;s<separators;s++){
            if( (mst_copy[s].u==mst[i].u && mst_copy[s].v==mst[i].v && mst_copy[s].w==mst[i].w) ||
                 (mst_copy[s].u==mst[i].v && mst_copy[s].v==mst[i].u && mst_copy[s].w==mst[i].w) ){
                skip = 1; break;
            }
        }
        if(!skip) dsu_union(uf, mst[i].u, mst[i].v);
    }

    int *cluster_ids = malloc(sizeof(int)*n);
    // compress representatives to 0..k-1 map
    int *rep_map = malloc(sizeof(int)*n);
    for(int i=0;i<n;i++) rep_map[i] = -1;
    int next_cluster = 0;
    for(int i=0;i<n;i++){
        int r = dsu_find(uf, i);
        if(rep_map[r] == -1) rep_map[r] = next_cluster++;
        cluster_ids[i] = rep_map[r];
    }
    int actual_k = next_cluster;

    // compute per-cluster route length (sum of MST edges inside cluster)
    double *cluster_route_lengths = calloc(actual_k, sizeof(double));
    for(int i=0;i<mst_edge_count;i++){
        int cu = cluster_ids[mst[i].u];
        int cv = cluster_ids[mst[i].v];
        if(cu == cv) cluster_route_lengths[cu] += mst[i].w;
    }

    // write output CSV
    write_clusters_csv(out, atms, n, cluster_ids);
    // print summary & recommendations
    print_summary_and_recommendations(atms, n, actual_k, cluster_ids, &total_mst, cluster_route_lengths);

    // cleanup
    free(edges); free(mst); free(mst_copy);
    dsu_free(uf); free(cluster_ids); free(rep_map); free(cluster_route_lengths);
    free(atms); free(input); free(out);
    return 0;
}
