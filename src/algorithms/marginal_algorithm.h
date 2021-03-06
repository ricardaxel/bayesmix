#ifndef BAYESMIX_ALGORITHMS_MARGINAL_ALGORITHM_H_
#define BAYESMIX_ALGORITHMS_MARGINAL_ALGORITHM_H_

#include <google/protobuf/message.h>

#include <Eigen/Dense>
#include <memory>

#include "base_algorithm.h"
#include "marginal_state.pb.h"
#include "src/collectors/base_collector.h"
#include "src/hierarchies/base_hierarchy.h"
#include "src/mixings/marginal_mixing.h"

class MarginalAlgorithm : public BaseAlgorithm {
 protected:
  //! Points at the same object as BaseAlgorithm::mixing
  std::shared_ptr<MarginalMixing> marg_mixing;
  //!
  bayesmix::MarginalState curr_state;
  //!
  void initialize() override;
  //! Computes marginal contribution of a given iteration & cluster
  virtual Eigen::VectorXd lpdf_marginal_component(
      std::shared_ptr<AbstractHierarchy> hier, const Eigen::MatrixXd &grid,
      const Eigen::MatrixXd &covariates) const = 0;
  //!
  Eigen::VectorXd lpdf_from_state(const Eigen::MatrixXd &grid,
                                  const Eigen::MatrixXd &hier_covariates,
                                  const Eigen::MatrixXd &mix_covariates);
  //!
  bool update_state_from_collector(BaseCollector *coll);

 public:
  ~MarginalAlgorithm() = default;
  MarginalAlgorithm() = default;
  //!
  Eigen::MatrixXd eval_lpdf(
      BaseCollector *const collector, const Eigen::MatrixXd &grid,
      const Eigen::MatrixXd &hier_covariates = Eigen::MatrixXd(0, 0),
      const Eigen::MatrixXd &mix_covariates = Eigen::MatrixXd(0, 0)) override;
};

#endif  // BAYESMIX_ALGORITHMS_MARGINAL_ALGORITHM_H_
