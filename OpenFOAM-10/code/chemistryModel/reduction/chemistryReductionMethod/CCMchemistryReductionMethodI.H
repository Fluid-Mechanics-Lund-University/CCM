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
    Chemistry Coordinate Mapping (CCM) acceleration method for OpenFOAM-10

\*---------------------------------------------------------------------------*/

#include "CCMchemistryReductionMethod.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
inline Foam::label Foam::CCMchemistryReductionMethod<ThermoType>::nSpecie()
{
    return nSpecie_;
}


template<class ThermoType>
inline Foam::label
Foam::CCMchemistryReductionMethod<ThermoType>::nActiveSpecies() const
{
    return nActiveSpecies_;
}


template<class ThermoType>
inline const Foam::List<bool>&
Foam::CCMchemistryReductionMethod<ThermoType>::activeSpecies() const
{
    return activeSpecies_;
}


template<class ThermoType>
inline Foam::scalar
Foam::CCMchemistryReductionMethod<ThermoType>::tolerance() const
{
    return tolerance_;
}


template<class ThermoType>
inline bool Foam::CCMchemistryReductionMethod<ThermoType>::reactionDisabled
(
    const label i
) const
{
    return reactionsDisabled_[i];
}


// ************************************************************************* //
