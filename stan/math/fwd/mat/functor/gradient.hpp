#ifndef STAN_MATH_FWD_MAT_FUNCTOR_GRADIENT_HPP
#define STAN_MATH_FWD_MAT_FUNCTOR_GRADIENT_HPP

#include <stan/math/fwd/core.hpp>
#include <stan/math/prim/mat/fun/Eigen.hpp>
#include <thread>

namespace stan {
namespace math {

static std::exception_ptr teptr = nullptr;

template <typename T, typename F>
void derivative(const F& f, const Eigen::Matrix<T, Eigen::Dynamic, 1>& x, size_t i, T& deriv) try {
  Eigen::Matrix<fvar<T>, Eigen::Dynamic, 1> x_fvar(x.size());
  for (int k = 0; k < x.size(); ++k)
    x_fvar(k) = fvar<T>(x(k), k == i);
  fvar<T> fx_fvar = f(x_fvar);
  deriv = fx_fvar.d_;
 } catch(...) {
  teptr = std::current_exception();
 }

template <typename T, typename F>
void derivative_with_value(const F& f, const Eigen::Matrix<T, Eigen::Dynamic, 1>& x,
			   size_t i, T& lp, T& deriv) try {
  Eigen::Matrix<fvar<T>, Eigen::Dynamic, 1> x_fvar(x.size());
  for (int k = 0; k < x.size(); ++k)
    x_fvar(k) = fvar<T>(x(k), k == i);
  fvar<T> fx_fvar = f(x_fvar);
  lp = fx_fvar.val_;
  deriv = fx_fvar.d_;
 } catch(...) {
  teptr = std::current_exception();
 }

/**
 * Calculate the value and the gradient of the specified function
 * at the specified argument.
 *
 * <p>The functor must implement
 *
 * <code>
 * fvar<T>
 * operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>&)
 * </code>
 *
 * using only operations that are defined for
 * <code>fvar</code>.  This latter constraint usually
 * requires the functions to be defined in terms of the libraries
 * defined in Stan or in terms of functions with appropriately
 * general namespace imports that eventually depend on functions
 * defined in Stan.
 *
 * <p>Time and memory usage is on the order of the size of the
 * fully unfolded expression for the function applied to the
 * argument, independently of dimension.
 *
 * @tparam F Type of function
 * @param[in] f Function
 * @param[in] x Argument to function
 * @param[out] fx Function applied to argument
 * @param[out] grad_fx Gradient of function at argument
 */
template <typename T, typename F>
void gradient(const F& f, const Eigen::Matrix<T, Eigen::Dynamic, 1>& x, T& fx,
              Eigen::Matrix<T, Eigen::Dynamic, 1>& grad_fx) {
  grad_fx.resize(x.size());

  /*
  for (size_t i = 0; i < x.size(); ++i) {
    if (i == 0)
      derivative_with_value<T, F>(f, x, i, fx, grad_fx[i]);
    else
      derivative<T, F>(f, x, i, grad_fx[i]);
  }
  */
  
  std::vector<std::thread> threads(x.size());

  for (size_t i = 0; i < x.size(); ++i) {
    if (i == 0)
      threads[i] = std::thread(derivative_with_value<T, F>,
			       f, std::ref(x), i, std::ref(fx), std::ref(grad_fx[i]));
    else
      threads[i] = std::thread(derivative<T, F>,
			       f, std::ref(x), i, std::ref(grad_fx[i]));
  }
  
  for (size_t i = 0; i < x.size(); ++i) {
    threads[i].join();
  }

  if (teptr) {
    std::rethrow_exception(teptr);
    teptr = nullptr;
  }
}

}  // namespace math
}  // namespace stan
#endif
