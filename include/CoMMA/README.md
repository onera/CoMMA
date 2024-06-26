# Welcome to CoMMA

**CoMMA** (COarse Mesh Multigrid %Agglomerator) is a suite of graph algorithms meant to operate on the graph representation of an **unstructured** computational mesh.

The final objective is to give a map between the fine cells and coarse cells in the object representing the coarse cell graph.

The main features of CoMMA are:
- Sequential by zone (i.e., no coupling with graph partitioner);
- Fine faces conservation for coarse levels;
- Optimization of the shape of the coarse cells w.r.t. their aspect ratio;
- Connectivity of coarse cells;
- Detection and treatment of anisotropic regions;
- Isotropic agglomeration with structured-like treatment of structured-like regions.

CoMMA is a **header-only library**. A python module can be generated using `pybind11`.

The interface to CoMMA is very simple and consists in only one function [agglomerate_one_level](@ref comma::agglomerate_one_level).

In CoMMA repository, one can find some [documentation](https://gitlab.onera.net/numerics/solver/comma/-/tree/main/Documentation), such as [the user manual](https://gitlab.onera.net/numerics/solver/comma/-/blob/main/Documentation/CoMMA_user_manual.pdf) and a note about the [aspect ratio](https://gitlab.onera.net/numerics/solver/comma/-/blob/main/Documentation/AR_note.pdf), quantity that plays a pivotal role in the agglomeration.

### Other info
**Copyright**: Copyright © 2024 ONERA

**Authors**: Nicolas Lantos, Alberto Remigi, and Riccardo Milani

**Contributors**: Karim Anemiche

**Licence**: This project is released under the [Mozilla Public License 2.0](https://mozilla.org/MPL/2.0/)
