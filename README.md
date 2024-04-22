# CoMMA (COarse Mesh Multigrid Agglomerator)

<div align="center">
    <picture>
        <source media="(prefers-color-scheme: dark)" srcset="images/logos/logo-CoMMA_darkbg.png">
        <source media="(prefers-color-scheme: light)" srcset="images/logos/logo-CoMMA.png">
        <img src="images/logos/logo-CoMMA.jpg" alt="CoMMA's logo" width="128" height="auto"/>
    </picture>
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

## :wrench: Building the library
CoMMA uses `C++17` standards, hence a fairly recent compiler is needed, for
instance, GNU > 8.1.9. For the configuration and building steps, it
relies on `cmake` (>= 3.15).

CoMMA is a `C++` **header-only library** hence it does not need compilation, per
se, however a configuration step is necessary. A `python` module can be
generated using `pybind11`: this is very convenient for testing and debugging
purposes. Some tests relying on `Catch2` have been written to check the
soundness of CoMMA (for more details see the
[dedicated section](#mag-testing-comma) below). In order to work, `cmake` should
find `pybind11` and `Catch2` in the path, see [below](#link-dependencies) some tips
on how to achieve this.

Both the `python` module and the tests need compilation. Finally, one can install
the headers and, when applies, the `python` module in a given directory.

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
cmake -DINDEX_T="int" -DINT_T="int" -DREAL_T="double" -DCMAKE_INSTALL_PREFIX=../install ..
make
make install
```
Instead of `-DCMAKE_INSTALL_PREFIX=`, one could have used `--install-prefix`,
although this later only accepts absolute paths.

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
support (default is off).
Of course it needs the test to be on.
In this case, the `gcov` library is needed:
```shell
cmake -DBUILD_TESTS=On -DCOVERAGE=On ..
```

Support for `pkg-config` is enabled.
A template of such configuration file can be found
[in the repository](config_files/comma.pc.in); given the prefix provided in the
example above, it will be installed in `path/to/CoMMA/install/lib64/pkgconfig`.

An option is available to use the flags usually considered when compiling the
CODA-CFD library:
```shell
cmake -DCODAFLAGS=On ..
```

In order for CoMMA to be compatible with `spack` package manager, a
configuration file and some patches are given in
[`config_files/spack/comma`](config_files/spack/comma). The spack configuration
supports almost the same variants that `cmake` uses, e.g., `+python`, `+doc`,
`codaflags`. They only two differences is that coverage option is
not available, and the type choices are more limited. Indeed, one use 64 bit
integer with `+int64`, otherwise 32 bit; and double reals with `+real64`,
otherwise float.

## :link: Dependencies
If compiling CoMMA with tests, one needs to get `Catch2`. Being based on `cmake`, the
flow is similar to CoMMA one:
```shell
git clone https://github.com/catchorg/Catch2
cd Catch2
mkdir build
cd build
cmake --install-prefix /path/to/Catch2/install ..
make -j4
make install
```
Once that is finished, in order for CoMMA to see `Catch2`, add the install directory
to `cmake` path:
```shell
export CMAKE_PREFIX_PATH=/path/to/Catch2/install:$CMAKE_PREFIX_PATH
```

In order to get a `python` module of CoMMA, one has to compile it relying on
`pybind11`. The easiest way to obtain it is to install it via `pip` (notice that we
select the user installation, `--user`):
```shell
python3 -m pip --user pybind11
```
For `cmake` to find `pybind` one then has to give it the right path. Typically,
assuming one has used the command above and was using `python3.10`, that is done by
updating `cmake` path:
```shell
export CMAKE_PREFIX_PATH=${HOME}/.local/lib/python3.10/site-packages/pybind11:$CMAKE_PREFIX_PATH
```
The CLI utility `pybind11-config`, automatically installed with `pybind`, can help
identifying the right path.

## :construction_worker: Usage
CoMMA provides a `namespace` with the same name, but in lowercase: `comma`. Its
interface is very simple and consists in only one function,
`comma::agglomerate_one_level`. This functions needs several arguments: some
define the graph representation of the mesh (e.g., connectivities, weights) in
Compressed Row Storage (CRS) format; others set the parametrization of the
coarsening algorithm (e.g., anisotropy, goal cardinality of the coarse cells);
others are modified by CoMMA to store the results. No special classes or
containers are needed since CoMMA itself relies on containers of the standard
library. CoMMA does have some custom types though, which, as seen
[above](#wrench-building-the-library), are chosen during the configuration
phase. For more details, about the arguments, the reader is referred to the
Doxygen page of the
[function](https://onera.github.io/CoMMA/_co_m_m_a_8h.html#a906c231be20a1f53a240618bae81d95f)
and section 2 of the [user manual](Documentation/CoMMA_user_manual.pdf).

A typical `C++` file using CoMMA will look like the following:
```cpp
#include <vector>

#include "CoMMA/CoMMA.h"

// Graph representation
std::vector<comma::CoMMAIndexT> graph_CRS_rows, graph_CRS_cols;
std::vector<comma::CoMMAWeightT> graph_CRS_weights;
// ...

// Algorithm parametrization
comma::CoMMAIntT min_card, goal_card, max_card;
// ...

// Output storage
std::vector<comma::CoMMAIndexT> fc2cc;
// ...

// Agglomerate
comma::agglomerate_one_level(
  // ...args...
)
```

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
cmake -DBUILD_DOC=On -DCMAKE_INSTALL_PREFIX=../install ..
make
make install
```

An [online version](https://onera.github.io/CoMMA/) of the doc is available.

A user manual is also available, see
[`Documentation/CoMMA_user_manual.pdf`](Documentation/CoMMA_user_manual.pdf). The
goal of this document is to clearly state and explain how CoMMA works, that
applies both to algorithms and their actual implementation (e.g., which data
structures have been used). After having read this document, the user should be
able to understand what CoMMA actually does under the hood and should have the
essential insights to use it (e.g., which input parameters should one provides,
how they will impact the final results...).

Finally, a brief note dedicated to the
[aspect-ratio computation](Documentation/AR_note.pdf) can be found in the
repository as well.

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

## :mag: Testing CoMMA
A set of tests to verify code and algorithm integrity has been set up, see
[the related file](tests/test_structure.cpp). The tests rely on the `Catch2`
framework. To run the tests, start by building the library (see
[above](#wrench-building-the-library). The `cmake` commands related to the tests are
already part of the reference [`CMakeLists.txt`](CMakeLists.txt)), this will generate
an executable `CoMMA_test` in the building directory, simply run it.
```shell
cmake -DBUILD_TESTS=On ..
make CoMMA_test # or simply make
./CoMMA_test  # or simply: make test
```

## :snake: A `python` interface to CoMMA
A `python` module which interfaces to CoMMA can be obtained using `pybind11` (a
submodule of CoMMA). To have it, just "build", see
[above](#wrench-building-the-library).
```shell
cmake -DBUILD_PYTHON_BINDINGS=On ..
make CoMMA # or simply make
```
A library called `CoMMA.cpython-310-x86_64-linux-gnu.so` (or similar depending on the
`python` version and the architecture) is installed in
`${CMAKE_INSTALL_LIBDIR}/python3.X/site-packages`, which, supposing one has given
`install` as prefix in the `cmake` configuration step and using `python3.10`, will
develop to `install/lib64/python3.10/site-packages`. To use it, add that directory to
your `python` path:
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

## :handshake: Cite CoMMA
If you have found CoMMA useful, do not hesitate to cite it in your paper:
```bibtex
@techreport{CoMMA23,
    author = {Milani, Riccardo},
    title = {{CoMMA}, a geometric unstructured agglomerator},
    institution = {ONERA},
    number = {RT 7/30485},
    year = {2023},
    month = {November},
    url = {https://github.com/onera/CoMMA/blob/main/Documentation/CoMMA_user_manual.pdf},
}
```

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

## :pray: Acknowledgements
The development of CoMMA was financially supported by the European Union's Horizon 2020 research and innovation program under grant agreement number 956104 ("NextSim") and the French Directorate General for Civil Aviation (DGAC) project "LAMA".
