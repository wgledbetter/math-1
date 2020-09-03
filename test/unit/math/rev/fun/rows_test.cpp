#include <stan/math/rev.hpp>
#include <test/unit/math/rev/util.hpp>
#include <test/unit/math/rev/fun/util.hpp>
#include <gtest/gtest.h>

TEST_F(AgradRev, Matrix_rows_vector) {
  using stan::math::row_vector_v;
  using stan::math::rows;
  using stan::math::vector_v;

  vector_v v(5);
  v << 0, 1, 2, 3, 4;
  EXPECT_EQ(5U, rows(v));

  v.resize(0);
  EXPECT_EQ(0U, rows(v));
}
TEST_F(AgradRev, Matrix_rows_rowvector) {
  using stan::math::row_vector_v;
  using stan::math::rows;

  row_vector_v rv(5);
  rv << 0, 1, 2, 3, 4;
  EXPECT_EQ(1U, rows(rv));

  rv.resize(0);
  EXPECT_EQ(1U, rows(rv));
}
TEST_F(AgradRev, Matrix_rows_matrix) {
  using stan::math::matrix_v;
  using stan::math::rows;

  matrix_v m(2, 3);
  m << 0, 1, 2, 3, 4, 5;
  EXPECT_EQ(2U, rows(m));

  m.resize(0, 2);
  EXPECT_EQ(0U, rows(m));
}
