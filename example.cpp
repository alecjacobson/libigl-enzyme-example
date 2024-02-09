// Are these all still necessary?
// From: https://github.com/EnzymeAD/Enzyme/blob/1e4a7ba11825e2a9f50927a6602b311915a0514a/enzyme/test/Integration/eigensumsqdyn.cpp
#define EIGEN_NO_AUTOMATIC_RESIZING 1
#define EIGEN_DONT_ALIGN 1
#define EIGEN_NO_DEBUG 1
#define EIGEN_UNROLLING_LIMIT 0
#define EIGEN_DONT_VECTORIZE 1

#include <Eigen/Core>

extern "C" {
  extern double __enzyme_autodiff(
    void*, 
    const Eigen::MatrixXd* __restrict V, 
    const Eigen::MatrixXd* __restrict Vp);
}

#include <igl/opengl/glfw/Viewer.h>
#include <igl/doublearea.h>
#include <igl/read_triangle_mesh.h>
#include <cstdio>

// global for now because I'm not sure what enzyme will do with ints
Eigen::MatrixXi F;

__attribute__((noinline))
static double total_surface_area( const Eigen::MatrixXd * __restrict pointer_V)
{
  // get input as reference
  const auto & V = *pointer_V;

  Eigen::VectorXd A;
  igl::doublearea(V, F, A);
  return 0.5*A.sum();
}

int main(int argc, char *argv[])
{
   // read ../decimated-knight.off or argv[1]
  Eigen::MatrixXd V;
  igl::read_triangle_mesh(
      argc>1?argv[1]:"../decimated-knight.obj", V, F);


  // Compute f = total surface area using our autodiff types
  double f = total_surface_area(&V);
  // Compute the gradient of f with respect to U into a matrix same size as U
  Eigen::MatrixXd dfdU(V.rows(),V.cols());
  __enzyme_autodiff((void*)total_surface_area, &V, &dfdU);



  // Draw the mesh
  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);
  viewer.data().set_face_based(true);

  // Draw dfdU as little white lines from each vertex
  const double scale = 
    0.1 * 
    (V.colwise().maxCoeff()-V.colwise().minCoeff()).norm()/
    dfdU.rowwise().norm().maxCoeff();
  viewer.data().add_edges(V,(V + dfdU*scale).eval(), Eigen::RowVector3d(1,1,1) );
  viewer.data().line_width = 1;
  viewer.launch();
}
