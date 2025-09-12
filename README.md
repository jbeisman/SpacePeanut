## SpacePeanut

SpacePeanut is a toy cosmology simulator with an Imgui UI and an OpenGL renderer, built on top of SDL3. Shared-memory CPU parallelism utilizing either C++ std::execution or OpenMP is used to allow real-time visualization of simulations containing millions of particles on common laptops.

## Features:
- Arcball camera
- Input and control UI
- Built-in renderer that uses grid mass density to color particles
- Dynamic color palette

## Building
This project uses CMake with the Ninja build generator. Additional prerequisites include:
- OpenMP or the Intel TBB library, depending on backend used
- OpenGL
- GLEW
- FFTW3

CMake presets are set, so the build is best invoked with one of the following combinations of Release or Debug builds and OpenMP or std::execution backends:
```
cmake --workflow --preset rel-omp
cmake --workflow --preset rel-std
cmake --workflow --preset dbg-omp
cmake --workflow --preset dbg-std
```

## Screenshots
