# Optimizer

## Build Instructions

```bash
mkdir build
cd build
# for debug
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug ..
# for release
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release ..
make
./optimizer
```
