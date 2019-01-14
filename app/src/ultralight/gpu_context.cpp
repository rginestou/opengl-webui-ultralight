#include "gpu_context.h"
#include "gpu_driver_gl.h"

GPUContext::GPUContext(GLFWwindow* window, double scale) : window_(window) {
  driver_.reset(new ultralight::GPUDriverGL(scale));
}

GPUContext::~GPUContext() {}

ultralight::GPUDriver* GPUContext::driver() const { return driver_.get(); }

ultralight::FaceWinding GPUContext::face_winding() const { return ultralight::kFaceWinding_CounterClockwise; }

void GPUContext::BeginDrawing() {}

void GPUContext::EndDrawing() {}

// Should only be called during drawing phase
void GPUContext::PresentFrame() {
  glfwSwapBuffers(window_);
}

void GPUContext::Resize(int width, int height) {
  ultralight::GPUDriverGL* driver_gl = static_cast<ultralight::GPUDriverGL*>(driver_.get());
  driver_gl->SetViewport(width, height);
}
