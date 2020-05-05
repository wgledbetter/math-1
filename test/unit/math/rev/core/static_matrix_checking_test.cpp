#include <stan/math/rev.hpp>
#include <stan/math/prim.hpp>
#include <gtest/gtest.h>
#include <vector>

TEST(MathRev, TestVarEigen) {
  using stan::math::var_type;
  using stan::math::var;
  Eigen::Matrix<double, -1, -1> x_vals = Eigen::MatrixXd::Random(5, 5);
  Eigen::Matrix<double, -1, -1> y_vals = Eigen::MatrixXd::Random(5, 5);
  var_type<Eigen::Matrix<double, -1, -1>> adder(y_vals);
  var_type<Eigen::Matrix<double, -1, -1>> x(x_vals);
  var_type<Eigen::Matrix<double, -1, -1>> y(y_vals);
  var_type<double> divider(10);
  auto z = (x * y) / divider + adder;
  auto zz = stan::math::sum(z) / divider;
  puts("Static Matrix:");
  std::cout << "static vals: \n" << zz.val() << "\n";
  std::cout << "static adj: \n" << zz.adj() << "\n";
  zz.grad();
  std::cout << "static grad vals: \n" << zz.val() << "\n";
  std::cout << "static grad adj: \n" << zz.adj() << "\n";
  puts("---------");
  puts("Dynamic Matrix:");
  Eigen::Matrix<var, -1, -1> x_dyn = x_vals;
  Eigen::Matrix<var, -1, -1> y_dyn = y_vals;
  Eigen::Matrix<var, -1, -1> adder_dyn = y_vals;
  Eigen::Matrix<var, -1, -1> z_dyn = (x_dyn * y_dyn) / divider + adder_dyn;
  var zz_dyn = z_dyn.sum();
  std::cout << "dynamic vals: \n" << zz_dyn.val() << "\n";
  std::cout << "dynamic adj: \n" << zz_dyn.adj() << "\n";
  zz_dyn.grad();
  std::cout << "dynamic grad vals: \n" << zz_dyn.val() << "\n";
  std::cout << "dynamic grad adj: \n" << zz_dyn.adj() << "\n";

}