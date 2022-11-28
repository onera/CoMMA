A brief description of the scripts in this directory:

* `test_from_comma_source.py`: Build a hand-picked 2D mesh, agglomerate it with `CoMMA` and export the result
* `test_dualGPy.py`: Build a hand-picked 2D mesh of quads and triangles with `dualGPy`, agglomerate it with `CoMMA` and export the result
* `test_square.py`: Build a Cartesian 2D mesh with `dualGPy` default constructor, agglomerate it with `CoMMA` and export the result
* `ex_default_dualGPy.py`: Like the previous script, it takes advantage of the `dualGPy` default mesh constructor to build a mesh. Simple switches enable the user to choose anisotropy, dimension and other parameters for `CoMMA` (needs `python >= 3.8`)
* `ex_read_mesh.py`: Read an existing mesh via `meshio` and agglomerate it. Simple switches enable the user to choose anisotropy, dimension and other parameters for `CoMMA` (needs `python >= 3.8`)
* `ex_aniso_lines.py`: Show how to draw anisotropic lines obtained from `CoMMA` (needs `python >= 3.8`)
* `ex_several_agglo_levels.py`: Show how to agglomerate several times the same mesh using `CoMMA` (needs `python >= 3.8`)
