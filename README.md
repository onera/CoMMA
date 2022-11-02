# CoMMA (COarse Mesh Multigrid Agglomerator)
[![coverage report](https://gitlab.com/albiremo/CoMMA/badges/main/coverage.svg)](https://gitlab.com/albiremo/CoMMA/-/commits/main)
[Documentation](https://albiremo.gitlab.io/CoMMA/)
## Capabilities
CoMMA is a Geometric Agglomerator with the following characteristics:
- Sequential by zone (i.e. no coupling with graph partitioner);
- Fine faces conservation for coarse levels;
- Optimization of the shape of CC w.r.t their aspect ratio;
- Connectivity of CC;
- Detection and treatment of anisotropic regions;
- Isotropic agglomeration with structured-like treatment of structured-
like regions;
- Treatment of triconnectivity.

## Building the Library

To build this library you need:
- A fairly recent `cmake` (3.14+)

You should be able to install them with your distribution package manager or
you can checkout [spack](https://spack.readthedocs.io/en/latest/) .
In order to build it just create a directory named `build` and do an
out-of-source build with cmake

```shell
mkdir build
cd build
cmake ..
```

An option is available to use the flags usually considered when compiling the
CODA-CFD library:
```shell
cmake -DCODAFLAGS=ON ..
```

CMake should be able to find the simple dependencies by itself if they're
installed in standard location.

In order to update the submodules do:

```shell
git submodule init
git submodule update
```



The file `CODA_transfer.sh` help for the installation in the CODA-CFD library.

## To understand the templetization and other features
- [The use of this in the child classes](https://stackoverflow.com/questions/7076169/not-declared-in-this-scope-error-with-templates-and-inheritance)
- [The runtime vs the compile time](https://stackoverflow.com/questions/7076169/not-declared-in-this-scope-error-with-templates-and-inheritance)
- [The constexpr bool vs macro](https://stackoverflow.com/questions/67454808/c-constexpr-boolean-vs-macro)
