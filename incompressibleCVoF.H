/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2024 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::incompressibleCVoF

Description

SourceFiles
    incompressibleCVoF.H
    incompressibleCVoF.C

\*---------------------------------------------------------------------------*/

#ifndef incompressibleMultiCVoF_H
#define incompressibleMultiCVoF_H

#include "incompressibleVoF.H"

#include "diluteDiffusionProperties.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace solvers
{

/*---------------------------------------------------------------------------*\
                          Class incompressibleCVoF Declaration
\*---------------------------------------------------------------------------*/

class incompressibleCVoF
:
    public incompressibleVoF
{
private:
    // Private Data

        //- Ref to main mixture momentum model
        const incompressibleTwoPhaseVoFMixture& mixture_;

protected:
    // main fields

        //- scalar concentration C 
        volScalarField C_;

    // diffusion properties

        //- main dictionary
        diluteDiffusionProperties diffusionProperties;

        //- diffusion constant
        dimensionedScalar DC_;

        //- Henry constant
        dimensionedScalar H_;

    // diffusion fields
        
        //- mean diffusivity
        volScalarField Diff_;

        //- interface flux field
        volScalarField phiH_;

        //- surface flux field
        surfaceScalarField phiHSurf_;


public:
    // Public Data Members

        //- Reference to scalar field B
        const volScalarField& C;

    // Runtime type information
    TypeName("incompressibleCVoF")

    // Constructors

        incompressibleCVoF(fvMesh& mesh);

        //- Disallow default bitwise copy construction
        incompressibleCVoF(const incompressibleCVoF&) = delete;

    //- Destructor
    virtual ~incompressibleCVoF();


    // Member Functions
        virtual void postSolve();


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const incompressibleCVoF&) = delete;


    // Friend Functions

    // Friend Operators

    // IOstream Operators
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace solvers
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
