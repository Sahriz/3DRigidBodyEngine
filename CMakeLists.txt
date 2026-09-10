cmake_minimum_required(VERSION 3.20)

# LANGUAGES C CXX -- glad ships as a .c file, so the C compiler must be
# enabled or the build fails on vendor/glad/src/glad.c.
project(RigidBodyEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# One binary location regardless of generator. The Visual Studio generator
# would nest output in build/Debug, Ninja puts it in build/ -- pinning it
# here means the runtime layout is identical either way, so you can switch
# generators without anything silently moving.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(OpenGL REQUIRED)

# ---------------------------------------------------------------- vendor ---
# GLFW builds its Doxygen docs by default whenever Doxygen is on PATH. You
# have Strawberry Perl installed, which ships doxygen.exe, so this was
# costing ~10s of configure time for output nobody looks at.
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)

# Glad is a single translation unit. Compiling it as its own static library
# keeps its warnings out of your target's output and stops it being
# recompiled every time you touch a header.
add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# glm is header-only here, so an INTERFACE target is all that's needed --
# no add_subdirectory, nothing to build.
#
# NOTE: the include root is vendor/glm, and glm itself lives at
# vendor/glm/glm, so headers resolve as <glm/gtc/matrix_transform.hpp>.
# That is the spelling every tutorial and the glm docs use. Your older
# projects vendored the inner folder directly and had to write <gtc/...>
# instead -- same library, non-standard path.
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE vendor/glm)

# ----------------------------------------------------------------- target ---
# CONFIGURE_DEPENDS re-runs the glob whenever files appear or disappear, so
# adding src/physics/Contact.cpp needs no edit here. The cost is a stat of
# the source tree on every build -- free at this scale. On a large codebase
# you would list sources explicitly instead, because the stat is not free
# and an explicit list is a reviewable record of what ships.
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.comp"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl"
)
# Listed so they show up in the IDE tree; HEADER_FILE_ONLY stops any
# generator trying to hand them to a compiler.
set_source_files_properties(${SHADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(RigidBodyEngine ${APP_SOURCES} ${SHADER_FILES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "src" FILES ${APP_SOURCES})
source_group("shaders" FILES ${SHADER_FILES})

target_include_directories(RigidBodyEngine PRIVATE src)

target_link_libraries(RigidBodyEngine PRIVATE
    OpenGL::GL
    glfw
    glad
    glm
)

# Absolute path to the *source* shader folder, baked in at compile time.
# Two benefits: the program finds its shaders whatever the working directory
# is (the classic "runs in VS, breaks in VS Code" bug), and because it points
# at the source tree rather than a copy, you can edit a shader and hot-reload
# it without rebuilding.
target_compile_definitions(RigidBodyEngine PRIVATE
    SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)

if (MSVC)
    target_compile_options(RigidBodyEngine PRIVATE /W4 /permissive-)
    # Only affects the Visual Studio generator; VS Code needs the same value
    # as "cwd" in .vscode/launch.json. Harmless to keep both.
    set_property(TARGET RigidBodyEngine PROPERTY
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
        VS_STARTUP_PROJECT RigidBodyEngine)
else()
    target_compile_options(RigidBodyEngine PRIVATE -Wall -Wextra)
endif()
