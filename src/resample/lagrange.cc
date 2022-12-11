#include "resample/lagrange.h"
#include <cassert>

namespace lagrange {

double basis(int j, double x, const double x_values[], int num_x) {
  double l_j = 1;
  double r;
  assert( 0 <= j && j < num_x && "j is in bounds" );
  if (num_x  == 0 || num_x == 1) return 1.;
  for (int m = 0; m < num_x; m++) {
    if (m != j) {
      r = (x - x_values[m])/(x_values[j] - x_values[m]);
      l_j *= r;
    }
  }
  return l_j;
}

}  // namespace lagrange
