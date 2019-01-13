#pragma once
#include "gpu_driver_gl.h"
#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/FileSystem.h>
#include <Ultralight/platform/FontLoader.h>
#include <Ultralight/platform/GPUDriver.h>

ultralight::FontLoader* CreatePlatformFontLoader();
ultralight::FileSystem* CreatePlatformFileSystem(const char* baseDir);

class GPUContext {
protected:
  std::unique_ptr<ultralight::GPUDriver> driver_;
  GLFWwindow* window_;

public:
  GPUContext(GLFWwindow* glfw_window) : window_(glfw_window) {
    driver_.reset(new ultralight::GPUDriverGL(1.0));
  }

  ~GPUContext() {}

  ultralight::GPUDriver* driver() const { return driver_.get(); }
  ultralight::FaceWinding face_winding() const { return ultralight::kFaceWinding_CounterClockwise; }

  void resize(int width, int height) {
    ultralight::GPUDriverGL* driver_gl = static_cast<ultralight::GPUDriverGL*>(driver_.get());
    driver_gl->SetViewport(width, height);
  }
};
