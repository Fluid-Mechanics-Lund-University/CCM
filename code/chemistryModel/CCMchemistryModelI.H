/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2016-2025 OpenFOAM Foundation
     \\/     M anipulation  | Copyright (C) 2021 Shijie Xu, Shenghui Zhong
                             | Copyright (C) 2025 Yuchen Zhou
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM CCM extension.

    OpenFOAM CCM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM CCM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM CCM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Chemistry Coordinate Mapping (CCM) acceleration method for OpenFOAM-13

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
inline const Foam::PtrList<Foam::DimensionedField<Foam::scalar, Foam::volMesh>>&
Foam::CCMchemistryModel<ThermoType>::RR() const
{
    return RR_;
}

template<class ThermoType>
inline const Foam::multicomponentMixture<ThermoType>&
Foam::CCMchemistryModel<ThermoType>::mixture() const
{
    return mixture_;
}


template<class ThermoType>
inline const Foam::PtrList<Foam::Reaction<ThermoType>>&
Foam::CCMchemistryModel<ThermoType>::reactions() const
{
    return reactions_;
}


template<class ThermoType>
inline const Foam::PtrList<ThermoType>&
Foam::CCMchemistryModel<ThermoType>::specieThermos() const
{
    return specieThermos_;
}


template<class ThermoType>
inline Foam::label Foam::CCMchemistryModel<ThermoType>::nReaction() const
{
    return reactions_.size();
}


template<class ThermoType>
inline const Foam::DimensionedField<Foam::scalar, Foam::volMesh>&
Foam::CCMchemistryModel<ThermoType>::RR(const label i) const
{
    return RR_[i];
}


template<class ThermoType>
Foam::DimensionedField<Foam::scalar, Foam::volMesh>&
Foam::CCMchemistryModel<ThermoType>::RR(const label i)
{
    return RR_[i];
}

template<class ThermoType>
inline const Foam::word& Foam::CCMchemistryModel<ThermoType>::reactionName
(
    const label reactioni
) const
{
    return reactions_[reactioni].name();
}

// template<class ThermoType>
// inline void Foam::CCMchemistryModel<ThermoType>::setActive(const label i)
// {
//     this->thermo().composition().setActive(i);
// }


// template<class ThermoType>
// inline bool Foam::CCMchemistryModel<ThermoType>::active(const label i) const
// {
//     return this->thermo().composition().active(i);
// }





// ************************************************************************* //
