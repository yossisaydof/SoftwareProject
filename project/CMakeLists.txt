cmake_minimum_required(VERSION 3.15)

project(SoftwareProject C)
set(CMAKE_C_STANDARD 90)

set(GCC_COVERAGE_COMPILE_FLAGS "-ansi -Wall -Wextra -Werror -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(project spmatArray.c cluster.c divideIntoTwo.c matrixStructure.c matrixShifting.c modularityGroups.c modularityGroups.h group.h matrixStructure.h divideIntoGroups.c powerIteration.c divideIntoGroups.h divideIntoTwo.h matrixShifting.h modularityMaximization.c modularityMaximization.h)

