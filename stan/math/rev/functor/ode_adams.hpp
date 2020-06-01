#ifndef STAN_MATH_REV_FUNCTOR_ODE_ADAMS_HPP
#define STAN_MATH_REV_FUNCTOR_ODE_ADAMS_HPP

#include <stan/math/rev/meta.hpp>
#include <stan/math/rev/functor/cvodes_integrator.hpp>
#include <ostream>
#include <vector>

namespace stan {
namespace math {
template <typename F, typename T_initial, typename T_t0, typename T_ts,
          typename... T_Args>
std::vector<Eigen::Matrix<stan::return_type_t<T_initial, T_t0, T_ts, T_Args...>,
                          Eigen::Dynamic, 1>>
ode_adams_tol_sens_error(const F& f,
                         const Eigen::Matrix<T_initial, Eigen::Dynamic, 1>& y0,
                         const T_t0& t0, const std::vector<T_ts>& ts,
                         double rtol, double atol, double rtols,
                         const std::vector<double>& atols,
                         long int max_num_steps, std::ostream* msgs,
                         const T_Args&... args) {
  cvodes_integrator<CV_ADAMS, F, T_initial, T_t0, T_ts, T_Args...> integrator(
      f, y0, t0, ts, rtol, atol, true, max_num_steps, msgs, args...);

  return integrator(rtols, atols);
}

template <typename F, typename T_initial, typename T_t0, typename T_ts,
          typename... T_Args>
std::vector<Eigen::Matrix<stan::return_type_t<T_initial, T_t0, T_ts, T_Args...>,
                          Eigen::Dynamic, 1>>
ode_adams_tol_error(const F& f,
                    const Eigen::Matrix<T_initial, Eigen::Dynamic, 1>& y0,
                    const T_t0& t0, const std::vector<T_ts>& ts,
                    double relative_tolerance, double absolute_tolerance,
                    long int max_num_steps, std::ostream* msgs,
                    const T_Args&... args) {
  cvodes_integrator<CV_ADAMS, F, T_initial, T_t0, T_ts, T_Args...> integrator(
      f, y0, t0, ts, relative_tolerance, absolute_tolerance, true,
      max_num_steps, msgs, args...);

  return integrator();
}

/**
 * Solve the ODE initial value problem y' = f(t, y), y(t0) = y0 at a set of
 * times, { t1, t2, t3, ... } using the stiff backward differentiation formula
 * Adams-Moulton solver from CVODES.
 *
 * \p f must define an operator() with the signature as:
 *   template<typename T_t, typename T_y, typename... T_Args>
 *   Eigen::Matrix<stan::return_type_t<T_t, T_y, T_Args...>, Eigen::Dynamic, 1>
 *     operator()(const T_t& t, const Eigen::Matrix<T_y, Eigen::Dynamic, 1>& y,
 *     std::ostream* msgs, const T_Args&... args);
 *
 * t is the time, y is the state, msgs is a stream for error messages, and args
 * are optional arguments passed to the ODE solve function (which are passed
 * through to \p f without modification).
 *
 * @tparam F Type of ODE right hand side
 * @tparam T_0 Type of initial time
 * @tparam T_ts Type of output times
 * @tparam T_Args Types of pass-through parameters
 *
 * @param f Right hand side of the ODE
 * @param y0 Initial state
 * @param t0 Initial time
 * @param ts Times at which to solve the ODE at. All values must be sorted and
 *   not less than t0.
 * @param relative_tolerance Relative tolerance passed to CVODES
 * @param absolute_tolerance Absolute tolerance passed to CVODES
 * @param max_num_steps Upper limit on the number of integration steps to
 *   take between each output (error if exceeded)
 * @param[in, out] msgs the print stream for warning messages
 * @param args Extra arguments passed unmodified through to ODE right hand side
 * @return Solution to ODE at times \p ts
 */
template <typename F, typename T_initial, typename T_t0, typename T_ts,
          typename... T_Args>
std::vector<Eigen::Matrix<stan::return_type_t<T_initial, T_t0, T_ts, T_Args...>,
                          Eigen::Dynamic, 1>>
ode_adams_tol(const F& f, const Eigen::Matrix<T_initial, Eigen::Dynamic, 1>& y0,
              const T_t0& t0, const std::vector<T_ts>& ts,
              double relative_tolerance, double absolute_tolerance,
              long int max_num_steps, std::ostream* msgs,
              const T_Args&... args) {
  cvodes_integrator<CV_ADAMS, F, T_initial, T_t0, T_ts, T_Args...> integrator(
      f, y0, t0, ts, relative_tolerance, absolute_tolerance, false,
      max_num_steps, msgs, args...);

  return integrator();
}

/**
 * Solve the ODE initial value problem y' = f(t, y), y(t0) = y0 at a set of
 * times, { t1, t2, t3, ... } using the non-stiff Adams-Moulton
 * solver in CVODES with a relative tolerance of 1e-10, an absolute
 * tolerance of 1e-10, and taking a maximum of 1e8 steps.
 *
 * \p f must define an operator() with the signature as:
 *   template<typename T_t, typename T_y, typename... T_Args>
 *   Eigen::Matrix<stan::return_type_t<T_t, T_y, T_Args...>, Eigen::Dynamic, 1>
 *     operator()(const T_t& t, const Eigen::Matrix<T_y, Eigen::Dynamic, 1>& y,
 *     std::ostream* msgs, const T_Args&... args);
 *
 * t is the time, y is the state, msgs is a stream for error messages, and args
 * are optional arguments passed to the ODE solve function (which are passed
 * through to \p f without modification).
 *
 * @tparam F Type of ODE right hand side
 * @tparam T_0 Type of initial time
 * @tparam T_ts Type of output times
 * @tparam T_Args Types of pass-through parameters
 *
 * @param f Right hand side of the ODE
 * @param y0 Initial state
 * @param t0 Initial time
 * @param ts Times at which to solve the ODE at. All values must be sorted and
 *   not less than t0.
 * @param relative_tolerance Relative tolerance passed to CVODES
 * @param absolute_tolerance Absolute tolerance passed to CVODES
 * @param max_num_steps Upper limit on the number of integration steps to
 *   take between each output (error if exceeded)
 * @param[in, out] msgs the print stream for warning messages
 * @param args Extra arguments passed unmodified through to ODE right hand side
 * @return Solution to ODE at times \p ts
 */
template <typename F, typename T_initial, typename T_t0, typename T_ts,
          typename... T_Args>
std::vector<Eigen::Matrix<stan::return_type_t<T_initial, T_t0, T_ts, T_Args...>,
                          Eigen::Dynamic, 1>>
ode_adams(const F& f, const Eigen::Matrix<T_initial, Eigen::Dynamic, 1>& y0,
          const T_t0& t0, const std::vector<T_ts>& ts, std::ostream* msgs,
          const T_Args&... args) {
  double relative_tolerance = 1e-10;
  double absolute_tolerance = 1e-10;
  long int max_num_steps = 1e8;

  return ode_adams_tol(f, y0, t0, ts, relative_tolerance, absolute_tolerance,
                       max_num_steps, msgs, args...);
}

}  // namespace math
}  // namespace stan
#endif