/******************************************************************************
Copyright (c) 2021, Farbod Farshidian. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

 * Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#include "ocs2_legged_robot_opt/constraint/EndEffectorYZAxisLinearConstraint.h"

namespace ocs2 {
namespace legged_robot {

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
EndEffectorYZAxisLinearConstraint::EndEffectorYZAxisLinearConstraint(const EndEffectorKinematics<scalar_t>& endEffectorKinematics,
                                                         size_t numConstraints, Config config)
    : StateInputConstraint(ConstraintOrder::Linear),
      endEffectorKinematicsPtr_(endEffectorKinematics.clone()),
      numConstraints_(numConstraints),
      config_(std::move(config)) {
  if (endEffectorKinematicsPtr_->getIds().size() != 1) {
    throw std::runtime_error("[EndEffectorYZAxisLinearConstraint] this class only accepts a single end-effector!");
  }
}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
EndEffectorYZAxisLinearConstraint::EndEffectorYZAxisLinearConstraint(const EndEffectorYZAxisLinearConstraint& rhs)
    : StateInputConstraint(rhs),
      endEffectorKinematicsPtr_(rhs.endEffectorKinematicsPtr_->clone()),
      numConstraints_(rhs.numConstraints_),
      config_(rhs.config_) {}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
void EndEffectorYZAxisLinearConstraint::configure(Config&& config) {
  assert(config.b.rows() == numConstraints_);
  assert(config.Ax.size() > 0 || config.Av.size());
  assert(config.Ax.size() > 0 && config.Ax.rows() == numConstraints_);
  assert(config.Ax.size() > 0 && config.Ax.cols() == 2);
  assert(config.Av.size() > 0 && config.Av.rows() == numConstraints_);
  assert(config.Av.size() > 0 && config.Av.cols() == 2);
  config_ = std::move(config);
}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
vector_t EndEffectorYZAxisLinearConstraint::getValue(scalar_t time, const vector_t& state, const vector_t& input,
                                               const PreComputation& preComp) const {
  vector_t f = config_.b;
  if (config_.Ax.size() > 0) {
    f.noalias() += config_.Ax * endEffectorKinematicsPtr_->getPosition(state).front().tail(2);
  }
  if (config_.Av.size() > 0) {
    f.noalias() += config_.Av * endEffectorKinematicsPtr_->getVelocity(state, input).front().tail(2);
  }
  return f;
}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
VectorFunctionLinearApproximation EndEffectorYZAxisLinearConstraint::getLinearApproximation(scalar_t time, const vector_t& state,
                                                                                      const vector_t& input,
                                                                                      const PreComputation& preComp) const {
  VectorFunctionLinearApproximation linearApproximation =
      VectorFunctionLinearApproximation::Zero(getNumConstraints(time), state.size(), input.size());

  linearApproximation.f = config_.b;

  if (config_.Ax.size() > 0) {
    const auto positionApprox = endEffectorKinematicsPtr_->getPositionLinearApproximation(state).front();
    linearApproximation.f.noalias() += config_.Ax * positionApprox.f.tail(2);
    linearApproximation.dfdx.noalias() += config_.Ax * positionApprox.dfdx.block(1,0,2,positionApprox.dfdx.cols());
  }

  if (config_.Av.size() > 0) {
    const auto velocityApprox = endEffectorKinematicsPtr_->getVelocityLinearApproximation(state, input).front();
    linearApproximation.f.noalias() += config_.Av * velocityApprox.f.tail(2);
    linearApproximation.dfdx.noalias() += config_.Av * velocityApprox.dfdx.block(1,0,2,velocityApprox.dfdx.cols());
    linearApproximation.dfdu.noalias() += config_.Av * velocityApprox.dfdu.block(1,0,2,velocityApprox.dfdu.cols());
  }

  return linearApproximation;
}

}  // namespace legged_robot
}  // namespace ocs2
