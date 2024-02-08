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


template <typename DerivedV, typename DerivedF, typename DeriveddblA>
IGL_INLINE void doublearea(
  const Eigen::MatrixBase<DerivedV> & V,
  const Eigen::MatrixBase<DerivedF> & F,
  Eigen::PlainObjectBase<DeriveddblA> & dblA);

template <typename DerivedV, typename DerivedF, typename DeriveddblA>
IGL_INLINE void doublearea(
  const Eigen::MatrixBase<DerivedV> & V,
  const Eigen::MatrixBase<DerivedF> & F,
  Eigen::PlainObjectBase<DeriveddblA> & dblA)
{
  // quads are handled by a specialized function
  if (F.cols() == 4) return igl::doublearea_quad(V,F,dblA);

  const int dim = V.cols();
  // Only support triangles
  assert(F.cols() == 3);
  const size_t m = F.rows();
  // Compute edge lengths
  Eigen::Matrix<typename DerivedV::Scalar, Eigen::Dynamic, 3> l;

  // Projected area helper
  const auto & proj_doublearea =
    [&V,&F](const int x, const int y, const int f)
    ->typename DerivedV::Scalar
  {
    auto rx = V(F(f,0),x)-V(F(f,2),x);
    auto sx = V(F(f,1),x)-V(F(f,2),x);
    auto ry = V(F(f,0),y)-V(F(f,2),y);
    auto sy = V(F(f,1),y)-V(F(f,2),y);
    return rx*sy - ry*sx;
  };

  switch(dim)
  {
    case 3:
    {
      dblA = DeriveddblA::Zero(m,1);
      for(size_t f = 0;f<m;f++)
      {
        for(int d = 0;d<3;d++)
        {
          const auto dblAd = proj_doublearea(d,(d+1)%3,f);
          dblA(f) += dblAd*dblAd;
        }
      }
      dblA = dblA.array().sqrt().eval();
      break;
    }
    // Uncommenting this causes the autodiff to fail
    //case 2:
    //{
    //  dblA.resize(m,1);
    //  for(size_t f = 0;f<m;f++)
    //  {
    //    dblA(f) = proj_doublearea(0,1,f);
    //  }
    //  break;
    //}

    // Uncommenting this causes the autodiff to fail
    //default:
    //{
    //  igl::edge_lengths(V,F,l);
    //  return doublearea(l,0.,dblA);
    //}
  }
}

// global for now because I'm not sure what enzyme will do with ints
Eigen::MatrixXi F;



__attribute__((noinline))
static double total_surface_area( const Eigen::MatrixXd * __restrict pointer_V)
{
  // get input as reference
  const auto & V = *pointer_V;

  // Why does this fail?
  Eigen::VectorXd A;
  //igl::doublearea(V, F, A);
  doublearea(V, F, A);
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
  std::printf("f: %g\n",f);
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
