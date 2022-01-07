cmake_minimum_required(VERSION 3.21)
project(sqlittle C)

set(CMAKE_C_STANDARD 23)

add_executable(sqlittle sqlittle.c sl_core.c sl_core.h sl_storage.c sl_storage.h)
