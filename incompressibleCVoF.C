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

\*---------------------------------------------------------------------------*/

#include "incompressibleCVoF.H"
#include "addToRunTimeSelectionTable.H"

#include "incompressibleInterPhaseTransportModel.H"

#include "fvModels.H"
#include "fvConstraints.H"

#include "fvmDiv.H"
#include "fvmLaplacian.H"
#include "fvcSnGrad.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace solvers
{
    defineTypeNameAndDebug(incompressibleCVoF, 0);
    addToRunTimeSelectionTable(solver, incompressibleCVoF, fvMesh);
}
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::solvers::incompressibleCVoF::incompressibleCVoF(fvMesh& mesh)
:
    incompressibleVoF(mesh),

    mixture_(
        refCast<incompressibleTwoPhaseVoFMixture>(twoPhaseVoFSolver::mixture)
    ),

    C_
    (
        IOobject
        (
            "C",
            runTime.name(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),

    diffusionProperties(mesh, Foam::word::null),

    DC_
    (
        diffusionProperties.lookup
        (
            "DC",
            false,
            false
        )
    ),

    H_
    (
        diffusionProperties.lookup
        (
            "H",
            false,
            false
        )
    ),

    Diff_
    (
        "Diff",
        (DC_ * alpha1 + DC_ * alpha2 * H_)
    ),

    phiH_
    (
        "phiH",
        (Diff_ * (1.0 - H_) / (alpha1 + (1.0 - alpha1) * H_))
    ),

    phiHSurf_
    (
        "phiHsurf",
        (fvc::interpolate(phiH_) * fvc::snGrad(alpha1) * mesh.magSf())
    ),

    C(C_)
{
}

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::solvers::incompressibleCVoF::~incompressibleCVoF()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::solvers::incompressibleCVoF::postSolve()
{
    // mean diffusivity
    Diff_ = (DC_ * alpha1 + DC_ * alpha2 * H_);
    // mean diffusive flux
    phiH_ = (Diff_ * (1.0 - H_) / (alpha1 + (1.0 - alpha1) * H_));
    // diffusive flux through surface
    phiHSurf_ = (fvc::interpolate(phiH_) * fvc::snGrad(alpha1) * mesh.magSf());

    {
        fvScalarMatrix CEqn
        (
            fvm::ddt(C_)
            + fvm::div(phi, C_)
            - fvm::laplacian(DC_, C_)
            - fvm::div(phiHSurf_,C_)
            ==
                fvModels().source(C_)
        );

        CEqn.relax();
        fvConstraints().constrain(CEqn);
        CEqn.solve();
        fvConstraints().constrain(C_);
    }

}

// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * Friend Operators * * * * * * * * * * * * * * //


// ************************************************************************* //
