/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_FORCEFIELD_ISOCHORICFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_ISOCHORICFORCEFIELD_H

#include "IABPlugin/include/debuggers.h"  // this from global project include
#include "IABPlugin/ForceFields/include/config.h"
#include <sofa/core/behavior/ForceField.h>

#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/core/MechanicalParams.h>
#include <SofaEigen2Solver/EigenSparseMatrix.h>

#include <sofa/defaulttype/RGBAColor.h>

namespace sofa
{

namespace component
{

namespace forcefield
{

/// Apply constant forces to given degrees of freedom.
template<typename DataTypes>
class IsochoricForceField : public core::behavior::ForceField<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(IsochoricForceField, DataTypes), SOFA_TEMPLATE(core::behavior::ForceField, DataTypes));
    using Real = typename DataTypes::Real;
    using VecCoord = typename DataTypes::VecCoord;
    using VecDeriv = typename DataTypes::VecDeriv;
    using DataVecCoord = Data<VecCoord>;
    using DataVecDeriv = Data<VecDeriv>;

    /// Declare here the data and their type, you want the user to have access to
    Data< Real > d_Ri, d_Ro; // referencce configuration radius
    Data< Real > d_ri, d_ro; // current configuration radius
    Data< Real > d_C1, d_C2; // material elasticity of iab walls
    Data< std::string > d_mode; // mode tells whether we are expanding or compressing the IABs; accepts "compress" or "expand"
    // Data< defaulttype::RGBAColor> color; ///< isochoric spherical forcefield color. (default=[0.0,0.5,1.0,1.0])
    Data< helper::vector< unsigned int > > indices; ///< index of nodes controlled by the isochoric fields
    unsigned counter = 0;

    Real m_Ri, m_Ro, m_ri, m_ro, m_C1, m_C2;  // local for internal data usage

    enum { N=DataTypes::spatial_dimensions };
    using DeformationGrad = defaulttype::Mat<N,N,Real>;  // defines the dimension of the deformation tensor

public:
    /// Function responsible for the initialization of the component
    void init() override;

    virtual void reinit() override;

    /// Add the explicit forces (right hand side)
    virtual void addForce(const core::MechanicalParams* params, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v) override;

    /// Add the explicit derivatives of the forces (contributing to the right hand side vector b)
    /// IF iterative solver: add the implicit derivatives of the forces (contributing to the left hand side matrix A)
    virtual void addDForce(const core::MechanicalParams* mparams, DataVecDeriv& d_df , const DataVecDeriv& d_dx) override;

    void draw(const core::visual::VisualParams* vparams) override;

    /// IF direct solver: add the implicit derivatives of the forces (contributing to the left hand side matrix A)
    void addKToMatrix(sofa::defaulttype::BaseMatrix *m, SReal kFactor, unsigned int &offset) override;

    /// Same as previous, but using accessor
    void addKToMatrix(const sofa::core::behavior::MultiMatrixAccessor* /*matrix*/, SReal /*kFact*/) ;

    SReal getPotentialEnergy(const core::MechanicalParams* params, const DataVecCoord& x) const override;

    linearsolver::EigenBaseSparseMatrix<typename DataTypes::Real> matS;

protected:

    IsochoricForceField();
    ~IsochoricForceField();

  core::behavior::MechanicalState<DataTypes> *mState;

  Real abstol, reltol;
};


#if  !defined(SOFA_COMPONENT_FORCEFIELD_TEMPLATEFORCEFIELD_CPP)
extern template class SOFA_BOUNDARY_CONDITION_API IsochoricForceField<sofa::defaulttype::Vec3Types>;
extern template class SOFA_BOUNDARY_CONDITION_API IsochoricForceField<sofa::defaulttype::Vec2Types>;
extern template class SOFA_BOUNDARY_CONDITION_API IsochoricForceField<sofa::defaulttype::Vec1Types>;
extern template class SOFA_BOUNDARY_CONDITION_API IsochoricForceField<sofa::defaulttype::Vec6Types>;
#endif


} // namespace forcefield

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_FORCEFIELD_TEMPLATEFORCEFIELD_H
