#ifndef BAYESMIX_ALGORITHMS_CONDITIONAL_ALGORITHM_H_
#define BAYESMIX_ALGORITHMS_CONDITIONAL_ALGORITHM_H_

#include <Eigen/Dense>
#include <memory>

#include "base_algorithm.h"
#include "src/collectors/base_collector.h"
#include "src/mixings/conditional_mixing.h"

class ConditionalAlgorithm : public BaseAlgorithm {
 protected:
  //! Points at the same object as BaseAlgorithm::mixing
  std::shared_ptr<ConditionalMixing> cond_mixing;

 public:
  ~ConditionalAlgorithm() = default;
  ConditionalAlgorithm() = default;
  //!
  void initialize() override;
  //!
  void step() override {
    BaseAlgorithm::step();
    cond_mixing->update_state(unique_values, allocations);
  }
  //!
  Eigen::MatrixXd eval_lpdf(
      BaseCollector *const collector, const Eigen::MatrixXd &grid,
      const Eigen::MatrixXd &hier_covariates = Eigen::MatrixXd(0, 0),
      const Eigen::MatrixXd &mix_covariates = Eigen::MatrixXd(0, 0)) override;
};

#endif  // BAYESMIX_ALGORITHMS_CONDITIONAL_ALGORITHM_H_
