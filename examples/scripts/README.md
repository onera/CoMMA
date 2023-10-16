# Examples of CoMMA python module
## Dependencies
In most of the scripts here, we use the string format `{var=}`, hence `python3.8` or later is needed.

Two main libraries are used, both are available on `GitHub` under the MIT license:

* [`meshio`](https://github.com/nschloe/meshio) for reading and exporting meshes in various formats
* [`dualGPy`](https://github.com/albiremo/dualGPy) for building the dual graph of a mesh

## Description
A brief description of the scripts in this directory:

* `ex_default_dualGPy.py`: Build a Cartesian 2D mesh with `dualGPy` default constructor. Simple switches enable the user to choose anisotropy, dimension and other parameters for `CoMMA`
* `ex_read_mesh.py`: Read an existing mesh via `meshio` and agglomerate it. Simple switches enable the user to choose anisotropy, dimension and other parameters for `CoMMA`
* `ex_aniso_lines.py`: Show how to draw anisotropic lines obtained from `CoMMA`
* `ex_several_agglo_levels.py`: Show how to agglomerate several times the same mesh using `CoMMA`

## Deprecated
Some old deprecated examples:

* `test_from_comma_source.py`: Build a hand-picked 2D mesh, agglomerate it with `CoMMA` and export the result
* `test_dualGPy.py`: Build a hand-picked 2D mesh of quads and triangles with `dualGPy`, agglomerate it with `CoMMA` and export the result
* `test_square.py`: Build a Cartesian 2D mesh with `dualGPy` default constructor, agglomerate it with `CoMMA` and export the result
