/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2021-2024 OpenFOAM Foundation
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

#include "diluteDiffusionProperties.H"

#include "objectRegistry.H"

#include "Time.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(diluteDiffusionProperties, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * //

Foam::IOobject Foam::diluteDiffusionProperties::findModelDict
(
    const objectRegistry& obr,
    const word& group,
    bool registerObject
)
{
    typeIOobject<IOdictionary> diluteDiffusionPropertiesIO
    (
        IOobject::groupName(diluteDiffusionProperties::typeName, group),
        obr.time().constant(),
        obr,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        registerObject
    );

    if (diluteDiffusionPropertiesIO.headerOk())
    {
        return diluteDiffusionPropertiesIO;
    }

}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diluteDiffusionProperties::diluteDiffusionProperties
(
    const objectRegistry& obr,
    const word& group
)
:
    IOdictionary(findModelDict(obr, group, true))
{
    // Ensure name of IOdictionary is typeName
    rename(IOobject::groupName(diluteDiffusionProperties::typeName, group));
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::diluteDiffusionProperties::read()
{
    return regIOobject::read();
}


// ************************************************************************* //