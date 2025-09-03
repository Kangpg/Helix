git submodule update --init --recursive

cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX="%CD%\install"

