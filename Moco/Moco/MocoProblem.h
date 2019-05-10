#ifndef MOCO_MOCOPROBLEM_H
#define MOCO_MOCOPROBLEM_H
/* -------------------------------------------------------------------------- *
 * OpenSim Moco: MocoProblem.h                                                *
 * -------------------------------------------------------------------------- *
 * Copyright (c) 2017 Stanford University and the Authors                     *
 *                                                                            *
 * Author(s): Christopher Dembia, Nicholas Bianco                             *
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

#include "MocoProblemRep.h"

namespace OpenSim {

// ============================================================================
// MocoPhase
// ============================================================================

/// The states, controls, dynamics, parameters, costs, and constraints for a
/// phase of the problem.
/// The dynamics are provided by the %OpenSim Model.
///
/// This class allows you to define your problem, but does not let you do
/// anything with your problem (this class only contains user input).
/// Use MocoProblem::createRep() to create an instance of MocoProblemRep,
/// which provides additional functionality.
///
/// Supported %Model Component%s
/// ----------------------------
/// Moco does not support all types of models. Specifically, the
/// following components are not supported:
///   - Actuator%s with multiple controls (non-ScalarActuator%s).
class OSIMMOCO_API MocoPhase : public Object {
OpenSim_DECLARE_CONCRETE_OBJECT(MocoPhase, Object);
public:
    MocoPhase();

    /// Set the Model whose dynamics should be used for this phase.
    /// The phase takes ownership of the passed-in model. This function
    /// returns a pointer to the model stored in the phase (identical to the
    /// passed-in model).
    Model* setModel(std::unique_ptr<Model> model);
    /// The model is copied into the MocoPhase; further changes made to the
    /// passed-in model will have no effect on this MocoPhase.
    /// This function returns a pointer to the model stored in the phase
    /// (the copy).
    Model* setModelCopy(Model model);
    /// Set the bounds on the initial and final time for this phase.
    /// If you want to constrain the initial time to a single value, pass
    /// that value to the constructor of MocoInitialBounds. If you want the
    /// initial time to fall within a range, pass the lower and upper bounds
    /// to the constructor of MocoInitialBounds. Likewise for MocoFinalBounds.
    /// This will overwrite bounds that were set previously, if any.
    void setTimeBounds(const MocoInitialBounds&, const MocoFinalBounds&);
    /// Set information about a single state variable in this phase.
    /// @param name
    ///     The name must match the path of a state variable in the
    ///     model (e.g., `hip/flexion/value` or `hip/flexion/speed`).
    /// @param bounds
    ///     The bounds on this state variable over the entire phase. If
    ///     default-constructed (`{}`), then either no bounds are applied or
    ///     bounds are taken from the model (depending on the type of state
    ///     variable).
    /// @param init
    ///     The bounds on this state variable at the start of the phase.
    ///     By default, there are no additional bounds on the initial value
    ///     (though the `bounds` over the entire phase still apply to the
    ///     initial value).
    /// @param final
    ///     Similar to `init` but for the value at the end of the phase.
    ///
    /// For all bounds arguments: if you want to constrain to a single value,
    /// pass that single value. If you want to constrain to a range, pass
    /// the lower and upper bounds to the constructor as two arguments.
    ///
    /// ### Examples
    /// Set bounds over the entire phase, but do not specify additional
    /// bounds on the value at the start and end of the phase.
    /// @code{.cpp}
    /// phase.setStateInfo("/knee/flexion/value", {-1.5*SimTK::Pi, 0});
    /// @endcode
    ///
    /// Allow any value throughout the phase (within the coordinate's range,
    /// if clamped), but the initial value is 5.
    /// @code{.cpp}
    /// phase.setStateInfo("/ankle/flexion/value", {}, 5);
    /// @endcode
    ///
    /// Constrain the initial and final state to a single value of 0.
    /// @code{.cpp}
    /// phase.setStateInfo("/ankle/flexion/speed", {}, 0, 0);
    /// @endcode
    ///
    /// This function will overwrite any info that has previously been set for
    /// this state variable.
    void setStateInfo(const std::string& name, const MocoBounds& bounds,
            const MocoInitialBounds& init = {},
            const MocoFinalBounds& final = {});
    /// Set information about a single control variable in this phase.
    /// Similar to setStateInfo(). The name for a control is the path to the
    /// associated ScalarActuator (e.g., "/forceset/soleus_r").
    void setControlInfo(const std::string& name, const MocoBounds&,
            const MocoInitialBounds& = {}, const MocoFinalBounds& = {});
    /// Set information about a single control variable associated with a 
    /// non-scalar actuator in this phase. This differs from setControlInfo()
    /// for ScalarActuators in that you provide the actuator name and the
    /// index to the actuator's control vector for this variable. The control
    /// name becomes the actuator name appended with the index, e.g.
    /// "pelvis_residuals_2", where "pelvis_residuals" is the actuator name and
    /// "_2" specifies the control index.
    // TODO remove when we support named controls in OpenSim
    void setControlInfo(const std::string& actuName, int controlIndex,
            const MocoBounds&, const MocoInitialBounds& = {}, 
            const MocoFinalBounds& = {});
    /// Set the bounds on *all* of the kinematic constraint equations in this
    /// phase. When creating a MocoProblemRep, these bounds are used to create
    /// MocoConstraintInfo's for each kinematic constraint equation in the 
    /// phase.
    void setKinematicConstraintBounds(const MocoBounds& bounds)
    {   set_kinematic_constraint_bounds(bounds); }
    /// Set the bounds on *all* of the Lagrange multipliers in this phase. 
    /// When creating a MocoProblemRep, these bounds are used to create
    /// MocoVariableInfo%s for each Lagrange multiplier in the phase.
    void setMultiplierBounds(const MocoBounds& bounds)
    {   set_multiplier_bounds(bounds); }
    /// Add a parameter to this phase.
    /// Parameter variables must have a name (MocoParameter::setName()), and the
    /// name must be unique. Note that parameters have the name "parameter" by
    /// default, but choosing a more appropriate name is recommended.
    /// C++ example:
    /// @code{.cpp}
    /// // Using the base MocoParameter directly.
    /// auto param0Ptr = phase.addParameter("mass", "body", "mass", {0, 10});
    /// // Using a custom MocoParameter.
    /// auto param1Ptr = phase.addParameter<MyCustomParameter>(...);
    /// @endcode
    /// You can edit the parameter using the returned pointer.
    /// Python example:
    /// @code{.py}
    /// param = opensim.MocoParameter()
    /// phase.addParameter(param)
    /// @endcode
    /// Matlab example:
    /// @code
    /// param = org.opensim.modeling.MocoParameter();
    /// phase.addParameter(param);
    /// @endcode
    /// In both Python and Matlab, changes to `param` will affect your problem.
    template <typename MocoParamType, typename... Args>
    MocoParamType* addParameter(Args&&... args) {
        return addParameter(std::unique_ptr<MocoParamType>(
                new MocoParamType(std::forward<Args>(args)...)));
    }
    template <typename MocoParamType>
    MocoParamType* addParameter(std::unique_ptr<MocoParamType> param) {
        MocoParamType* ptr = param.get();
        updProperty_parameters().adoptAndAppendValue(param.release());
        return ptr;
    }
    /// Add a cost term to this phase.
    /// Cost terms must have a name (MocoCost::setName()), and the name must be
    /// unique. Note that costs have the name "cost" by default, so if you
    /// only have one cost, you don't need to set its name manually.
    /// C++ example:
    /// @code{.cpp}
    /// auto cost0Ptr = phase.addCost<MocoFinalTimeCost>();
    /// auto cost1Ptr = phase.addCost<MocoFinalTimeCost>("final_time");
    /// @endcode
    /// You can edit the cost using the returned pointer.
    /// Python example:
    /// @code{.py}
    /// cost = opensim.MocoFinalTimeCost()
    /// phase.addCost(cost)
    /// @endcode
    /// Matlab example:
    /// @code
    /// cost = org.opensim.modeling.MocoFinalTimeCost();
    /// phase.addCost(cost);
    /// @endcode
    /// In both Python and Matlab, changes to `cost` will affect your problem.
    template <typename MocoCostType, typename... Args>
    MocoCostType* addCost(Args&&... args) {
        return addCost(std::unique_ptr<MocoCostType>(
                new MocoCostType(std::forward<Args>(args)...)));
    }
    /// Add a cost term to this phase.
    /// Similar to above.
    template <typename MocoCostType>
    MocoCostType* addCost(std::unique_ptr<MocoCostType> cost) {
        MocoCostType* ptr = cost.get();
        updProperty_costs().adoptAndAppendValue(cost.release());
        return ptr;
    }

    /// Add a path constraint to this phase.
    /// Path constraints must have a name (MocoPathConstraint::setName()), and 
    /// the name must be unique. Note that path constraints have the name
    /// "path_constraint" by default, so if you only have one path constraint, 
    /// you don't need to set its name manually.
    /// C++ example:
    /// @code{.cpp}
    /// auto pcPtr = phase.addPathConstraint<MyPathConstraint>();
    /// @endcode
    /// You can edit the constraint using the returned pointer.
    /// Python example:
    /// @code{.py}
    /// pc = opensim.MyPathConstraint()
    /// phase.addPathConstraint(pc)
    /// @endcode
    /// Matlab example:
    /// @code
    /// pc = MyPathConstraint();
    /// phase.addPathConstraint(pc);
    /// @endcode
    /// In both Python and Matlab, changes to `pc` will affect your problem.
    template <typename MocoPCType, typename... Args>
    MocoPCType* addPathConstraint(Args&&... args) {
        return addPathConstraint(std::unique_ptr<MocoPCType>(
                new MocoPCType(std::forward<Args>(args)...)));
    }
    /// Add a path constraint to this phase.
    /// Similar to above.
    template <typename MocoPCType>
    MocoPCType* addPathConstraint(std::unique_ptr<MocoPCType> pc) {
        MocoPCType* ptr = pc.get();
        updProperty_path_constraints().adoptAndAppendValue(pc.release());
        return ptr;
    }

    const Model& getModel() const { return get_model(); }
    Model& updModel() { return upd_model(); }

    /// @details Note: the return value is constructed fresh on every call from
    /// the internal property. Avoid repeated calls to this function.
    MocoInitialBounds getTimeInitialBounds() const;
    /// @copydoc getTimeInitialBounds()
    MocoFinalBounds getTimeFinalBounds() const;
    /// Access explicit state infos provided to this phase. For some state
    /// variables, default bounds are obtained from the model.
    /// This function does *not* provide such automatically-populated bounds
    /// from the model. For that, use see MocoProblemRep::getStateInfo().
    const MocoVariableInfo& getStateInfo(const std::string& name) const;
    /// Access explicit control infos provided to this phase.
    /// Default bounds are obtained from the model.
    /// This function does *not* provide such automatically-populated bounds
    /// from the model. For that, use see MocoProblemRep::getControlInfo().
    const MocoControlInfo& getControlInfo(const std::string& name) const;

    const MocoParameter& getParameter(const std::string& name) const;
    MocoParameter& updParameter(const std::string& name);

    const MocoCost& getCost(const std::string& name) const;
    MocoCost& updCost(const std::string& name);

    /// Get a MocoPathConstraint from this MocoPhase. Note: this does not 
    /// include MocoKinematicConstraints, use getKinematicConstraint() instead.
    const MocoPathConstraint& getPathConstraint(const std::string& name) const;
    MocoPathConstraint& updPathConstraint(const std::string& name);

    /// @}

protected: // Protected so that doxygen shows the properties.
    OpenSim_DECLARE_PROPERTY(model, Model,
            "OpenSim Model to provide dynamics.");
    // TODO error if not provided.
    OpenSim_DECLARE_PROPERTY(time_initial_bounds, MocoInitialBounds,
            "Bounds on initial value.");
    OpenSim_DECLARE_PROPERTY(time_final_bounds, MocoFinalBounds,
            "Bounds on final value.");
    OpenSim_DECLARE_PROPERTY(default_speed_bounds, MocoBounds,
            "Bounds for coordinate speeds if not specified in "
            "state_infos (default: [-50, 50]).");
    OpenSim_DECLARE_LIST_PROPERTY(state_infos, MocoVariableInfo,
            "The state variables' bounds.");
    OpenSim_DECLARE_LIST_PROPERTY(control_infos, MocoControlInfo,
            "The control variables' bounds.");
    OpenSim_DECLARE_LIST_PROPERTY(parameters, MocoParameter,
            "Parameter variables (model properties) to optimize.");
    OpenSim_DECLARE_LIST_PROPERTY(costs, MocoCost,
            "Quantities to minimize in the cost functional.");
    OpenSim_DECLARE_LIST_PROPERTY(path_constraints, MocoPathConstraint,
            "Path constraints to enforce in the optimal control problem.");
    // TODO make this a list property of MocoConstraintInfos when we are able to
    // map OpenSim constraint names to Simbody constraints.
    OpenSim_DECLARE_PROPERTY(kinematic_constraint_bounds, MocoBounds,
        "The bounds on all the kinematic constraints in the model to be "
        "enforced. By default the constraints are strictly enforced (zero "
        "bounds).");
    OpenSim_DECLARE_PROPERTY(multiplier_bounds, MocoBounds,
        "Variable info to apply to all Lagrange multipliers in the problem. "
        "The default bounds are [-1000 1000].");

private:
    void constructProperties();

    friend MocoProblemRep;

};


// ============================================================================
// MocoProblem
// ============================================================================

/// A description of an optimal control problem, backed by %OpenSim Model%s.
/// A MocoProblem is a series of phases, each of which contains the following:
///   - OpenSim Model
///   - state and control variable info (e.g., bounds)
///   - parameter variables (model properties)
///   - cost terms
///   - constraint equations
/// Currently, only single-phase problems are supported.
/// This class has convenience methods to configure the first (0-th) phase.
///
/// This class allows you to define your problem, but does not let you do
/// anything with your problem (this class only contains user input).
/// Use createRep() to create an instance of MocoProblemRep,
/// which provides additional functionality.
class OSIMMOCO_API MocoProblem : public Object {
OpenSim_DECLARE_CONCRETE_OBJECT(MocoProblem, Object);
public:
    MocoProblem();

    /// @name Convenience methods for phase 0.
    /// These methods allow you to conveniently edit phase 0 of the problem.
    /// See MocoPhase's documentation for more information.
    /// @{

    /// Set the model to use for phase 0.
    /// @see MocoPhase::setModel().
    Model* setModel(std::unique_ptr<Model> model);
    /// Set the model to use for phase 0.
    /// @see MocoPhase::setModelCopy().
    Model* setModelCopy(Model model);
    /// Set time bounds for phase 0.
    void setTimeBounds(const MocoInitialBounds&, const MocoFinalBounds&);
    /// Set bounds for a state variable for phase 0.
    void setStateInfo(const std::string& name, const MocoBounds&,
            const MocoInitialBounds& = {}, const MocoFinalBounds& = {});
    /// Set bounds for a control variable for phase 0.
    void setControlInfo(const std::string& name, const MocoBounds&,
            const MocoInitialBounds& = {}, const MocoFinalBounds& = {});
    /// TODO
    void setControlInfo(const std::string& actuatorName, int controlIndex, 
            const MocoBounds&, const MocoInitialBounds& = {}, 
            const MocoFinalBounds& = {});
    /// Set bounds for the kinematic constraints in phase 0.
    void setKinematicConstraintBounds(const MocoBounds& bounds);
    /// Set bounds for the Lagrange multipliers in phase 0.
    void setMultiplierBounds(const MocoBounds& bounds);
    /// Add a parameter variable for phase 0.
    /// @see MocoPhase::addParameter()
    template <typename MocoParamType = MocoParameter, typename... Args>
    MocoParamType* addParameter(Args&&... args) {
        return upd_phases(0).addParameter(std::unique_ptr<MocoParamType>(
                new MocoParamType(std::forward<Args>(args)...)));
    }
    /// Add a parameter variable for phase 0.
    template <typename MocoParamType = MocoParameter>
    MocoParamType* addParameter(std::unique_ptr<MocoParamType> param) {
        return upd_phases(0).addParameter(std::move(param));
    }
    /// Add a cost term for phase 0.
    /// @see MocoPhase::addCost()
    template <typename MocoCostType, typename... Args>
    MocoCostType* addCost(Args&&... args) {
        return upd_phases(0).addCost(std::unique_ptr<MocoCostType>(
                new MocoCostType(std::forward<Args>(args)...)));
    }
    /// Add a cost term for phase 0.
    template <typename MocoCostType>
    MocoCostType* addCost(std::unique_ptr<MocoCostType> cost) {
        return upd_phases(0).addCost(std::move(cost));
    }
    /// Add a constraint for phase 0.
    /// @see MocoPhase::addPathConstraint()
    template <typename MocoPCType, typename... Args>
    MocoPCType* addPathConstraint(Args&&... args) {
        return upd_phases(0).addPathConstraint(std::unique_ptr<MocoPCType>(
                new MocoPCType(std::forward<Args>(args)...)));
    }
    /// Add a constraint for phase 0.
    template <typename MocoPCType>
    MocoPCType* addPathConstraint(std::unique_ptr<MocoPCType> pc) {
        return upd_phases(0).addPathConstraint(std::move(pc));
    }
    /// @}

    /// Get a modifiable phase of the problem by index (starting index of 0).
    /// This accesses the internal phases property.
    MocoPhase& updPhase(int index = 0)
    {   return upd_phases(index); }
    /// Get a modifiable phase of the problem by index (starting index of 0).
    /// This accesses the internal phases property.
    const MocoPhase& getPhase(int index = 0) const
    {   return get_phases(index); }

#ifndef SWIG // MocoProblemRep() is not copyable.
    /// Create an instance of MocoProblemRep, which fills in additional
    /// state and control bounds, and allows you to apply parameter values
    /// and evaluate the cost terms.
    ///
    /// This function will check your problem for various errors.
    MocoProblemRep createRep() const
    {   return MocoProblemRep(*this); }
#endif
    /// @cond
    /// For internal use. You must manage the memory for the returned pointer.
    std::unique_ptr<MocoProblemRep> createRepHeap() const
    {   return std::unique_ptr<MocoProblemRep>(new MocoProblemRep(*this)); }
    /// @endcond

    friend MocoProblemRep;

protected: // We'd prefer private, but protected means it shows up in Doxygen.
    // TODO OpenSim_DECLARE_LIST_PROPERTY_ATLEAST(phases, MocoPhase, 1,
    OpenSim_DECLARE_LIST_PROPERTY_SIZE(phases, MocoPhase, 1,
            "List of 1 or more MocoPhases.");

private:
    void constructProperties();

};

} // namespace OpenSim

#endif // MOCO_MOCOPROBLEM_H
