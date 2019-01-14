link_directories("${ULTRALIGHT_LIBRARY_DIRS}")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

if (NOT APPLE)
    link_libraries(glfw ${OPENGL_LIBRARIES} GLEW UltralightCore Ultralight WebCore)
else ()
    # On macOS the last two libraries are Frameworks so we must pass special flags to the linker
    link_libraries(glfw ${OPENGL_LIBRARIES} GLEW UltralightCore "-framework Ultralight -framework WebCore")
endif()

include_directories("${ULTRALIGHT_INCLUDE_DIRS}")
include_directories(${glfw_INCLUDE_DIRS})
include_directories("${GLFW_SOURCE_DIR}/deps")

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

set(APP_SRC
    "src/app.h"
    "src/app.cpp"
    "src/engine.h"
    "src/engine.cpp"
    "src/gui.h"
    "src/gui.cpp"
    "src/window.h"
    "src/window.cpp"
    "src/main.cpp"
    "src/listener.h")

set(ULTRALIGHT_BINDINGS_SRC
    "src/ultralight/gpu_context.h"
    "src/ultralight/js_helpers.h"
    "src/ultralight/js_helpers.cpp"
    "src/ultralight/gpu_driver_gl.h"
    "src/ultralight/gpu_driver_gl.cpp")

set(ULTRALIGHT_SHADER_SRC
    "src/ultralight/shaders/fill.frag"
    "src/ultralight/shaders/fill_path.frag"
    "src/ultralight/shaders/v2f_c4f_t2f.vert"
    "src/ultralight/shaders/v2f_c4f_t2f_t2f_d28f.vert")

if (WIN32)
list(APPEND ULTRALIGHT_SRC
    "src/common/FileSystemBasic.h"
    "src/common/FileSystemBasic.cpp"
    "src/win/FontLoaderWin.h"
    "src/win/FontLoaderWin.cpp")
elseif (APPLE)
list(APPEND ULTRALIGHT_SRC
    "src/ultralight/mac/FileSystemMac.h"
    "src/ultralight/mac/FileSystemMac.mm"
    "src/ultralight/mac/FontLoaderMac.h"
    "src/ultralight/mac/FontLoaderMac.mm")

set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/bin/mac")
set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/bin/mac")
elseif (UNIX)
list(APPEND ULTRALIGHT_SRC
    "src/ultralight/common/FileSystemBasic.h"
    "src/ultralight/common/FileSystemBasic.cpp"
    "src/ultralight/common/FontLoaderRoboto.h"
    "src/ultralight/common/FontLoaderRoboto.cpp")

set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/bin/linux")
set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/bin/linux")
endif ()

add_executable(App WIN32 MACOSX_BUNDLE
    ${APP_SRC} ${ULTRALIGHT_BINDINGS_SRC} ${ULTRALIGHT_SRC} ${ULTRALIGHT_SHADER_SRC} ${GLAD_SRC})

# Copy all binaries to target directory
add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ULTRALIGHT_BINARY_DIRS} $<TARGET_FILE_DIR:App>)

if (NOT APPLE)
    # Copy assets to target directory
    add_custom_command(TARGET App POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:App>/assets)
    # Copy GLSL shaders to target directory
    add_custom_command(TARGET App POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/ultralight/shaders $<TARGET_FILE_DIR:App>/assets/shaders)

    if (MSVC)
        # Tell MSVC to use main instead of WinMain for Windows subsystem executables
        set_target_properties(App PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    endif()
else()
    # Copy assets to target directory
    add_custom_command(TARGET App POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:App>/../Resources/assets)
    # Copy GLSL shaders to target directory
    add_custom_command(TARGET App POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/ultralight/shaders $<TARGET_FILE_DIR:App>/../Resources/assets/shaders)
endif ()
