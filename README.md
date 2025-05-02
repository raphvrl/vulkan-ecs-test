## Compilation and Execution

To compile and run the project, simply use the provided Makefile:

1. Open a terminal in the project directory.
2. Compile the project with the command:
    ```
    make
    ```
3. Run the generated application:
    ```
    ./app.exe
    ```

> **Note:** Make sure you have a C compiler installed as well as the appropriate Vulkan SDK for your platform.  
> **Tested Environment:** This project has only been tested using MSYS2 with MinGW on Windows.

This project is an attempt to create an engine using an ECS (Entity Component System) architecture and 3D rendering with Vulkan in C.

## Objective

The initial goal was to explore designing a game engine based on ECS, while learning to use Vulkan for 3D rendering in C.

## Project Status

The project is unfinished and will likely not be continued. The main difficulty encountered was finding a suitable physics library in C. Since then, I have started working with C++ and have moved on to more interesting projects in that language.

## Features

- Early implementation of an ECS system in C
- Vulkan integration for 3D rendering
- Integration of ImGui for managing entities

## Limitations

- Unfinished project
- No integrated physics engine
- Limited documentation and examples

## Acknowledgements

Thanks to all open source resources and the community around Vulkan and ECS.

---

**Note:** This project remains available as an example or starting point for other developers interested in exploring these technologies in C.