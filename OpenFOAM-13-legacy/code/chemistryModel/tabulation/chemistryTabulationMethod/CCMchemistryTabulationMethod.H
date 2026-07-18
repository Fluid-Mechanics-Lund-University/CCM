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
#ifndef CCMchemistryTabulationMethod_H
#define CCMchemistryTabulationMethod_H

#include "IOdictionary.H"
#include "scalarField.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class CCModeChemistryModel;

/*---------------------------------------------------------------------------*\
                  Class CCMchemistryTabulationMethod Declaration
\*---------------------------------------------------------------------------*/

class CCMchemistryTabulationMethod
{
public:

    //- Runtime type information
    TypeName("CCMchemistryTabulationMethod");


    // Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        CCMchemistryTabulationMethod,
        dictionary,
        (
            const dictionary& dict,
            const CCModeChemistryModel& chemistry
        ),
        (dict, chemistry)
    );


    // Constructors

        //- Construct from components
        CCMchemistryTabulationMethod
        (
            const dictionary& dict,
            const CCModeChemistryModel& chemistry
        );


    // Selectors

        static autoPtr<CCMchemistryTabulationMethod> New
        (
            const IOdictionary& dict,
            const CCModeChemistryModel& chemistry
        );


    //- Destructor
    virtual ~CCMchemistryTabulationMethod();


    // Member Functions

        //- Return true if the tabulation method tabulates
        //  otherwise return false
        virtual bool tabulates() = 0;

        // Retrieve function: (only virtual here)
        // Try to retrieve a stored point close enough (according to tolerance)
        // to a stored point. If successful, it returns true and store the
        // results in RphiQ, i.e. the result of the integration of phiQ
        virtual bool retrieve
        (
             const scalarField& phiQ,
             scalarField& RphiQ
        ) = 0;

        // Add function: (only virtual here)
        // Add information to the tabulation algorithm. Give the reference for
        // future retrieve (phiQ) and the corresponding result (RphiQ).
        virtual label add
        (
            const scalarField& phiQ,
            const scalarField& RphiQ,
            const label nActive,
            const label li,
            const scalar deltaT
        ) = 0;

        // Update function: (only virtual here)
        // The underlying structure of the tabulation is updated/cleaned
        // to increase the performance of the retrieve
        virtual bool update() = 0;

        virtual void reset() = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
