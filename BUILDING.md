# Linux
Next packages are required:

- **clang** (for dev) / **gcc**

- **cmake**

- optional: [*ninja*](https://ninja-build.org/) ([generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#manual:cmake-generators(7)): can be chosen by `cmake -G <generator>`)

- optional for dev: clang-format, clang-tidy, cppcheck, doxygen, codespell


# Required packages for native build in Windows
To build under **Windows**, the [**MSYS2**](https://www.msys2.org/) environment is required. The following packages need to be installed in it:

- **mingw-w64-clang-x86_64-clang** [**(MSYS2 Packages)**](https://packages.msys2.org/package/mingw-w64-clang-x86_64-clang)

- **mingw-w64-clang-x86_64-cmake** [**(MSYS2 Packages)**](https://packages.msys2.org/package/mingw-w64-clang-x86_64-cmake)

- **mingw-w64-clang-x86_64-clang-tools-extra** [**(MSYS2 Packages)**](https://packages.msys2.org/package/mingw-w64-clang-x86_64-clang-tools-extra)

- **mingw-w64-clang-x86_64-cppcheck** [**(MSYS2 Packages)**](https://packages.msys2.org/package/mingw-w64-clang-x86_64-cppcheck)

- **mingw-w64-clang-x86_64-doxygen** [**(MSYS2 Packages)**](https://packages.msys2.org/package/mingw-w64-clang-x86_64-doxygen)

- **mingw-w64-clang-x86_64-python-codespell** [**(MSYS2 Packages)**](https://packages.msys2.org/package/mingw-w64-clang-x86_64-python-codespell)

To install them, enter the following command in the **MSYS2** terminal:

`pacman -S mingw-w64-clang-x86_64-clang mingw-w64-clang-x86_64-cmake mingw-w64-clang-x86_64-clang-tools-extra mingw-w64-clang-x86_64-cppcheck mingw-w64-clang-x86_64-doxygen mingw-w64-clang-x86_64-python-codespell`
