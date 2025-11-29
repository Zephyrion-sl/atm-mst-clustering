#include <math.h>
#include "distance.h"
#define R_EARTH_KM 6371.0
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static double deg2rad(double deg){ return deg * M_PI / 180.0; }

double haversine_km(double lat1, double lon1, double lat2, double lon2){
    double dlat = deg2rad(lat2 - lat1);
    double dlon = deg2rad(lon2 - lon1);
    lat1 = deg2rad(lat1);
    lat2 = deg2rad(lat2);
    double a = sin(dlat/2.0)*sin(dlat/2.0) + cos(lat1)*cos(lat2)*sin(dlon/2.0)*sin(dlon/2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1-a));
    return R_EARTH_KM * c;
}
