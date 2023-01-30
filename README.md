# CoMMA (COarse Mesh Multigrid Agglomerator)
<!--[![coverage report](https://gitlab.com/albiremo/CoMMA/badges/main/coverage.svg)](https://gitlab.com/albiremo/CoMMA/-/commits/main)-->

:book: [**DOCUMENTATION**](https://numerics.gitlab-pages.onera.net/solver/comma/) :book:

**CoMMA** is a suite of graph algorithms meant to operate on the graph
representation of an **unstructured** computational mesh.

## Capabilities
- Sequential by zone (i.e. no coupling with graph partitioner);
- Fine faces conservation for coarse levels;
- Optimization of the shape of the coarse cells w.r.t their aspect ratio;
- Connectivity of coarse cells;
- Detection and treatment of anisotropic regions;
- Isotropic agglomeration with structured-like treatment of structured-like
  regions.

## Building the Library
CoMMA is a `C++` **header-only library** hence it does not need compilation, per
se. Nonetheless, a `python` module can be generated using `pybind11`: this is
very convenient for testing and debugging purposes. Moreover, some tests via
`Catch2` have been written to check the integrity of CoMMA. Both the `python`
module and the tests need compilation.

To build this library you need:
- A fairly recent `cmake` (3.14+)

You should be able to install them with your distribution package manager or
you can checkout [spack](https://spack.readthedocs.io/en/latest/).

In order to build them, just create a directory named `build` and do an
out-of-source build with `cmake`

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

`cmake` should be able to find the simple dependencies by itself if they're
installed in standard location.

## Usage
The interface to CoMMA is very simple and consists in only one function
[`agglomerate_one_level`](CoMMA_lib/CoMMA.h).

## Documentation
CoMMA is documented via `doxygen`. If you have it and wish to have the full
documentation, just run from the main directory:
```shell
doxygen Documentation/Doxyfile
```
and related html pages will be built in `documentation`.

An [online version](https://numerics.gitlab-pages.onera.net/solver/comma/) of the doc hosted by
GitLab is available.

## Submodules
To handle the `python` binding and the tests, we take advantage of two
thirdparty libraries here included as submodules.

In order to update the submodules do:

```shell
git submodule init
git submodule update
```

## Convenient scripts
The file `CODA_transfer.sh` help for the installation in the CODA-CFD library.
