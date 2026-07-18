# 🗺️ Restricting Chemistry to a cellZone

This page covers the `cellZone` control, available in the **OpenFOAM-14** version. It builds on the standard chemistry model in the [README](../README.md).

## What it does

By default CCM integrates reaction rates in every cell of the domain. In many cases the chemistry is only active in a small part of the domain (the burning region), and integrating the cold surroundings wastes time. The `cellZone` control restricts integration to a chosen set of cells: reaction rates are computed inside the zone and set to zero everywhere else.

This feature was introduced in `OpenFOAM-14` for the standard chemistry model; the CCM version supports it as well.


## Examples

Add a `cellZone` entry to `constant/chemistryProperties`:

### 1. Whole domain (default)

```cpp
cellZone all;
```

### 2. Coded zone that tracks the flame

Re-select the cells every step from a live field. The example below enables chemistry only where the temperature exceeds a threshold:

```cpp
cellZone
{
    type        coded;
    name        reactingZone;

    // Re-select the cells every evaluation so the zone follows the solution
    regenerate  yes;

    code
    #{
        // Live temperature field from the object registry
        const volScalarField& T =
            mesh().lookupObject<volScalarField>("T");

        // Threshold: enable chemistry where T exceeds Tact
        const scalar Tact = 1500;

        // Select the internal cells satisfying the criterion
        cellIndices = selectIndices
        (
            T,
            [&](const scalar Ti){ return Ti > Tact; }
        );
    #};
}
```

