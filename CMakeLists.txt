cmake_minimum_required(VERSION 3.10)

project(N-BodySim VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(raylib REQUIRED)

add_executable(nbody src/main.cpp src/sim.cpp src/body.cpp)

target_include_directories(nbody PRIVATE ${CMAKE_SOURCE_DIR}/include)

option(OMP "Abilita il multithreading con OpenMP" OFF)
message(STATUS "OpenMP multi-core: ${OMP}")

if(OMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(nbody PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(nbody PRIVATE USE_OMP)
else()
    if(MSVC)
        target_compile_options(nbody PRIVATE /openmp:experimental)
    else()
        target_compile_options(nbody PRIVATE -fopenmp-simd)
    endif()
endif()

if(MSVC)
    target_compile_options(nbody PRIVATE /O2 /arch:AVX2 /fp:fast)
else()
    target_compile_options(nbody PRIVATE -O3 -march=native -ffast-math)
endif()

target_link_libraries(nbody PRIVATE raylib)

if(UNIX AND NOT APPLE)
    target_link_libraries(nbody PRIVATE m)
endif()

set_target_properties(nbody PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")


# TEST 
# add_custom_target(run
#     COMMAND $<TARGET_FILE:nbody> $$ARGS
#     DEPENDS nbody
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
# )

add_custom_target(galaxy1
    COMMAND $<TARGET_FILE:nbody> -glxy -blend --tilt2 25 3000
    DEPENDS nbody
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(galaxy2
    COMMAND $<TARGET_FILE:nbody> -glxy -blend --tilt2 90 --tilt1 -12 1000 -3D
    DEPENDS nbody
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
