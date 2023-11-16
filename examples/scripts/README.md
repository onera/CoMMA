# Examples of `CoMMA` python module
## Dependencies
In most of the scripts here, we use the string format `{var=}`, hence `python3.8` or later is needed.

Two main libraries are used, both are available on `GitHub` under the MIT license:

* [`meshio`](https://github.com/nschloe/meshio) for reading and exporting meshes in various formats
* [`dualGPy`](https://github.com/albiremo/dualGPy) for building the dual graph of a mesh

## Description
All the examples in the directory have, roughly speaking, the same structure:
1. Create / Read a mesh
2. Prepare the mesh with `dualGPy`
3. Agglomerate with `CoMMA`
4. Export the results to `.vtu` thanks to `meshio`

A brief description of the scripts in this directory:

* `ex_default_dualGPy.py`: Build a Cartesian 2D mesh with `dualGPy` default constructor. Simple switches enable the user to choose anisotropy, dimension and other parameters for `CoMMA`. Useful for familiarizing with:
    * Prepare a mesh with `dualGPy` so that it can be handled by `CoMMA`
    * Arguments and outputs of `CoMMA` call
    * Preparing the output (subdivision in classes) for `meshio`
* `ex_read_mesh.py`: Read an existing mesh via `meshio` and agglomerate it. Simple switches enable the user to choose anisotropy, dimension and other parameters for `CoMMA`. Useful for familiarizing with:
    * Reading a mesh with `meshio` and preparing it with `dualGPy`
    * Preparing the output (subdivision in classes and/or in cell types) for `meshio`
* `ex_aniso_lines.py`: Show how to draw anisotropic lines obtained from `CoMMA`. Useful for familiarizing with:
    * `CoMMA`'s definition of anisotropic lines
    * Drawing anisotropic lines with `dualGPy`
* `ex_several_agglo_levels.py`: Show how to agglomerate several times the same mesh using `CoMMA`. Useful for familiarizing with:
    * Running several agglomeration steps on the same mesh, each time preparing the result of the previous step for the following one
    * Outputting more than one field via `meshio`

## Deprecated
Some old deprecated examples:

* `test_from_comma_source.py`: Build a hand-picked 2D mesh, agglomerate it with `CoMMA` and export the result
* `test_dualGPy.py`: Build a hand-picked 2D mesh of quads and triangles with `dualGPy`, agglomerate it with `CoMMA` and export the result
* `test_square.py`: Build a Cartesian 2D mesh with `dualGPy` default constructor, agglomerate it with `CoMMA` and export the result

## Formatting
The python files are formatted with [_Black_](https://black.readthedocs.io/en/stable/) and [`isort`](https://pycqa.github.io/isort/). A configuration file is available, see `pyproject.toml`.
