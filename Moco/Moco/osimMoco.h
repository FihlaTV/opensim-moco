#ifndef MOCO_OSIMMOCO_H
#define MOCO_OSIMMOCO_H
/* -------------------------------------------------------------------------- *
 * OpenSim Moco: osimMoco.h                                                   *
 * -------------------------------------------------------------------------- *
 * Copyright (c) 2017 Stanford University and the Authors                     *
 *                                                                            *
 * Author(s): Christopher Dembia                                              *
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

#include "About.h"
#include "Common/TableProcessor.h"
#include "Components/ActivationCoordinateActuator.h"
#include "Components/DeGrooteFregly2016Muscle.h"
#include "Components/DiscreteForces.h"
#include "Components/ModelFactory.h"
#include "Components/MultivariatePolynomialFunction.h"
#include "Components/PositionMotion.h"
#include "Components/SmoothSphereHalfSpaceForce.h"
#include "Components/StationPlaneContactForce.h"
#include "MocoBounds.h"
#include "MocoCasADiSolver/MocoCasADiSolver.h"
#include "MocoConstraint.h"
#include "MocoControlBoundConstraint.h"
#include "MocoFrameDistanceConstraint.h"
#include "MocoGoal/MocoAccelerationTrackingGoal.h"
#include "MocoGoal/MocoAngularVelocityTrackingGoal.h"
#include "MocoGoal/MocoContactTrackingGoal.h"
#include "MocoGoal/MocoControlGoal.h"
#include "MocoGoal/MocoControlTrackingGoal.h"
#include "MocoGoal/MocoInitialActivationGoal.h"
#include "MocoGoal/MocoInitialForceEquilibriumGoal.h"
#include "MocoGoal/MocoInitialVelocityEquilibriumDGFGoal.h"
#include "MocoGoal/MocoJointReactionGoal.h"
#include "MocoGoal/MocoMarkerFinalGoal.h"
#include "MocoGoal/MocoMarkerTrackingGoal.h"
#include "MocoGoal/MocoOrientationTrackingGoal.h"
#include "MocoGoal/MocoOutputGoal.h"
#include "MocoGoal/MocoPeriodicityGoal.h"
#include "MocoGoal/MocoStateTrackingGoal.h"
#include "MocoGoal/MocoSumSquaredStateGoal.h"
#include "MocoGoal/MocoTranslationTrackingGoal.h"
#include "MocoInverse.h"
#include "MocoParameter.h"
#include "MocoProblem.h"
#include "MocoSolver.h"
#include "MocoStudy.h"
#include "MocoStudyFactory.h"
#include "MocoTrack.h"
#include "MocoTrajectory.h"
#include "MocoTropterSolver.h"
#include "MocoUtilities.h"
#include "MocoWeightSet.h"
#include "ModelOperators.h"
#include "ModelProcessor.h"
#include "RegisterTypes_osimMoco.h"

#endif // MOCO_OSIMMOCO_H
