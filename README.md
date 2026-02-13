# video-ascii

A small C++/OpenCV application that plays a video as real-time ASCII animation in a terminal.

## Features

- Preserves source video aspect ratio when converting to terminal cells.
- Cross-platform build with CMake (Linux/macOS/Windows).
- Optional ANSI true-color rendering.
- Adjustable output width and character ramp.

## Project structure

- `include/AsciiRenderer.hpp`: renderer API and options.
- `src/AsciiRenderer.cpp`: frame conversion + timing playback loop.
- `src/main.cpp`: command-line interface.
- `CMakeLists.txt`: build setup.

## Build

### 1) Install dependencies

You need:

- CMake >= 3.16
- A C++17 compiler
- OpenCV development package

Examples:

- **Ubuntu/Debian**: `sudo apt install cmake g++ libopencv-dev`
- **macOS (Homebrew)**: `brew install cmake opencv`
- **Windows (vcpkg)**: `vcpkg install opencv`

### 2) Configure + compile

```bash
cmake -S . -B build
cmake --build build --config Release
```

## Run

```bash
./build/video-ascii /path/to/video.mp4 --width 120
```

On Windows:

```powershell
.\build\Release\video-ascii.exe C:\path\to\video.mp4 --width 120
```

## CLI options

- `--width N` : output width in characters (default `120`).
- `--color` : enable ANSI true-color output.
- `--invert` : invert brightness mapping.
- `--chars <ramp>` : custom dark-to-bright character ramp.

Example:

```bash
./build/video-ascii input.mp4 --width 140 --color --chars " .:-=+*#%@"
```

## Notes on aspect ratio

Terminal character cells are not square. The renderer applies a cell-aspect correction so circles remain visually closer to circles instead of appearing stretched.

If your terminal font is unusually wide/tall, tweak `kTerminalCellAspect` in `src/AsciiRenderer.cpp`.
