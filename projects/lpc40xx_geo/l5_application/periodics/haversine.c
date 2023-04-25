#include <math.h> //library for sin/cos, etc
#include <stdio.h>

#define PI 3.14159265
#define R 6371.0 // radius of earth in kms

static double convert_degree_to_radians(double degrees) { return degrees * PI / 180.0; }

static double convert_radians_to_degree(double radians) { return radians * (180.0 / PI); }

double calculate_distance_in_meters(double lat1, double lon1, double lat2, double lon2) {
  double dLat = convert_degree_to_radians(lat2 - lat1);
  double dLon = convert_degree_to_radians(lon2 - lon1);
  double a = sin(dLat / 2.0) * sin(dLat / 2.0) + cos(convert_degree_to_radians(lat1)) *
                                                     cos(convert_degree_to_radians(lat2)) * sin(dLon / 2.0) *
                                                     sin(dLon / 2.0);
  double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
  double distance_in_meters = R * c * 1000;
  return distance_in_meters;
}

double calculate_heading(double lat1, double lon1, double lat2, double lon2) {
  double dLon = convert_degree_to_radians(lon2 - lon1);
  double y = sin(dLon) * cos(convert_degree_to_radians(lat2));
  double x = cos(convert_degree_to_radians(lat1)) * sin(convert_degree_to_radians(lat2)) -
             sin(convert_degree_to_radians(lat1)) * cos(convert_degree_to_radians(lat2)) * cos(dLon);
  double bearing = atan2(y, x);
  return fmod(convert_radians_to_degree(bearing) + 360.0, 360.0);
}