cmake_minimum_required(VERSION 3.15)
project(van_Emde_Boas C)

set(CMAKE_C_STANDARD 99)

add_executable(van_Emde_Boas van_emde_boas.c van_emde_boas.h test_van_emde_boas.c)