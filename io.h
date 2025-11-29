#ifndef IO_H
#define IO_H
#include "atm.h"
int read_atm_csv(const char *path, ATM **out_list);
void write_clusters_csv(const char *path, ATM *atms, int n, int *cluster_ids);
void print_summary_and_recommendations(ATM *atms, int n, int k, int *cluster_ids, double *mst_total_dist, double *cluster_route_lengths);
#endif
