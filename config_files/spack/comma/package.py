# CoMMA
#
# Copyright © 2024 ONERA
#
# Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
# Contributors: Karim Anemiche
#
# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

from spack.package import *


class Comma(CMakePackage):
    """CoMMA (COarse Mesh Multigrid Agglomerator)."""

    homepage = "https://github.com/onera/CoMMA"
    url = "https://github.com/onera/CoMMA"
    git = "https://github.com/onera/CoMMA.git"

    submodules = True
    submodules_to_delete = ["perfetto"]
    # Not sure if something as what follows is possible
    # if not run_tests:
    #     submodules_delete.append("Catch2")

    maintainers("RiMillo")

    version("develop", branch="main", submodules_delete=submodules_to_delete)
    version("1.3.2", tag="v1.3.2", submodules_delete=submodules_to_delete, preferred=True)
    version("1.3.1", tag="v1.3.1", submodules_delete=submodules_to_delete)
    # Named 1.3, but as we have a 1.3.1 that changes patches, let us do this differently
    version("1.3.0", tag="v1.3", submodules_delete=submodules_to_delete)
    version("1.2", tag="v1.2", submodules_delete=submodules_to_delete)
    version("1.1", tag="v1.1", submodules_delete=submodules_to_delete)
    version("1.0", tag="v1.0", submodules_delete=submodules_to_delete)

    # Add install methods
    patch("v1.3_install.patch", when="@1.2:1.3.0")  # Works for version 1.2 and 1.3
    patch("v1.1_install.patch", when="@1.1")
    patch("v1.0_install.patch", when="@1.0")

    # Fix problems with path
    patch("fix_pkgconfig_v1.3.1.patch", when="@1.3.1")

    variant("python", when="@1.2:", description="Install python bindings", default=True)
    variant("codaflags", when="@1.1:", description="Compile with usual CODA flags", default=False)
    variant("doc", when="@1.3.1:", description="Build Doxygen documentation", default=False)
    variant(
        "pkgconfig", when="@1.3.1:", description="Add pkg-config configuration file", default=True
    )
    variant(
        "int64",
        when="@1.3.2:",
        default=False,
        description="Set size of integer types to 64 b (true) or 32 b (false)",
    )
    variant(
        "real64",
        when="@1.3.2:",
        default=True,
        description="Set size of real types to 64 b (double, true) or 32 b (false)",
    )

    depends_on("cmake@3.15:", type=("build"))
    depends_on("doxygen", type=("build",), when="+doc")

    extends("python", when="+python")
    depends_on("python", type=("build", "link", "run"), when="+python")
    depends_on("py-pybind11@2.12.0:", type=("build", "link", "run"), when="@develop:+python")

    # older versions always had a Python dependency
    extends("python", when="@1.0:1.1")
    depends_on("python", type=("build", "link", "run"), when="@1.0:1.1")

    # Require C++17 compilers
    conflicts("%gcc@:8.1.9", msg="Compiler supporting C++17 required")
    conflicts("%clang@:5.9.9", msg="Compiler supporting C++17 required")
    conflicts("%intel@:19.9.9", msg="Compiler supporting C++17 required")

    def cmake_args(self):
        args = super(Comma, self).cmake_args()
        args += [
            self.define_from_variant("CODAFLAGS", "codaflags"),
            self.define_from_variant("BUILD_PYTHON_BINDINGS", "python"),
            self.define_from_variant("BUILD_DOC", "doc"),
            self.define_from_variant("PKGCONFIG_SUPPORT", "pkgconfig"),
            # `make test` added in v1.3.1
            self.define("BUILD_TESTS", self.run_tests and self.spec.satisfies("@1.3.1:")),
        ]

        if self.spec.satisfies("@1.3.2:"):
            bit_int = 64 if "+int64" in self.spec else 32
            args += [
                self.define("INDEX_T", "uint{}_t".format(bit_int)),
                self.define("INT_T", "int{}_t".format(bit_int)),
                self.define("REAL_T", "double" if "+real64" in self.spec else "float"),
            ]

        return args
