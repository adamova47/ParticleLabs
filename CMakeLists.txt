cmake_minimum_required (VERSION 3.24)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(ParticleLabs
    VERSION 0.1.0
    LANGUAGES CXX CUDA
)

# C++ configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CUDA configuration
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_EXTENSIONS OFF)

# Point to your raylib source folder
add_subdirectory("C:/raylib/raylib" raylib_build)

# NVIDIA CUDA runtime
find_package(CUDAToolkit REQUIRED)

# Defines the executable
add_executable (ParticleLabs
    src/main.cpp
    "src/ParticleSystem.cpp"
    "src/Particle.cpp"
)

# Link CUDA runtime
target_link_libraries(ParticleLabs PRIVATE 
    raylib
    CUDA::cudart
)

# RTX 3070 Laptop GPU: compute capability 8.6
set_target_properties(ParticleLabs PROPERTIES
    CUDA_ARCHITECTURES 86
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ParticleLabs PROPERTY CXX_STANDARD 20)
endif()

# target_include_directories() tells CMake which directories the compiler should search for headers
target_include_directories(ParticleLabs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)