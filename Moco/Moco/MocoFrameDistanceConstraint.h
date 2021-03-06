#ifndef MOCO_MOCOFRAMEDISTANCECONSTRAINT_H
#define MOCO_MOCOFRAMEDISTANCECONSTRAINT_H
/* -------------------------------------------------------------------------- *
 * OpenSim Moco: MocoFrameDistanceConstraint.h                                *
 * -------------------------------------------------------------------------- *
 * Copyright (c) 2019 Stanford University and the Authors                     *
 *                                                                            *
 * Author(s): Nicholas Bianco                                                 *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0          *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "MocoConstraint.h"
#include "osimMocoDLL.h"

namespace OpenSim {

class OSIMMOCO_API MocoFrameDistanceConstraintPair : public Object {
    OpenSim_DECLARE_CONCRETE_OBJECT(MocoFrameDistanceConstraintPair, Object);

public:
    OpenSim_DECLARE_PROPERTY(frame1_path, std::string,
            "The first model frame path of the pair.");
    OpenSim_DECLARE_PROPERTY(frame2_path, std::string,
            "The second model frame path of the pair.");
    OpenSim_DECLARE_PROPERTY(minimum_distance, double,
            "The minimum distance apart that the two frame origins can be.");
    OpenSim_DECLARE_PROPERTY(maximum_distance, double,
            "The maximum distance apart that the two frame origins can be.")

    MocoFrameDistanceConstraintPair();
    MocoFrameDistanceConstraintPair(std::string firstFramePath,
            std::string secondFramePath, double minimum_distance,
            double maximum_distance);

private:
    void constructProperties();
};

/// This path constraint enforces that the distance between the origins of pairs 
/// of model frames is kept between minimum and maximum bounds. Frame pairs and 
/// their bounds are specified via a MocoFrameDistancConstraintPair. 
/// Any model component derived from Frame is valid to be included in a frame 
/// pair, and any number of frame pairs may be append to this constraint via 
/// addFramePair().
/// 
/// This constraint can be used as a simple method for preventing bodies in your
/// model from intersecting during an optimization. For example, the
/// following prevents feet from intersecting during a walking optimization:
/// @code
/// distance = problem.addPathConstraint<MocoFrameDistanceConstraint>();
/// distance.setName("minimum_distance");
/// SimTK::Real inf = SimTK::Infinity;
/// distance.addFramePair('/bodyset/calcn_l', '/bodyset/calcn_r', 0.1, inf);
/// distance.addFramePair('/bodyset/toes_l', '/bodyset/toes_r', 0.1, inf);
/// distance.addFramePair('/bodyset/calcn_l', '/bodyset/toes_r', 0.1, inf);
/// distance.addFramePair('/bodyset/toes_l', '/bodyset/calcn_r', 0.1, inf);
/// @endcode
/// 
/// @note This class represents a path constraint, *not* a model kinematic 
/// constraint. Therefore, there are no Lagrange multipliers or constraint
/// forces associated with this constraint. The model's force elements 
/// (including actuators) must generate the forces necessary for satisfying this 
/// constraint.
///       
/// @ingroup mocopathcon
class OSIMMOCO_API MocoFrameDistanceConstraint : public MocoPathConstraint {
    OpenSim_DECLARE_CONCRETE_OBJECT(
            MocoFrameDistanceConstraint, MocoPathConstraint);

public:
    MocoFrameDistanceConstraint();

    void addFramePair(MocoFrameDistanceConstraintPair pair) {
        append_frame_pairs(std::move(pair));
    }
    void addFramePair(const std::string& frame1_path, 
            const std::string& frame2_path, double minimum_distance, 
            double maximum_distance) {
        append_frame_pairs(MocoFrameDistanceConstraintPair(frame1_path, 
            frame2_path, minimum_distance, maximum_distance));
    }

protected:
    void initializeOnModelImpl(
            const Model& model, const MocoProblemInfo&) const override;
    void calcPathConstraintErrorsImpl(
            const SimTK::State& state, SimTK::Vector& errors) const override;

private:
    OpenSim_DECLARE_LIST_PROPERTY(frame_pairs, 
            MocoFrameDistanceConstraintPair, 
            "Pairs of frames whose origins are constrained to be within minimum "
            "and maximum bounds.");

    void constructProperties();
    mutable std::vector<std::pair<SimTK::ReferencePtr<const Frame>,
            SimTK::ReferencePtr<const Frame>>> m_frame_pairs;
};

} // namespace OpenSim

#endif // MOCO_MOCOFRAMEDISTANCECONSTRAINT_H