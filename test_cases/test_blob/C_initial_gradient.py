#!/usr/bin/env python3

import numpy as np
from pathlib import Path


def main(inputCPath: Path):
    """
    Creates a gradient concentration field given an input concentration field with set field values
    Resolution of the mesh needs to be set inside xRes and zRes!
    """
    cPreField = []
    cInput = []
    cPostField = []

    with open(inputCPath, "r") as inputFile:
        line = inputFile.readline()
        isInField = False
        fieldPreValue = 4

        while line != "":
            if line == "internalField   nonuniform List<scalar> \n":
                isInField = True

            if isInField and fieldPreValue > 0:
                fieldPreValue -= 1

            if fieldPreValue > 0:
                cPreField.append(line)

            if isInField and line == ")\n":
                isInField = False

            if isInField and fieldPreValue == 0:
                cInput.append(float(line.split("\n")[0]))

            if not isInField and fieldPreValue == 0:
                cPostField.append(line)

            line = inputFile.readline()

    xRes = 100
    zRes = xRes

    if len(cInput) != (xRes * zRes):
        raise ValueError("Input C file not corresponding to given resolution!")

    cStart = 1e-9
    """Starting value for the gradient"""
    cEnd = 4e-2
    """Ending value for the gradient"""
    cInitialEmpty = 1e-9
    """Initial value from the read C field, which is supposed to be filled by the gradient"""

    cFile = Path("0/C")

    zSpacing = np.linspace(cStart, cEnd, zRes)

    with open(cFile, "w") as outputFile:
        outputFile.writelines(cPreField)

        for z, zVal in enumerate(zSpacing):
            for x in range(xRes,):
                iterC = int(z * zRes + x)

                givenC = cInput[iterC]

                if givenC == cInitialEmpty:
                    outputFile.write(f"{zVal:.1e}\n")

                else:
                    outputFile.write(f"{givenC:.1e}\n")

        outputFile.writelines(cPostField)


if __name__ == "__main__":
    main("0/C.fieldSet_noGrad")
