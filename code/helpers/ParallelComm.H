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

#ifndef ParallelComm_H
#define ParallelComm_H

#include "RoundRobin.H"
#include "List.H"
#include "word.H"
#include "string.H"
#include "label.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class ParallelComm Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class ParallelComm
{
    // Private data

        //- Reference to RoundRobin communicator
        RoundRobin& RoundRobinCommunicator_;

public:

    // Constructors

        //- Construct from references to required data
        ParallelComm
        (
            RoundRobin& RoundRobinCommunicator
        );

        //- Disallow default bitwise copy construction
        ParallelComm(const ParallelComm&) = delete;


    //- Destructor
    ~ParallelComm() = default;


    // Member Functions

        //- Round-robin communication: distribute different data to each core
        template<typename DataType>
        void performRoundRobinDistribute
        (
            const List<DataType>& sendData,
            List<DataType>& receiveData
        );

        //- Round-robin communication: broadcast same data to all cores
        template<typename DataType>
        void performRoundRobinBroadcast
        (
            DataType& myData,
            List<DataType>& allData,
            const label myCoreNumLCS
        );

        //- Low-level send/receive utility for round-robin communication
        template<typename DataType>
        void sendReceivePair
        (
            const label opponent,
            const label opponentLCS,
            const DataType& dataToSend,
            DataType& dataToReceive,
            const label myCoreNumLCS
        );

        //- Encode string using FNV-1a hash
        static inline uint encode(const word& s);

        //- Calculate remainder for distribution
        static inline label remainder(const uint& value, const uint& dividedBy);

        //- Convert integer to base 256 word for encoding
        static inline string toBase256Word(uint value);


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const ParallelComm&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Include inline functions
#include "ParallelCommI.H"

#ifdef NoRepository
    #include "ParallelComm.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //