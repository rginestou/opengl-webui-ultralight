#pragma once
#include "../gl/glinclude.h"
#include <GLFW/glfw3.h>

#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/GPUDriver.h>
#include <memory>

class GPUContext {
public:
  GPUContext(GLFWwindow* window, double scale);
  ~GPUContext();

  ultralight::GPUDriver* driver() const;
  ultralight::FaceWinding face_winding() const;

  void BeginDrawing();
  void EndDrawing();

  // Should only be called during drawing phase
  void PresentFrame();
  void Resize(int width, int height);

private:
  std::unique_ptr<ultralight::GPUDriver> driver_;
  GLFWwindow* window_;
};
