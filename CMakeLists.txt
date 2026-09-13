cmake_minimum_required(VERSION 3.20)

# This file is the solution, not a project. It holds the settings every project
# shares and nothing else; each project describes itself in its own directory:
#
#   vendor/   third-party code (glm, plus the demo's graphics libraries)
#   engine/   RigidBodyEngine   -- the physics library. The product.
#   src/      DemoPhysicsEngine -- the application that drives the library.
#
# The dependency arrow points one way: the demo links the engine, the engine
# knows nothing about the demo. The targets enforce that rather than the folder
# names -- engine/ publishes only engine/include, and the demo's include path is
# never handed to the engine target, so engine code physically cannot reach
# demo headers even by accident.
project(RigidBodyEngine VERSION 0.1.0 LANGUAGES CXX)

option(RBE_BUILD_DEMO "Build DemoPhysicsEngine, the OpenGL demo application" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# glad ships as a .c file, so the C compiler must be enabled -- but only when
# the demo is part of the build. A library-only configure needs no C compiler.
if (RBE_BUILD_DEMO)
    enable_language(C)
endif()

# One output layout regardless of generator. The Visual Studio generator would
# nest output under build/<config>, Ninja puts it in build/ -- pinning it here
# means you can switch generators without anything silently moving.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Solution folders, so the vendor projects do not sit next to yours in the
# Visual Studio tree.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(vendor)
add_subdirectory(engine)

if (RBE_BUILD_DEMO)
    add_subdirectory(src)

    # VS_STARTUP_PROJECT is only read from the top-level directory, so it has to
    # be set here rather than next to the target it names.
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT DemoPhysicsEngine)
endif()
