# CoMMA (COarse Mesh Multigrid Agglomerator)

<div align="center">
    <img src="images/logos/logo-CoMMA.jpg" alt="CoMMA's logo" width="128" height="auto"/>
</div>

**CoMMA** is a suite of graph algorithms meant to operate on the graph
representation of an **unstructured** computational mesh.

## :triangular_ruler: Capabilities
The main features of CoMMA are:
- Support for any mesh (2- or 3D, polyhedral,...);
- Minimal knowledge of the mesh, only its graph representation;
- Sequential by zone (i.e. no coupling with graph partitioner);
- Optimization of the shape of the coarse cells w.r.t. their aspect ratio;
- Detection and treatment of anisotropic regions;
- Isotropic agglomeration with structured-like treatment of structured-like
  regions;
- Fine faces conservation for coarse levels;
- Connectivity of coarse cells.

## :scroll: License
CoMMA is distributed under [Mozilla Public License Version 2.0](LICENSE). It has
been registered (more precisely, version 1.3) to the
[Agency for the Protection of Programs](https://www.app.asso.fr/lagence) (APP),
Paris, with [IDDN](https://www.iddn.org/cert) identification number
IDDN.FR.001.420013.000.S.X.2023.000.31235.

## :wrench: Building the Library
CoMMA relies on a fairly recent version of `cmake` (3.15+). It is a `C++`
**header-only library** hence it does not need compilation, per se, however a
configuration step is necessary. A `python` module can be generated using
`pybind11`: this is very convenient for testing and debugging purposes. Some
tests relying on `Catch2` have been written to check the integrity of CoMMA (for
more details see the [dedicated section](#mag-testing-comma) below). Both the
`python` module and the tests need compilation. Finally, one can install the
headers and, when applies, the `python` module in a given directory.

The pivotal part of the configuration step is the choice of the types used by
CoMMA: one for indices (e.g., cell IDs), one for standard integers (e.g.,
cardinality of the coarse cells), and one for reals (e.g., graph weights). They
can be chosen with configuration options, respectively, `INDEX_T`, `INT_T`, and
`REAL_T`. If not set, the default values are, respectively, `unsigned long`,
`int`, and `double`.

A typical flow for the configuration and installation of CoMMA usually relies on
a standard out-of-source build and look like this:
```shell
cd path/to/CoMMA
mkdir build install
cd build
cmake -DINDEX_T="int" -DINT_T="int" -DREAL_T="double" --prefix=../install ..
make
make install
```
Notice that custom types have been chosen. Since the dependencies for the
`python` module and the tests are included in CoMMA repositories as submodules,
`cmake` should be able to find them automatically; of course, the repository
should have been initialized accordingly, see [below](#link-submodules).

If one wants to use CoMMA in their code, it is important to perform the
installation step: indeed, during configuration an important header (including
the type definitions) is generated and then added to the other headers during
the installation phase. If one tries to build using only the files in the
`include` directory, the additional configuration header won't be found and the
process will fail.

The compilation of the tests and the generation of the `python` bindings are
activated by default, but they can be switched off
```shell
cmake -DBUILD_TESTS=Off .. # No tests
cmake -DBUILD_PYTHON_BINDINGS=Off .. # No python bindings
```

An additional `cmake` option might be passed to build the tests with coverage
support (default is off), in this case, the `gcov` library is needed:
```shell
cmake -DCOVERAGE=On ..
```

Support for `pkg-config` can be enabled by passing the related option to
`cmake`:
```shell
cmake -DPKGCONFIG_SUPPORT=On --prefix=../install ..
```
A template of such configuration file can be found
[in the repository](config_files/comma.pc.in); given the prefix provided in the
example above, it will be installed in `path/to/CoMMA/install/lib64/pkgconfig`.

An option is available to use the flags usually considered when compiling the
CODA-CFD library:
```shell
cmake -DCODAFLAGS=On ..
```

## :construction_worker: Usage
The interface to CoMMA is very simple and consists in only one function
[`agglomerate_one_level`](https://onera.github.io/CoMMA/_co_m_m_a_8h.html#a906c231be20a1f53a240618bae81d95f).

## :book: Documentation
CoMMA is documented via `doxygen`. If you have it and wish to have the full
documentation, just run from the main directory:
```shell
doxygen Documentation/Doxyfile
```
and related html pages will be built in `documentation`. Otherwise, the
documentation can be activated during the configuration phase of the
compilation, then built and installed:
```shell
cmake -DBUILD_DOC=On --prefix=../install ..
make
make install
```

An [online version](https://onera.github.io/CoMMA/) of the doc is available.

A user manual is also available, see
[Documentation/CoMMA_user_manual.pdf](Documentation/CoMMA_user_manual.pdf). The
goal of this document is to clearly state and explain how CoMMA works, that
applies both to algorithms and their actual implementation (e.g., which data
structures have been used). After having read this document, the user should be
able to understand what CoMMA actually does under the hood and should have the
essential insights to use it (e.g., which input parameters should one provides,
how they will impact the final results...).

## :bulb: Examples
Here are two animations about the agglomeration on a 2D mesh of a ring for two
different option settings:
* Seeds pool with
  [full initialization](https://onera.github.io/CoMMA/struct_s_p_full_initializator.html)
  and
  [boundary priority](https://onera.github.io/CoMMA/class_seeds___pool___boundary___priority.html)

<div align="center">
    <img src="images/videos/bnd_full_init.gif" alt="" width="512" height="auto"/>
</div>

* Seeds pool with
  [one-point initialization](https://onera.github.io/CoMMA/struct_s_p_one_point_initializator.html)
  and
  [neighbourhood priority](https://onera.github.io/CoMMA/class_seeds___pool___neighbourhood___priority.html)

<div align="center">
    <img src="images/videos/neigh_pt_init.gif" alt="" width="512" height="auto"/>
</div>

## :link: Submodules
To handle the `python` binding and the tests, we take advantage of two
thirdparty libraries, [`pybind11`](https://github.com/pybind/pybind11) and
[`Catch2`](https://github.com/catchorg/Catch2) respectively. They are included
in this repository as submodules.

In order to update the submodules do:
```shell
git submodule init
git submodule update
```

## :mag: Testing CoMMA
A set of tests to verify code and algorithm integrity has been set up, see
[the related file](tests/test_structure.cpp). The tests rely on the `Catch2`
framework, which is why the related submodule is included in this repository. To
run the tests, build the library (see [above](#wrench-building-the-library), the
`cmake` commands related to the tests are already part of the reference
[`CMakeLists.txt`](CMakeLists.txt)), this will generate an executable
`CoMMA_test` in the building directory, simply run it.
```shell
cmake ..
make
./CoMMA_test  # or simply: make test
```

## :snake: A `python` interface to CoMMA
A `python` module which interfaces to CoMMA can be obtained using `pybind11` (a
submodule of CoMMA). To have it, just "build" (see
[above](#wrench-building-the-library), related the `cmake` commands are already
part of the reference [`CMakeLists.txt`](CMakeLists.txt)). A library called
`CoMMA.cpython-38-x86_64-linux-gnu.so` (or similar) is installed in the
`${CMAKE_INSTALL_LIBDIR}/python3.X/site-packages`, which, supposing one has
given `install` as prefix in the `cmake` configuration step and using
`python3.10`, will develop to `install/lib64/python3.10/site-packages`. To use
it, add that directory to your `python` path:
```shell
export PYTHONPATH:/path/to/CoMMA/install/lib64/python3.10/site-packages:$PYTHONPATH
```
then just load CoMMA module in a `python` session:
```python
import CoMMA

# Do python stuff
```

Like standard `C++` CoMMA, the `python` module contains only one
function, the counterpart of
[`agglomerate_one_level`](https://onera.github.io/CoMMA/_co_m_m_a_8h.html#abfb7a4b061c35873233941bb1329ea09).
It has just the very same input arguments, only, all arguments are necessary (no
defaulted parameters). However, differently from the `C++` version, it returns
three lists:
* `fc_to_cc`: list telling the ID of the coarse cell to which a fine cell
  belongs after agglomeration
* `aggloLines_Idx`: connectivity for the agglomeration lines: each element
  points to a particular element in the list `aggloLines`
* `aggloLines`: list storing all the elements of the anisotropic lines.

```python
import CoMMA
fc_to_cc, aggloLines_Idx, aggloLines = CoMMA.agglomerate_one_level(*args)
```

Several `python` scripts showcasing the CoMMA package (as well as its two main
dependencies, `meshio` and `dualGPy`) are [available](examples/scripts).

<!-- ## :robot: Continuous Integration
A Continuous Integration (CI) [workflow](.gitlab-ci.yml) is available and it
runs at each push event concerning `C++` or CI files (at least with the current
configuration). The workflow performs several steps:
1. Check out the repository and the submodules.
2. Do the build, both the tests and the `python` module (same as
   [above](#wrench-building-the-library)).
3. Run the tests and code coverage (done with
   [`gcovr`](https://gcovr.com/en/stable/) `python` module). This enables us to
   have nice badges related to the status of the pipeline
   [![pipeline status](https://gitlab.onera.net/numerics/solver/comma/badges/main/pipeline.svg)](https://gitlab.onera.net/numerics/solver/comma/-/commits/main)
   and the coverage
   [![coverage report](https://gitlab.onera.net/numerics/solver/comma/badges/main/coverage.svg)](https://gitlab.onera.net/numerics/solver/comma/-/commits/main)
4. Build the documentation (same as [above](#book-documentation)) and deploy it
   on a [GitLab page](https://numerics.gitlab-pages.onera.net/solver/comma/) -->
