# Conway's Game of Life Simulation

A modern implementation of Conway's Game of Life using Raylib, featuring a beautiful and interactive interface.

## Features

- Interactive grid-based simulation
- Fullscreen support
- Customizable grid size
- Smooth rendering
- Modern UI with dark theme
- Real-time simulation controls

## Requirements

- Windows operating system
- Raylib library
- CMake (version 3.10 or higher)
- C++ compiler (compatible with C++14 or later)

## Installation

1. Clone this repository:
```bash
git clone https://github.com/danadrianmirea/GameOfLife
cd RayGameOfLife
```

2. Ensure you have Raylib installed on your system

3. Build the project using CMake:
```bash
# Create a build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build the project
cmake --build .
```

The executable will be created in the `build/bin` directory.

## Usage

Run the executable:
```bash
# On Windows
build/bin/game.exe

# On Linux
build/bin/game
```

### Controls
- Left-click: Toggle cell state (alive/dead)
- Space: Pause/Resume simulation
- R: Reset the grid
- F: Toggle fullscreen
- ESC: Exit the application

## Project Structure

```
RayGameOfLife/
├── src/           # Source code
├── lib/           # Library dependencies
├── Font/          # Font assets
├── CMakeLists.txt # Build configuration
└── LICENSE.txt    # MIT License
```

## License

This project is licensed under the MIT License - see the [LICENSE.txt](LICENSE.txt) file for details.

Big thanks to https://github.com/educ8s (https://www.youtube.com/@programmingwithnick) for making great Raylib tutorials on youtube.
