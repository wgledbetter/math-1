#include <stan/math/prim.hpp>
#include <gtest/gtest.h>

TEST(ProbNegBinomial2Log, log_matches_lpmf) {
  int y = 1;
  double eta = 1.1;
  double phi = 2.3;

  EXPECT_FLOAT_EQ((stan::math::neg_binomial_2_log_lpmf(y, eta, phi)),
                  (stan::math::neg_binomial_2_log_log(y, eta, phi)));
  EXPECT_FLOAT_EQ((stan::math::neg_binomial_2_log_lpmf<true>(y, eta, phi)),
                  (stan::math::neg_binomial_2_log_log<true>(y, eta, phi)));
  EXPECT_FLOAT_EQ((stan::math::neg_binomial_2_log_lpmf<false>(y, eta, phi)),
                  (stan::math::neg_binomial_2_log_log<false>(y, eta, phi)));
  EXPECT_FLOAT_EQ(
      (stan::math::neg_binomial_2_log_lpmf<true, int, double, double>(y, eta,
                                                                      phi)),
      (stan::math::neg_binomial_2_log_log<true, int, double, double>(y, eta,
                                                                     phi)));
  EXPECT_FLOAT_EQ(
      (stan::math::neg_binomial_2_log_lpmf<false, int, double, double>(y, eta,
                                                                       phi)),
      (stan::math::neg_binomial_2_log_log<false, int, double, double>(y, eta,
                                                                      phi)));
  EXPECT_FLOAT_EQ(
      (stan::math::neg_binomial_2_log_lpmf<int, double, double>(y, eta, phi)),
      (stan::math::neg_binomial_2_log_log<int, double, double>(y, eta, phi)));
}
