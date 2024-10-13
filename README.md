# incompressibleCVoF

OpenFOAM solver based upon incompressibleVoF utilizing an added scalar field **C** for modelling concentration.

Solver architecture (older OpenFOAM version): <br>
[Albert S. Kim](https://github.com/enphysoft/interPhaseDiffusionFoam)

Adapted concentration-dependent surface tension with ideas from: <br>
[Zimbrod et al.](https://github.com/pzimbrod/thermocapillaryInterFoam)

## Prerequisites

- OpenFOAM-12 (from [OpenFOAM.org](https://openfoam.org))
- basic C++ toolchain (compatible for OpenFOAM)

Tested for compilation under Linux, Ubuntu 22.04 and 24.04

## Building

- Clone the repo
- Standard OpenFOAM compilation invoke

```bash
wmake
```

The library will be installed into the users OpenFOAM library directory `FOAM_USER_LIBBIN`

## Utilization

The standard setup for incompressibleVoF is needed. Therefore any two-phase model can be used.
An added concentration field is needed inside the time-directory (standard `/0`).

The concentration field `C` models an arbitrary concentration of dilute inside all phases.
To accommodate the phase transition from A to B, Henry's Law defines the concentration jump at the
boundary.

Henry's constant (dimension-les) is defined as: <br>
$H=\frac{c_{alpha}}{c_{1-alpha}}$ <br>
Where $alpha$ is the main defined and modelled phase (from the two-phase approach).
$1-\alpha$ is the remaining phase inside the modelled domain.

Besides the concentration field an additional dictionary `diluteDiffusionProperties` inside `/constant`
is needed where all necessary constants for the concentration modelling are defined.
- Henry's constant as `H`
- phase-averaged diffusion constant `DC` $[m^2/s]$

Adapted from the ideas of Zimbrod et al. an concentration dependent surface tension model is also included
in the solver. Inside `phaseProperties` the surface tension can be modelled as `concentrationDependent`.
With this setting the surface tension can be modelled with `Funtion1` support from OpenFOAM. A polynomial
function with the concentration as argument can therefore be used to model the surface tension.
This allows the implementation of effects such as the Marangoni-effect.

Test-cases utilizing the solver are located inside `/test_cases`

## References

1.
> Zimbrod, P., Schreter, M., & Schilp, J. (2022). Efficient Simulation of Complex Capillary Effects in Advanced Manufacturing Processes using the Finite Volume Method. 2022 International Conference on Electrical, Computer, Communications and Mechatronics Engineering (ICECCME), 1–6. https://doi.org/10.1109/ICECCME55909.2022.9988504

2.
> Irvine, J. L., & Kim, A. S. (2019). Coupled transport phenomena of a bioswale process during storm runoff events: A CFD study using OpenFOAM. DESALINATION AND WATER TREATMENT, 140, 103–117. https://doi.org/10.5004/dwt.2019.23459

