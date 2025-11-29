#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "distance.h"

int read_atm_csv(const char *path, ATM **out_list){
    FILE *f = fopen(path, "r");
    if(!f) return -1;
    char line[256];
    int capacity = 64, count = 0;
    ATM *arr = malloc(sizeof(ATM)*capacity);
    // skip header
    if(!fgets(line, sizeof(line), f)){ fclose(f); free(arr); return -2; }
    while(fgets(line, sizeof(line), f)){
        if(count >= capacity){ capacity *= 2; arr = realloc(arr, sizeof(ATM)*capacity); }
        char id[64]; double lat, lon;
        if(sscanf(line, "%31[^,],%lf,%lf", id, &lat, &lon) >= 3){
            strncpy(arr[count].id, id, 31); arr[count].id[31]=0;
            arr[count].lat = lat; arr[count].lon = lon;
            count++;
        }
    }
    fclose(f);
    *out_list = arr;
    return count;
}

void write_clusters_csv(const char *path, ATM *atms, int n, int *cluster_ids){
    FILE *f = fopen(path, "w");
    if(!f) return;
    fprintf(f, "atm_id,lat,lon,cluster\n");
    for(int i=0;i<n;i++){
        fprintf(f, "%s,%.6f,%.6f,%d\n", atms[i].id, atms[i].lat, atms[i].lon, cluster_ids[i]);
    }
    fclose(f);
}

void print_summary_and_recommendations(ATM *atms, int n, int k, int *cluster_ids, double *mst_total_dist, double *cluster_route_lengths){
    printf("\n--- CLUSTER SUMMARY ---\n");
    int *counts = calloc(k, sizeof(int));
    for(int i=0;i<n;i++) counts[cluster_ids[i]]++;
    printf("Clusters requested: %d\n", k);
    for(int c=0;c<k;c++){
        printf("Cluster %d: %d ATMs, approx route length: %.3f km\n", c, counts[c], cluster_route_lengths[c]);
    }
    printf("Total MST length: %.3f km\n", *mst_total_dist);
    printf("\nRecommendations:\n");
    for(int c=0;c<k;c++){
        if(counts[c] >= 4) printf("- Cluster %d is dense (>=4 ATMs). Assign dedicated replenishment vehicle.\n", c);
        if(counts[c] == 1) printf("- Cluster %d has single ATM (outlier). Investigate accessibility.\n", c);
        if(cluster_route_lengths[c] > 6.0) printf("- Cluster %d route is long (>6km). Consider splitting into subzones.\n", c);
    }
    printf("- Check pairs with distance < 0.2 km for redundancy (possible merge).\n");
    free(counts);
}
