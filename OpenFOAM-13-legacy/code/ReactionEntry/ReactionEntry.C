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

#include "volFields.H"

#include "ReactionEntry.H"
namespace Foam
{
    Foam::word num2Word(const Foam::label& num, const Foam::label& digits)
    {
        word tempWord = std::to_string(num);
        tempWord.insert(0, digits - tempWord.size(), '0');
        return tempWord;
    }


    Ostream& operator << (Ostream& os, const ReactionEntry& re)
    {
        re.write(os);
        return os;
    }

    Istream& operator >> (Istream& is, ReactionEntry& re)
    {
        re.read(is);
        return is;
    }

    bool operator == (const ReactionEntry& re1, const ReactionEntry& re2)
    {
        return (&re1 == &re2)? true: false;
    }

    bool operator != (const ReactionEntry& re1, const ReactionEntry& re2)
    {
        return !(re1 == re2);
    }

    ReactionEntry operator+ (const ReactionEntry& re1, const ReactionEntry& re2)
    {
        scalar totalCount = re1.count + re2.count;
        scalar re1Ratio = scalar(re1.count)/totalCount;
        scalar re2Ratio = scalar(re2.count)/totalCount;

        if (re1.debug && re2.debug)
        {
            scalarField re1Ystd = (re1.count == 1) ? scalarField(re1.Y.size(),0.0) : re1.Ystd;
            scalarField re2Ystd = (re2.count == 1) ? scalarField(re2.Y.size(),0.0) : re2.Ystd;
            scalarField M1 = sqr(re1Ystd) * (re1.count - 1);
            scalarField M2 = sqr(re2Ystd) * (re2.count - 1);
            scalarField avg = re1.Y * re1Ratio + re2.Y * re2Ratio;
            label newCount = re1.count + re2.count;
            M1 = M1 + M2 + sqr(re1.Y - re2.Y)/(1/scalar(re1.count) + 1/scalar(re2.count));
            return ReactionEntry(avg, 
                            re1.T*re1Ratio + re2.T*re2Ratio, 
                            re1.p*re1Ratio + re2.p*re2Ratio, 
                            re1.dtChem*re1Ratio + re2.dtChem*re2Ratio,
                            re1.rho0*re1Ratio + re2.rho0*re2Ratio,
                            re1.rho*re1Ratio + re2.rho*re2Ratio, 
                            newCount,
                            true,
                            sqrt(M1/(newCount - 1))
                            );
        }


        return ReactionEntry(re1.Y*re1Ratio + re2.Y*re2Ratio, 
                            re1.T*re1Ratio + re2.T*re2Ratio, 
                            re1.p*re1Ratio + re2.p*re2Ratio, 
                            re1.dtChem*re1Ratio + re2.dtChem*re2Ratio,
                            re1.rho0*re1Ratio + re2.rho0*re2Ratio,
                            re1.rho*re1Ratio + re2.rho*re2Ratio, 
                            totalCount);
    }

    void mergeReactionEntries(reactionEntries& dest, const reactionEntries& target)
    {
        for (auto it = target.begin(); it != target.end(); it++)
        {
            if (dest.found(it.key()))
            {
                dest.set(it.key(), dest.find(it.key())() + it());
            }
            else
            {
                dest.set(it.key(), it());
            }
        }
    }
}