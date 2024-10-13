/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2017-2024 OpenFOAM Foundation
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

#include "concentrationDependentSurfaceTension.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace surfaceTensionModels
{
    defineTypeNameAndDebug(concentrationDependent, 0);
    addToRunTimeSelectionTable
    (
        surfaceTensionModel,
        concentrationDependent,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::surfaceTensionModels::concentrationDependent::concentrationDependent
(
    const dictionary& dict,
    const fvMesh& mesh
)
:
    surfaceTensionModel(mesh),
    CName_(dict.lookupOrDefault<word>("C", "C")),
    sigma_
    (
        Function1<scalar>::New
        (
            "sigma",
            dimMass/dimVolume,
            dimForce/dimLength,
            dict
        )
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::surfaceTensionModels::concentrationDependent::~concentrationDependent()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::surfaceTensionModels::concentrationDependent::sigma() const
{
    tmp<volScalarField> tsigma
    (
        volScalarField::New("sigma", mesh_, dimSigma)
    );
    volScalarField& sigma = tsigma.ref();

    const volScalarField& C = mesh_.lookupObject<volScalarField>(CName_);

    sigma.primitiveFieldRef() = sigma_->value(C.primitiveField());

    volScalarField::Boundary& sigmaBf = sigma.boundaryFieldRef();
    const volScalarField::Boundary& CBf = C.boundaryField();

    forAll(sigmaBf, patchi)
    {
        sigmaBf[patchi] = sigma_->value(CBf[patchi]);
    }

    return tsigma;
}


bool Foam::surfaceTensionModels::concentrationDependent::readDict
(
    const dictionary& dict
)
{
    const dictionary& sigmaDict = surfaceTensionModel::sigmaDict(dict);

    CName_ = sigmaDict.lookupOrDefault<word>("C", "C");
    sigma_ =
        Function1<scalar>::New
        (
            "sigma",
            dimMass/dimVolume,
            dimForce/dimLength,
            sigmaDict
        );

    return true;
}


bool Foam::surfaceTensionModels::concentrationDependent::writeData
(
    Ostream& os
) const
{
    if (surfaceTensionModel::writeData(os))
    {
        os  << sigma_() << token::END_STATEMENT << nl;
        return os.good();
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //