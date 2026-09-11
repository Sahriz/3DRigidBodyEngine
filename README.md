# RigidBodyEngine

A C++20 project for building a 3D rigid-body physics API, inspired by
[Box2D-Lite](https://github.com/erincatto/box2d-lite). The goal is a reusable CPU
simulation library with an OpenGL demo for displaying and testing rigid bodies.

The project is in early development. The current demo renders a triangle.

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
.\build\bin\Debug\RigidBodyEngine.exe
```

Press **Escape** to close the demo.

For a release build:

```powershell
cmake --build build --config Release --parallel
.\build\bin\Release\RigidBodyEngine.exe
```

Keep the source `shaders/` directory available when running the executable.
If you move the project, rerun configuration and rebuild so the shader path is
updated.
