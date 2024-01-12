Miscellaneous scripts and examples:
* `test_util.h`: Provide a simple reader for meshes and a tool to build a new CSR graph representation of a coarse mesh from the data of the agglomeration. This second function is useful when one has already done an agglomeration and wants to do another level, it indeed generate the inputs needed by `CoMMA`
* `test_profile.cpp`, `header_profile.h`, and related `profiling` directory: Attempt to make `CoMMA` work with `perfetto`. **ATTENTION: Not maintained anymore!**
