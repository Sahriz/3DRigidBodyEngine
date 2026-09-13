# Third-party code. Every dependency is wrapped in a target here so that
# engine/ and src/ link the same names and nobody hardcodes a vendor path.
#
# Only glm is unconditional. It is header-only math and the single dependency
# the physics library is allowed to have. OpenGL, GLFW and glad belong to the
# demo, and are not even configured when the demo is switched off -- that is
# what keeps RBE_BUILD_DEMO=OFF an honest test of the library's independence.

# The include root is vendor/glm because the headers live at vendor/glm/glm/,
# so they resolve as <glm/gtc/matrix_transform.hpp> -- the spelling the glm docs
# and every tutorial use. SYSTEM keeps glm's own warnings out of /W4 output.
add_library(glm INTERFACE)
add_library(glm::glm ALIAS glm)
target_include_directories(glm SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/glm")

if (NOT RBE_BUILD_DEMO)
    return()
endif()

# ------------------------------------------------------- demo graphics ---
# OpenGL itself is not vendored -- it comes from the driver, and the demo finds
# it in src/CMakeLists.txt, because find_package's imported targets are only
# visible in the directory that created them.

# GLFW builds its Doxygen docs whenever doxygen.exe is on PATH -- Strawberry
# Perl ships one -- costing configure time for output nobody reads.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# glad is a single translation unit. Giving it its own static library keeps its
# warnings out of the demo's output and stops it recompiling whenever a demo
# header changes.
add_library(glad STATIC "${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c")
target_include_directories(glad SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glad/include")

set_target_properties(glad glfw PROPERTIES FOLDER "vendor")
