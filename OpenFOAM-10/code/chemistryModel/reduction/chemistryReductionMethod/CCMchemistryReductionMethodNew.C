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
#include "cNoChemistryReduction.H"
#include "Time.H"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class ThermoType>
Foam::autoPtr<Foam::CCMchemistryReductionMethod<ThermoType>>
Foam::CCMchemistryReductionMethod<ThermoType>::New
(
    const IOdictionary& dict,
    CCMchemistryModel<ThermoType>& chemistry
)
{
    if (dict.found("reduction"))
    {
        const dictionary& reductionDict(dict.subDict("reduction"));

        const word methodName(reductionDict.lookup("method"));

        Info<< "Selecting chemistry reduction method " << methodName << endl;

        const word methodTypeName =
            methodName + '<' + ThermoType::typeName() + '>';

        typename dictionaryConstructorTable::iterator cstrIter =
            dictionaryConstructorTablePtr_->find(methodTypeName);

        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalErrorInFunction
                << "Unknown " << typeName_() << " type " << methodName << endl
                << endl;

            const wordList names(dictionaryConstructorTablePtr_->sortedToc());

            wordList thisCmpts;
            thisCmpts.append(word::null);
            thisCmpts.append
            (
                basicThermo::splitThermoName(ThermoType::typeName(), 5)
            );

            wordList validNames;
            forAll(names, i)
            {
                const wordList cmpts(basicThermo::splitThermoName(names[i], 6));

                if
                (
                    SubList<word>(cmpts, 5, 1) == SubList<word>(thisCmpts, 5, 1)
                )
                {
                    validNames.append(cmpts[0]);
                }
            }

            FatalErrorInFunction
                << "Valid " << typeName_()
                << " types are:" << validNames << endl
                << exit(FatalError);
        }

        autoPtr<CCMchemistryReductionMethod<ThermoType>> crmPtr
        (
            cstrIter()(dict, chemistry)
        );

        chemistry.reduction_ = crmPtr->active();

        return crmPtr;
    }
    else
    {
        return autoPtr<CCMchemistryReductionMethod<ThermoType>>
        (
            new CCMchemistryReductionMethods::none<ThermoType>(dict, chemistry)
        );
    }
}


// ************************************************************************* //
