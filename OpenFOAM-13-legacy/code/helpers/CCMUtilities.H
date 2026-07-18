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

#ifndef CCMUtilities_H
#define CCMUtilities_H

#include "hashedWordList.H"
#include "dictionary.H"
#include "multicomponentMixture.H"
#include "volFields.H"
#include "Switch.H"
#include "scalarList.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class CCMUtilities Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class CCMUtilities
{
    // Private data

        //- Reference to the multi component mixture
        const multicomponentMixture<ThermoType>& mixture_;

        //- Reference to thermodynamic data of the species
        const PtrList<ThermoType>& specieThermos_;

        //- Reference to CCM dictionary
        const dictionary& CCMdict_;

        //- Number of species
        const label& nSpecie_;

        //- Reference to principal variables list
        hashedWordList& principalVars_;

        //- Reference to update variables list
        hashedWordList& updateVars_;

        //- Reference to regular variables list
        hashedWordList& regularVars_;

        //- Reference to old style phi switch
        const Switch& oldStylePhi_;

        //- References to J coefficient arrays
        scalarList& JhCoeff_;
        scalarList& JcCoeff_;
        scalarList& JnCoeff_;
        scalarList& JoCoeff_;
        scalarList& Jh_h2oCoeff_;
        scalarList& Jc_co2Coeff_;
        scalarList& Jo_co2_h2oCoeff_;

        //- Reference to mass fraction fields
        const PtrList<volScalarField>& Yvf_;

        //- Reference to global maximum mass fraction field
        scalarField& Ymax_;

        //- Reference to principal variable hash tables
        HashTable<scalar, word>& pvMin_;
        HashTable<scalar, word>& pvMax_;
        HashTable<scalar, word>& pvSpan_;

        //- Reference to ignore minimum threshold
        const scalar& ignoreMin_;

        //- Reference to mesh
        const fvMesh& mesh_;

public:

    // Constructors

        //- Construct from references to required data
        CCMUtilities
        (
            const multicomponentMixture<ThermoType>& mixture,
            const PtrList<ThermoType>& specieThermos,
            const dictionary& CCMdict,
            const label& nSpecie,
            hashedWordList& principalVars,
            hashedWordList& updateVars,
            hashedWordList& regularVars,
            const Switch& oldStylePhi,
            scalarList& JhCoeff,
            scalarList& JcCoeff,
            scalarList& JnCoeff,
            scalarList& JoCoeff,
            scalarList& Jh_h2oCoeff,
            scalarList& Jc_co2Coeff,
            scalarList& Jo_co2_h2oCoeff,
            const PtrList<volScalarField>& Yvf,
            scalarField& Ymax,
            HashTable<scalar, word>& pvMin,
            HashTable<scalar, word>& pvMax,
            HashTable<scalar, word>& pvSpan,
            const scalar& ignoreMin,
            const fvMesh& mesh
        );

        //- Disallow default bitwise copy construction
        CCMUtilities(const CCMUtilities&) = delete;


    //- Destructor
    ~CCMUtilities() = default;


    // Member Functions

        //- Append varName to list if not existing
        bool appendIfNotExisting(const word& varName, hashedWordList& list);

        //- Output hashedWordList in good format
        void outputHashedWordList(const hashedWordList& list) const;

        //- Initialize principal variable lists
        void initPvLists();

        //- Initialize regular variables
        void initRegularVars();

        //- Initialize J coefficients for element mass fractions
        void initJCoeffs();

        //- Examine min/max of principal variables
        void examineMinMax() const;

        //- Output the current min/max values of regular variables
        //- so that the user can adjust its settings
        void examineRegularVarsMinMax() const;

        //- Update global maximum mass fraction
        void updateYMax();

        //- Initialize principal variable min/max/span
        void initPvMinMaxSpan();

        //- Parameterized version of updatePvMinMaxSpan for any variable list
        void updateMinMaxSpan(
            const hashedWordList& vars, 
            scalarList& minVals, 
            scalarList& maxVals, 
            scalarList& spanVals,
            const label& nSlice
        ) const;

        //- Update principal variable min/max/span hash tables
        void updatePvMinMaxSpan(const label& nSlice);



    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const CCMUtilities&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Include implementation only for templates
#ifdef NoRepository
    #include "CCMUtilities.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //