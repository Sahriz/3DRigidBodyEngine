# RigidBodyEngine

A C++20 project for building a 3D rigid-body physics API, inspired by
[Box2D-Lite](https://github.com/erincatto/box2d-lite). The goal is a reusable CPU
simulation library with an OpenGL demo for displaying and testing rigid bodies.

The project is in early development. The demo renders cubes with a movable
camera. The physics library has its own build target; simulation is not yet
implemented.

## Requirements

The following build instructions target Windows:

- Visual Studio 2022 or Build Tools with **Desktop development with C++**,
  the MSVC v143 toolset, and a Windows SDK.
- [CMake](https://cmake.org/download/) 3.21 or newer, available on `PATH`.
- A graphics driver supporting **OpenGL 4.6**.

GLFW, GLAD, and GLM are included under `vendor/` and do not need separate
installation.

## Build and run

From the repository root, run these commands in PowerShell:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug --parallel
.\build\bin\Debug\DemoPhysicsEngine.exe
```

Press **Escape** to close the demo.

Configuration generates `build/RigidBodyEngine.sln`. Open it in Visual Studio
to work on both projects together:

- `RigidBodyEngine` is the physics library: a static library with no window,
  input, rendering, or wall-clock dependency. It is the product.
- `DemoPhysicsEngine` contains the application and renderer, links the engine,
  and is the startup project.
- `glad` and `glfw` sit under the solution's `vendor` folder and belong to the
  demo alone.

Building the demo also builds the physics library when needed.

The dependency runs one way. The demo calls the engine's API and draws the
results; the engine never includes a demo header. `engine/CMakeLists.txt`
publishes only `engine/include` to consumers, and the demo's `src/` include
path is private to the demo, so the boundary is enforced by the build rather
than by discipline.

For a release build:

```powershell
cmake --build build --config Release --parallel
.\build\bin\Release\DemoPhysicsEngine.exe
```

Keep the source `shaders/` directory available when running the executable.
If you move the project, rerun configuration and rebuild so the shader path is
updated.

## Build only the physics library

The engine uses C++20 and the bundled GLM headers, with no OpenGL, GLFW, or GLAD
dependencies. After configuring the project, build only the library with:

```powershell
cmake --build build --config Debug --target RigidBodyEngine --parallel
```

This creates `build/lib/Debug/RigidBodyEngine.lib`. Public API headers go in
`engine/include/RigidBodyEngine/`; implementation files go in `engine/src/`.

To check that the library has not quietly picked up a graphics dependency,
configure it without the demo. GLFW, GLAD, and OpenGL are then never looked for
at all, so the configure fails if physics code has started needing them:

```powershell
cmake -S . -B build-engine -DRBE_BUILD_DEMO=OFF
cmake --build build-engine --config Debug --parallel
```

Configuring `engine/` directly works the same way, and proves the library builds
with no knowledge of this repository's application:

```powershell
cmake -S engine -B build-engine-only
cmake --build build-engine-only --config Debug --parallel
```
