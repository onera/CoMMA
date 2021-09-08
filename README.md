# CoMMA (COarse Mesh Multigrid Agglomerator)

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

```
mkdir build
cd build
cmake ..
```

CMake should be able to find the simple dependencies by itself if they're
installed in standard location. 

In order to update the submodules do:

```
git submodule init
git submodule update
```



The file `CODA_transfer.sh` help for the installation in the CODA-CFD library.

## TO BE DONE

- Python interface for testing the algorithm with [PyBind11](https://github.com/pybind/pybind11). The interface should read a mesh in a given format and provide the [CSR](https://en.wikipedia.org/wiki/Sparse_matrix) representation of the dual graph with all the arguments required by CoMMA itself.
- Proper [Doxigen](https://www.doxygen.nl/index.html) documentation.
