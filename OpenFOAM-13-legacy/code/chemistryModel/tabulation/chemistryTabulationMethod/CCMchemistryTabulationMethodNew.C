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

#include "CCMchemistryTabulationMethod.H"
#include "cNoChemistryTabulation.H"
#include "Time.H"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::CCMchemistryTabulationMethod>
Foam::CCMchemistryTabulationMethod::New
(
    const IOdictionary& dict,
    const CCModeChemistryModel& chemistry
)
{
    if (dict.found("tabulation"))
    {
        const dictionary& tabulationDict(dict.subDict("tabulation"));

        const word methodName(tabulationDict.lookup("method"));

        Info<< "Selecting chemistry tabulation method " << methodName << endl;

        typename dictionaryConstructorTable::iterator cstrIter =
            dictionaryConstructorTablePtr_->find(methodName);

        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalErrorInFunction
                << "Unknown " << typeName_() << " type " << methodName << endl
                << "Valid " << typeName_() << " types are:"
                << dictionaryConstructorTablePtr_->sortedToc() << endl
                << exit(FatalError);
        }

        return autoPtr<CCMchemistryTabulationMethod>
        (
            cstrIter()(dict, chemistry)
        );
    }
    else
    {
        return autoPtr<CCMchemistryTabulationMethod>
        (
            new CCMchemistryTabulationMethods::none(dict, chemistry)
        );
    }
}


// ************************************************************************* //
