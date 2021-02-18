#include "metropolis.h"

#include <iostream>
#include <stan/math/prim/prob/bernoulli_rng.hpp>
#include <stan/math/prim/prob/multi_normal_lpdf.hpp>
#include <stan/math/prim/prob/multi_normal_rng.hpp>
#include <stan/math/prim/prob/uniform_rng.hpp>

#include "src/utils/rng.h"

void Metropolis::generate_data() {
  int n_data = 500;
  dim = 3;
  covariates = Eigen::MatrixXd::Random(n_data, dim);
  Eigen::VectorXd alpha_true(dim);
  if (dim == 2) {
    alpha_true << -5.0, 5.0;
  } else if (dim == 3) {
    alpha_true << -5.0, 5.0, 0.0;
  }

  Eigen::VectorXd y(n_data);
  auto &rng = bayesmix::Rng::Instance().get();
  for (int i = 0; i < n_data; i++) {
    double prob = sigmoid(covariates.row(i) * alpha_true);
    y(i) = stan::math::bernoulli_rng(prob, rng);
  }
  data = y;
}

Eigen::VectorXd Metropolis::mala_mean() const {
  Eigen::VectorXd grad = (-1.0 / true_var) * state;
  for (int i = 0; i < data.rows(); i++) {
    grad +=
        (data(i) - sigmoid(state.dot(covariates.row(i)))) * covariates.row(i);
  }
  return state + step * grad;
}

Eigen::VectorXd Metropolis::draw_proposal() const {
  auto &rng = bayesmix::Rng::Instance().get();
  auto covar = prop_var * Eigen::MatrixXd::Identity(dim, dim);
  return stan::math::multi_normal_rng(state, covar, rng);
}

double Metropolis::like_lpdf(const Eigen::VectorXd &alpha) const {
  double lpdf = 0.0;
  for (int i = 0; i < data.size(); i++) {
    double sig = sigmoid(covariates.row(i).dot(alpha));
    lpdf += data(i) * std::log(sig) + (1.0 - data(i)) * std::log(1 - sig);
  }
  return lpdf;
}

double Metropolis::prior_lpdf(const Eigen::VectorXd &alpha) const {
  auto mean = Eigen::VectorXd::Zero(dim);
  auto covar = true_var * Eigen::MatrixXd::Identity(dim, dim);
  return stan::math::multi_normal_lpdf(alpha, mean, covar);
}

void Metropolis::metropolis_hastings_step() {
  auto &rng = bayesmix::Rng::Instance().get();
  // Create proposed state
  Eigen::VectorXd state_prop = draw_proposal();
  // Compute acceptance ratio
  double like_ratio = like_lpdf(state_prop) - like_lpdf(state);
  double prior_ratio = prior_lpdf(state_prop) - prior_lpdf(state);
  // double proposal_ratio = h(alpha) - h(alpha*)
  logratio = like_ratio + prior_ratio;
  // Accept with probability ratio
  double p = stan::math::uniform_rng(0.0, 1.0, rng);
  if (p < std::exp(logratio)) {
    state = state_prop;
  }
  output();
}

void Metropolis::output() {
  std::cout << "#" << iter << ":\tp=" << std::exp(logratio)
            << ",\tstate=" << state.transpose() << std::endl;
}
