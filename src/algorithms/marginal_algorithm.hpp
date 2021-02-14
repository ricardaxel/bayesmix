#ifndef BAYESMIX_ALGORITHMS_MARGINAL_ALGORITHM_HPP_
#define BAYESMIX_ALGORITHMS_MARGINAL_ALGORITHM_HPP_

#include <google/protobuf/message.h>

#include <Eigen/Dense>

#include "../collectors/base_collector.hpp"
#include "base_algorithm.hpp"
#include "marginal_state.pb.h"

class MarginalAlgorithm : public BaseAlgorithm {
 protected:
    bayesmix::MarginalState curr_state;
 public:
  ~MarginalAlgorithm() = default;
  MarginalAlgorithm() = default;
  Eigen::MatrixXd eval_lpdf(const Eigen::MatrixXd &grid,
                            BaseCollector *coll) override;

  Eigen::VectorXd lpdf_from_state(const Eigen::MatrixXd &grid);

  bool update_state_from_collector(BaseCollector *coll);
};

#endif  // BAYESMIX_ALGORITHMS_MARGINAL_ALGORITHM_HPP_