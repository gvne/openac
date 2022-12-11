//
// This work is mostly based on the Jonah Miller (jonah.maxwell.miller@gmail.com)
// lagrange-interplation (https://github.com/Yurlungur/lagrange-interpolation)
//
// This code reduces the use case to a single dimension but allows any input
// type
//

#ifndef RESAMPLE_LAGRANGE_H_
#define RESAMPLE_LAGRANGE_H_

namespace lagrange {

double basis(int j, double x, const double x_values[], int num_x);

template <typename T>
T interp_1D(double x, const double x_values[], int num_x, const T y_values[]) {
  double out = 0;
  for (int j = 0; j < num_x; j++) {
    out += y_values[j] * basis(j,x,x_values,num_x);
  }
  return static_cast<T>(out);
}

}  // namespace lagrange


#endif  // RESAMPLE_LAGRANGE_H_
