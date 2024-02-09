# libigl 🤝 [enzyme](https://github.com/EnzymeAD/Enzyme)

## Build

Enzyme requires matching to its version of llvm. On Mac its not obvious how to
use the built in clang/llvm, so instead we'll assume you have installed llvm and
enzyme via homebrew:


    brew install llvm enzyme

Then you can build with the usual

    mkdir build
    cd build
    cmake ../
    make

## Run


From within the `build` directory just issue:

    ./example

A glfw app should launch displaying a knight with white lines indicating the
computed gradients.

![](decimated-knight.png)

## Known Issues

At the moment, Enzyme [will crash at compile time differentiating
programs](https://github.com/EnzymeAD/Enzyme/issues/1679) that contain calls to
`std::cerr<<`.

Libigl as of
[fe65ecb](https://github.com/libigl/libigl/commit/fe65ecb907730f96d99a92ebd9c5e852633cc990)
(e.g., pulled by this repos build) avoids `std::cerr` but some functions will
still do it (e.g., when a solver or meshIO fails).
