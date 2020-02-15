#include <stan/math/prim.hpp>
#include <gtest/gtest.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/math/distributions.hpp>

TEST(ProbNegBinomial2, ccdf_log_matches_lccdf) {
  int y = 1;
  double mu = 1.1;
  double phi = 2.3;

  EXPECT_FLOAT_EQ((stan::math::neg_binomial_2_lccdf(y, mu, phi)),
                  (stan::math::neg_binomial_2_ccdf_log(y, mu, phi)));
  EXPECT_FLOAT_EQ(
      (stan::math::neg_binomial_2_lccdf<int, double, double>(y, mu, phi)),
      (stan::math::neg_binomial_2_ccdf_log<int, double, double>(y, mu, phi)));
}
