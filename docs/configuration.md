# ⚙️ Advanced Configuration

This page covers the finer controls beyond the minimal setup in the [README](../README.md).

## Core Parameters

EC-CCM configuration comes down to a few controls:

### 1. `mode`
- `default` considers all dependencies of the reaction rates (all species, T, and p) and is recommended for almost all cases.
- `manual` gives finer control, e.g. choosing the variables and their min/max/span values.

### 2. `nSlice`
- Controls the maximum allowable error: `Error ≈ 1/2nSlice`.
- Typical range: 25 to 100.
- Start with 50 and adjust to your accuracy needs.

### 3. `principalVars` (manual mode)
- Selects which variables matter most for error control.
- Typically the major species (H2, O2, CH4) and temperature.
- More variables produce more groups (up to a limit), smoothly approaching DI accuracy.


## 🎯 Adaptive Error Control

The `ecMode` subdictionary lets CCM manage accuracy automatically by dynamically adjusting its active variable list (all species + T/p in `default` mode, or `principalVars` in `manual` mode).

```cpp
ecMode
{
    enabled           true;    // enable adaptive error control
    numECVarsToAdd    3;       // add up to 3 variables when accuracy demands it
    numECVarsToRemove 1;       // remove 1 variable when over-specified
    updateFreq        5;       // re-evaluate variable importance every 5 steps
}
```

CCM tunes itself continuously, adding critical variables as complexity rises and dropping redundant ones, with no manual intervention required.

> EC is enabled in the `default` mode.

## 🌐 Distributed Mode

By default, CCM considers thermochemical states across **all** processors when grouping (`global` mode). This maximizes reuse, but on very large runs it can become the bottleneck. The `communicator` subdictionary lets you trade a little reuse for much lighter communication:

```cpp
communicator
{
    mode              distributed;  // "global" (default) or "distributed"
    localCores        256;          // should be a factor of the total core count
    redistributeEvery 20;
}
```

- **`global`** (default): all processes share one communicator. Best reuse, ideal for small to medium cases (typically fewer than 4000 cores).
- **`distributed`**: processors are split into multiple independent sub-communicators of size `localCores`. States are shared only **within** each sub-communicator, which keeps communication local and scales far better as the core count grows.
- **`redistributeEvery`**: how often the local groups are rebuilt to keep communication costs low.

**Rule of thumb**: start with `global`, and switch to `distributed` when your case spans many cores (e.g. more than ~4000) and communication starts to dominate the CCM overhead. Set `localCores` to a value between 100 and 1000.
