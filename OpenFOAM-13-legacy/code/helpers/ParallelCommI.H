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
inline uint Foam::ParallelComm<ThermoType>::encode(const word& s)
{
    const uint FNV_prime     = 16777619u;     // 0x01000193
    const uint offset_basis  = 2166136261u;   // 0x811C9DC5
    
    uint hash = offset_basis;
    
    for (unsigned char c : s)
    {
        hash ^= c;                     // XOR with byte value
        hash *= FNV_prime;             // Multiply by FNV prime
    }
    
    return hash;
}


template<class ThermoType>
inline Foam::label Foam::ParallelComm<ThermoType>::remainder 
(
    const uint& value,
    const uint& dividedBy
)
{
    return value % dividedBy;
}


template<class ThermoType>
inline Foam::string Foam::ParallelComm<ThermoType>::toBase256Word(uint value)
{
    if (value > 49729) 
    {
        FatalErrorIn("ParallelComm::toBase256Word")
            << "Value exceeds the maximum limit of 49728 (223*223-1)" << exit(FatalError);
    }
    
    char high = static_cast<char>(value / 223 + 32);  // high decimal 
    char low  = static_cast<char>(value % 223 + 32);  // low decimal

    string returnValue = (value > 222) ? (string() + high + low) : (string() + low);

    return returnValue;
}


// ************************************************************************* //