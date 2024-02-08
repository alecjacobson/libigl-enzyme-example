#include <cstdio>
extern double __enzyme_autodiff(void*, double);
double square(double x) {
    return x * x;
}
double dsquare(double x) {
    // This returns the derivative of square or 2 * x
    return __enzyme_autodiff((void*) square, x);
}

#include <igl/opengl/glfw/Viewer.h>

int main() {
  // A cube
  Eigen::MatrixXd V(8,3);
  V << 0,0,0,
       1,0,0,
       1,1,0,
       0,1,0,
       0,0,1,
       1,0,1,
       1,1,1,
       0,1,1;
  Eigen::MatrixXi F(12,3);
  F << 0,1,2,
       0,2,3,
       0,1,4,
       1,5,4,
       1,2,5,
       2,6,5,
       2,3,6,
       3,7,6,
       3,0,7,
       0,4,7,
       4,5,6,
       4,6,7;

  for(double i=1; i<5; i++)
  {
    std::printf("square(%f)=%f, dsquare(%f)=%f\n", i, square(i), i, dsquare(i));
  }

  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);
  viewer.data().set_face_based(true);
  viewer.launch();
}
