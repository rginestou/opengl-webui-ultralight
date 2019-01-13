# OpenGL WebUI - Ultralight

This project is a simple C++ OpenGL GLFW application with a HTML/CSS UI overlay powered by the very promising [Ultralight](https://github.com/ultralight-ux/ultralight) library.

> **Motivation**
>
> The extensive use of web browsers (desktop and mobile) to access services and information has driven web UIs to be extremely powerful, user friendly and beautiful. The underlying technologies (HTML, CSS, ...) are very easy to use as a developer. On the other hand, the lack of nice, efficient and simple UI technologies for native desktop applications only adds to the contrast between the web and desktop worlds.
>
> Electron-like technologies are a first step toward bringing web UIs to the desktop, however it comes at the cost of performances. There _has_ to be a way to bring together web UIs technologies and low level backend programming languages...
>
> After many attemps to find the right set of tools to tackle this issue, I've come across the **Ultralight** librabry, which promised to do just that. This project is a boilerplate code that demonstrates the use of a web based UI overlay on top of a low level bare C++ OpenGL context.

## Setup the project

First, **clone the repository**. It only contains the projects sources.

Then, fetch the official [Ultralight](https://github.com/ultralight-ux/ultralight) repositiory, and copy the `bin` and `include` folders of the library.

**GLEW** and **OpenGL** are also required, make sure to install them on your system.

The folder structure should now look like this now:

    /
    ----/app
    ----/bin
    ----/deps
    ----/include

Use the standard CMake procedure to build the application:

    mkdir build
    cd build
    cmake ..
    make

Run the app from within its target folder:

    cd app
    ./App
