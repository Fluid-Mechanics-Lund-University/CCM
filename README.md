# Chemistry Coordinate Mapping (CCM) for OpenFOAM-13

## Overview

**CCM (Chemistry Coordinate Mapping)** is an efficient chemistry acceleration method for OpenFOAM-13 that can **reduce computational costs by up to 10×** in reactive flow simulations. By grouping CFD cells with similar thermochemical states and sharing reaction rate calculations, CCM provides significant speedup while maintaining accuracy.

### 🚀 How CCM Works

CCM features an **adaptive error control (EC)** system that operates with minimal overhead while dynamically adjusting the grouping strategy to keep chemistry calculation errors within user-specified bounds. This approach helps ensure both improved performance and reliable solution quality. The theoretical foundation and implementation details are available in the author's [PhD Thesis](https://portal.research.lu.se/files/226878780/thesis_yuchen_no_signature.pdf). This version is currently the closest to the spirit of the [original paper](https://www.tandfonline.com/doi/epdf/10.1080/13647830.2012.713518?needAccess=true) in terms of implementation.

Our load balancing algorithms are specifically designed for the CCM method, working to distribute computational load effectively while reducing communication overhead in parallel computations.

### ⚡ Updates for Existing CCM Users

**This version introduces several important improvements** that enhance performance significantly:

🔥 **Complete species coverage** - Now supports all species in your kinetics mechanism  
🔥 **Automatic configuration** - No need for manual min/max/span settings (handled automatically)  
🔥 **Notable performance gains** - Substantially faster than previous versions

## ✨ Key Features

✅ **Adaptive Error Control**: Automatically keeps chemistry errors below user-defined threshold (1/nSlice)  
✅ **Dynamic Variable Selection**: Adds or removes principal variables based on importance metrics during runtime  
✅ **Strong Performance**: Typically achieves 2-10× speedup depending on mechanism complexity  
✅ **Load Balancing**: Optimized communication patterns for parallel computing  
✅ **Easy Integration**: Works smoothly with OpenFOAM-13's foamRun solver using multicomponentFluid module  
✅ **Configurable Parameters**: Adjustable settings to balance accuracy and performance based on your needs


## 🔧 Compilation

### Prerequisites

- OpenFOAM-13 properly installed and configured

### Build Instructions

1. **Set up your OpenFOAM-13 environment**:
   ```bash
   source /path/to/OpenFOAM-13/etc/bashrc  # Update with your installation path
   ```

2. **Compile the CCM library**:
   ```bash
   cd code/
   wclean && wmake -j
   ```

3. **Verify installation**:
   ```bash
   ls -la $FOAM_USER_LIBBIN/libCCM.so
   ```
   ✅ Success if `libCCM.so` appears in the output

## 🚀 Usage

### Quick Start Guide

1. **Prepare your case** - Two simple modifications:

   **Add CCM library** to `<case>/system/controlDict`:
   ```cpp
   libs ("libCCM.so");
   ```

   **Enable CCM solver** in `<case>/constant/chemistryProperties`:
   ```cpp
   chemistry
   {
       solver ode;
       method CCM;
   }
   ```

2. **Run your simulation**:

   ```bash
   # Single processor execution
   foamRun

   # Parallel execution (example with 6 cores)
   decomposePar -fileHandler collated
   mpirun -n 6 foamRun -parallel -fileHandler collated
   ```

### 📊 Benchmark Test Case

**Sandia Flame D** - Modified from the OpenFOAM tutorials for validation:

```bash
# Navigate to test case
cd Sandia/  

# Run the benchmark
foamRun

# Or run in parallel for faster results
decomposePar -force
mpirun -n 6 foamRun -parallel -fileHandler collated
```

## ⚙️ Configuration Mastery

### Core Parameters - Simple Yet Powerful

EC-CCM configuration boils down to two intuitive controls:

#### 1. **`principalVars`** - Your accuracy knobs
- Select which variables matter most for error control
- Typically includes major species (H2, O2, CH4) and temperature
- **Pro tip**: More variables = More groups (there will be a upper limit) = Smooth scales towards DI
- **Be happy**: The program will tell you what variables are currently included. Unnecessary will be removed automatically with **ecMode** on

#### 2. **`nSlice`** - Your precision dial
- Controls maximum allowable error: `Error ≈ 1/nSlice`
- Sweet spot: 25-100
- **Rule of thumb**: Start with 50, adjust based on your accuracy needs

### 🎯 Adaptive Error Control - Set It and Forget It

The `ecMode` is where the magic happens - CCM automatically manages accuracy by dynamically adjusting its active variable list:

```cpp
ecMode
{
    enabled           true;    // Activate the autopilot
    numECVarsToAdd    3;       // Add up to 3 variables when accuracy matters
    numECVarsToRemove 1;       // Drop 1 variable when over-specified
    updateFreq        5;       // Re-evaluate importance every 5 steps
}
```

**What this means for you**: CCM continuously optimizes itself, adding critical variables when complexity increases and removing redundant ones when possible - all without your intervention!

## 🏆 Real-World Performance

### RCCI Engine Simulation Results

In a challenging RCCI (Reactivity Controlled Compression Ignition) case with:
- **45 principal variables (you don't need so many, it is a test for the code)** 
- **nSlice = 50**

The results speak for themselves:

<img src="./simulation_comparison.gif" width="900" alt="RCCI comparison: CCM vs Direct Integration">

**The verdict**: 
- ✅ **Visually identical results** to direct integration (DI)
- ✅ **4× faster** than traditional methods (even better than our published 2× improvement!)
- ✅ **No observable differences** in any examined metrics

*Can you spot the difference? Neither can we - and that's the point!*


## 📜 License

GPL-3.0 License (same as OpenFOAM-13)

Copyright (C) 2021 Shijie Xu, Shenghui Zhong  
Copyright (C) 2025 Yuchen Zhou  
Based on OpenFOAM® (Copyright (C) 2016-2025 OpenFOAM Foundation)

See [COPYING](COPYING) file for full license text.


## 📚 Citation

If you use CCM in your research, please cite:
```bibtex
[Citation to be added]
```

## 👤 Author
Current version author: Yuchen Zhou
Second version author: Shijie Xu, Shenghui Zhong
First version author: Mehdi Jangi


