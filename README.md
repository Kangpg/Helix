# Helix

## Overview
Helix is a Windows-focused C++20 playground for building an IOCP-driven game server. The codebase is split between a reusable `HelixCore` library that provides the low-level primitives (logging, Winsock lifecycle helpers, and foundational interfaces) and a thin `GameServer` executable that wires everything together.

## Repository layout
```
Helix/
├─ CMakeLists.txt            # Root CMake build script
├─ CMakePresets.json         # Helpful presets for IDEs and command-line builds
├─ GameServer/               # Executable project
│  ├─ GameServer.cpp/.h      # Concrete server that reads config and applies bind address
│  ├─ main.cpp               # Process entry point and bootstrap logic
│  └─ config/server.ini      # Default configuration shipped with the binary
├─ HelixCore/                # Shared library with common infrastructure
│  ├─ AllocationOverride.cpp # Global new/delete overrides backed by mimalloc
│  ├─ Logger.*               # Thread-safe logging utility with stack-trace support
│  ├─ Network.*              # Winsock initialization helpers and AcceptEx/ConnectEx loaders
│  ├─ Noncopyable.*          # Small mixin to forbid copy semantics
│  └─ Server.*               # Abstract base server with lifecycle hooks
├─ ThirdParty/               # Placeholder for vendor drops (actual code via vcpkg)
├─ launch_debug.bat          # Helper script for Visual Studio debug builds
├─ launch_release.bat        # Helper script for Visual Studio release builds
└─ vcpkg.json                # Declares the external dependencies pulled through vcpkg
```

## Build requirements
* **Toolchain:** Visual Studio 2022 (or any MSVC toolchain with C++20 support) and CMake 3.16+
* **Package manager:** [vcpkg](https://github.com/microsoft/vcpkg) with manifests enabled so the declared dependencies (`unofficial-inih`, `libcurl`, `mimalloc`) resolve automatically.

> ℹ️ The project is currently Windows-only because it depends on the Win32 API, Winsock extensions, and Microsoft-specific pragmas.

## Building
1. Install the required vcpkg packages by running CMake from a developer prompt with `VCPKG_ROOT` configured (manifests handle the rest).
2. Configure the project:
   ```powershell
   cmake --preset=default
   ```
   or manually:
   ```powershell
   cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
   ```
3. Build the solution:
   ```powershell
   cmake --build build --config Release
   ```

## Configuration
Runtime settings live in `GameServer/config/server.ini`. On startup the executable locates its `config` directory, loads the INI file via **inih**, and applies the bind endpoint before the server begins running.

```ini
[server]
ip=0.0.0.0
port=5000
```

Feel free to extend the INI file with additional sections; hook them up inside `GameServer::LoadConfig`.

## Logging
`HelixCore::Logger` writes timestamped, thread-aware logs (and optional stack traces) to a per-day file next to the executable. Initialize it early in `main` to ensure any subsequent subsystems can log diagnostics.

## Next steps
The networking primitives (`HelixCore::Network` and the IOCP interfaces) currently expose the essential building blocks. Future work will flesh out the acceptor, session dispatching, and gameplay-specific logic on top of the existing scaffolding.
