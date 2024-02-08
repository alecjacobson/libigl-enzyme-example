# libigl 🤝 enzyme

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

Building creates the `./example` executable which you can just run:

    ./example
