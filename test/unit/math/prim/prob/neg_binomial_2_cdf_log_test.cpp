#include <stan/math/prim.hpp>
#include <gtest/gtest.h>

TEST(ProbNegBinomial2, cdf_log_matches_lcdf) {
  int y = 1;
  double mu = 1.1;
  double phi = 2.3;

  EXPECT_FLOAT_EQ((stan::math::neg_binomial_2_lcdf(y, mu, phi)),
                  (stan::math::neg_binomial_2_cdf_log(y, mu, phi)));
  EXPECT_FLOAT_EQ(
      (stan::math::neg_binomial_2_lcdf<int, double, double>(y, mu, phi)),
      (stan::math::neg_binomial_2_cdf_log<int, double, double>(y, mu, phi)));
}
