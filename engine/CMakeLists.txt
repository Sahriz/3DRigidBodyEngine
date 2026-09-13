cmake_minimum_required(VERSION 3.20)

# RigidBodyEngine -- the physics library, and the reason the repository exists.
#
# It is a plain static library with no window, no input, no rendering and no
# wall-clock: the application hands it objects and asks it to step, and reads
# the resulting poses back out. Nothing in here may include a demo header or
# link a graphics library.
#
# The solution in the parent directory adds this project, but configuring this
# directory on its own also works and is the cheapest possible proof that the
# library does not depend on the demo:
#
#   cmake -S engine -B build-engine && cmake --build build-engine
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(RigidBodyEngine VERSION 0.1.0 LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

    # Pull in glm only. RBE_BUILD_DEMO=OFF makes vendor/ skip OpenGL, GLFW and
    # glad entirely, so this configure fails if physics code ever starts
    # needing them.
    set(RBE_BUILD_DEMO OFF)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../vendor" "${CMAKE_BINARY_DIR}/vendor")
endif()

# CONFIGURE_DEPENDS re-runs the glob when files appear or disappear, so adding
# engine/src/Contact.cpp needs no edit here. The cost is a stat of the tree on
# every build -- free at this scale.
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_library(RigidBodyEngine STATIC ${ENGINE_SOURCES})

# Consumers link the namespaced name. It is the spelling a find_package() would
# give them later, so call sites do not change if this ever ships installed.
add_library(RigidBodyEngine::RigidBodyEngine ALIAS RigidBodyEngine)

target_compile_features(RigidBodyEngine PUBLIC cxx_std_20)
set_target_properties(RigidBodyEngine PROPERTIES CXX_EXTENSIONS OFF)

# This is the API boundary. include/ is PUBLIC, so the demo sees exactly the
# headers under engine/include/RigidBodyEngine/ and nothing else; src/ is
# PRIVATE, so implementation headers stay unreachable from outside.
target_include_directories(RigidBodyEngine
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# PUBLIC because glm types appear in the public headers (positions, velocities,
# orientations), so anyone who includes them needs glm on their include path.
target_link_libraries(RigidBodyEngine PUBLIC glm::glm)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ENGINE_SOURCES})

if (MSVC)
    target_compile_options(RigidBodyEngine PRIVATE /W4 /permissive-)
else()
    target_compile_options(RigidBodyEngine PRIVATE -Wall -Wextra)
endif()
