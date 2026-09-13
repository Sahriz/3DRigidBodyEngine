# DemoPhysicsEngine -- the application. It owns the window, input, timing and
# rendering, and it is the only half of the build that knows OpenGL exists.
# Anything physical it wants done, it asks RigidBodyEngine to do.
#
# Everything under src/ belongs to the demo, so one recursive glob is the whole
# story. Physics implementation goes in engine/src/, never here.
# OpenGL comes from the graphics driver rather than vendor/. The imported
# target it defines is scoped to this directory, which is where it is used.
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE DEMO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# Listed so they appear in the IDE tree; HEADER_FILE_ONLY stops any generator
# from handing them to a compiler.
set(RBE_SHADER_DIR "${CMAKE_SOURCE_DIR}/shaders")
file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
    "${RBE_SHADER_DIR}/*.vert"
    "${RBE_SHADER_DIR}/*.frag"
    "${RBE_SHADER_DIR}/*.comp"
    "${RBE_SHADER_DIR}/*.glsl"
)
set_source_files_properties(${SHADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(DemoPhysicsEngine ${DEMO_SOURCES} ${SHADER_FILES})

# The demo's own headers are spelled from src/, as in "render/Renderer.hpp".
# This path is private to this target, which is what stops the engine -- or
# anything else -- from ever including a demo header.
target_include_directories(DemoPhysicsEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# glm arrives transitively through the engine, but the demo uses it directly in
# Transform and Camera, so it is named here too: the demo should still compile
# if the engine ever stops exposing glm in its public headers.
target_link_libraries(DemoPhysicsEngine PRIVATE
    RigidBodyEngine::RigidBodyEngine
    glm::glm
    OpenGL::GL
    glfw
    glad
)

# Absolute path to the *source* shader folder, baked in at compile time. The
# program then finds its shaders whatever the working directory is (the classic
# "runs in VS, breaks in VS Code" bug), and because it points at the source tree
# rather than a copy, a shader edit can be reloaded without rebuilding.
target_compile_definitions(DemoPhysicsEngine PRIVATE
    SHADER_DIR="${RBE_SHADER_DIR}/"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "src" FILES ${DEMO_SOURCES})
source_group("shaders" FILES ${SHADER_FILES})

if (MSVC)
    target_compile_options(DemoPhysicsEngine PRIVATE /W4 /permissive-)
    # Only affects the Visual Studio generator; VS Code needs the same value as
    # "cwd" in .vscode/launch.json. Harmless to keep both.
    set_property(TARGET DemoPhysicsEngine PROPERTY
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
else()
    target_compile_options(DemoPhysicsEngine PRIVATE -Wall -Wextra)
endif()
