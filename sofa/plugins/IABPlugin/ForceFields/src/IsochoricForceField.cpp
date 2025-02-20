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
#define SOFA_COMPONENT_FORCEFIELD_ISOCHORICFORCEFIELD_CPP
// below from /modules/sofageneralobjectinteraction/interactionforcefield.cpp
#include "IABPlugin/ForceFields/include/IsochoricForceField.inl"
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/core/ObjectFactory.h>
// #include "IABPlugin/ForceFields/include/initMiscFEM.h"

namespace sofa
{

namespace component
{

namespace forcefield
{

using namespace sofa::defaulttype;


// Give a description of your class
// and declare the DataTypes on which the ForceField is instantiated

int IsochoricForceFieldClass = core::RegisterObject("IAB Isochoric ForceField applied to the boundary and interior")
        .add< IsochoricForceField<Vec3Types> >()
        // .add< IsochoricForceField<Vec2Types> >()
        // .add< IsochoricForceField<Vec1Types> >()
        // .add< IsochoricForceField<Vec6Types> >()

        ;

template class IsochoricForceField<Vec3Types>;
// template class IsochoricForceField<Vec2Types>;
// template class IsochoricForceField<Vec1Types>;
// template class IsochoricForceField<Vec6Types>;



} // namespace forcefield

} // namespace component

} // namespace sofa
