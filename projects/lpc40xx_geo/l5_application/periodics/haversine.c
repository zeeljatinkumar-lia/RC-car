#include <math.h> //library for sin/cos, etc
#include <stdio.h>

#define M_PI 3.14159265
#define R 6371.0 // radius of earth in kms

static double convert_degree_to_Radians(double degrees) { return degrees * M_PI / 180.0; }

static double convert_Radians_to_degree(double radians) { return radians * (180.0 / M_PI); }

double calculate_distance(double lat1, double lon1, double lat2, double lon2) {
  double dLat = convert_degree_to_Radians(lat2 - lat1);
  double dLon = convert_degree_to_Radians(lon2 - lon1);
  double a = sin(dLat / 2.0) * sin(dLat / 2.0) + cos(convert_degree_to_Radians(lat1)) *
                                                     cos(convert_degree_to_Radians(lat2)) * sin(dLon / 2.0) *
                                                     sin(dLon / 2.0);
  double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
  double distance = R * c;
  return distance;
}

double calculate_heading(double lat1, double lon1, double lat2, double lon2) {
  double dLon = convert_degree_to_Radians(lon2 - lon1);
  double y = sin(dLon) * cos(convert_degree_to_Radians(lat2));
  double x = cos(convert_degree_to_Radians(lat1)) * sin(convert_degree_to_Radians(lat2)) -
             sin(convert_degree_to_Radians(lat1)) * cos(convert_degree_to_Radians(lat2)) * cos(dLon);
  double bearing = atan2(y, x);
  return fmod(convert_Radians_to_degree(bearing) + 360.0, 360.0);
}
/*

int main() {
    double lat1 = 37.198406;
    double lon1 = -121.542557;
    double lat2 = 47.198404;
    double lon2 = -111.542557;

    double distance = calculate_distance(lat1, lon1, lat2, lon2);
    double bearing = calculate_bearing(lat1, lon1, lat2, lon2);

    printf("Distance: %.2f km\n", dist);
    printf("Heading: %.2f degrees\n", bearing);

    return 0;
}
*/