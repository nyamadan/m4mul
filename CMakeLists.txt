cmake_minimum_required(VERSION 3.0.0)
project(m4mul VERSION 0.0.0)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "\
-Wall \
-std=c++11 \
-O3 \
-s WASM=1 \
-s EXPORTED_FUNCTIONS=\"[ \
'_CreateMatrix', \
'_TransposeMatrix', \
'_MultiplyMatrix', \
'_PrintMatrix', \
'_ReleaseMatrix' \
]\" \
")
add_executable(m4mul m4mul.cpp)
