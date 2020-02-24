#ifndef STAN_MATH_PRIM_META_IS_EIGEN_BASE_HPP
#define STAN_MATH_PRIM_META_IS_EIGEN_BASE_HPP

#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/meta/bool_constant.hpp>
#include <stan/math/prim/meta/disjunction.hpp>
#include <type_traits>

namespace stan {

namespace internal {
/*
 * Underlying implimenation to check if a type is derived from EigenBase
 */
template <typename T>
struct is_eigen_base
    : std::integral_constant<bool,
                             std::is_base_of<Eigen::EigenBase<std::decay_t<T>>,
                              std::decay_t<T>>::value> {};

template <typename T>
struct is_eigen_base<Eigen::EigenBase<T>> : std::true_type {};

}  // namespace internal

namespace internal {
/*
 * Underlying implimenation to check if a type is derived from EigenBase
 */
template <typename T>
struct is_eigen_dense_base
    : std::integral_constant<bool,
                             std::is_base_of<Eigen::DenseBase<T>, T>::value> {};

template <typename T>
struct is_eigen_dense_base<Eigen::DenseBase<T>> : std::true_type {};

}  // namespace internal

namespace internal {
/*
 * Underlying implimenation to check if a type is derived from EigenBase
 */
template <typename T>
struct is_eigen_matrix_base
    : std::integral_constant<bool,
                             std::is_base_of<Eigen::MatrixBase<T>, T>::value> {
};

template <typename T>
struct is_eigen_matrix_base<Eigen::MatrixBase<T>> : std::true_type {};

}  // namespace internal

/*
 * Checks whether type T is derived from EigenBase. If true this will have a
 * static member function named value with a type of true, else value is false.
 */
template <typename T>
struct is_eigen<
    T, std::enable_if_t<internal::is_eigen_base<std::decay_t<T>>::value>>
    : std::true_type {};

namespace internal {
template <typename T, typename Enable = void>
struct is_eigen_matrix_impl : std::false_type {};
template <typename T>
struct is_eigen_matrix_impl<
    T, std::enable_if_t<internal::is_eigen_matrix_base<T>::value>>
    : bool_constant<T::RowsAtCompileTime != 1 && T::ColsAtCompileTime != 1> {};

}  // namespace internal

/*
 * Checks whether type T is derived from Eigen::MatrixBase and has columns and
 * rows not equal to 1. If true this will have a
 * static member function named value with a type of true, else value is false.
 */
template <typename T>
struct is_eigen_matrix : internal::is_eigen_matrix_impl<std::decay_t<T>> {};

namespace internal {
template <typename T>
struct is_eigen_array_impl : std::false_type {};
template <typename T, int R, int C>
struct is_eigen_array_impl<Eigen::Array<T, R, C>> : std::true_type {};
}  // namespace internal

template <typename T>
struct is_eigen_array : internal::is_eigen_array_impl<std::decay_t<T>> {};

template <typename T>
using is_eigen_matrix_or_array
    = math::disjunction<is_eigen_matrix<T>, is_eigen_array<T>>;

namespace internal {
template <typename T>
struct is_eigen_contiguous_map_impl : std::false_type {};
template <typename T, int Opts>
struct is_eigen_contiguous_map_impl<Eigen::Map<T, Opts, Eigen::Stride<0, 0>>>
    : std::true_type {};

}  // namespace internal

template <typename T>
struct is_eigen_contiguous_map
    : internal::is_eigen_contiguous_map_impl<std::decay_t<T>> {};

}  // namespace stan
#endif