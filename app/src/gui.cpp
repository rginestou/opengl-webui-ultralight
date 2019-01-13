#include "gui.h"
#include "window.h"

#include <Ultralight/Buffer.h>
#include <Ultralight/Renderer.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/Platform.h>
#include <iostream>

using namespace ultralight;

GUI::GUI(Window& window) : _window(window) {
  _gpu_context.reset(new GPUContext(_window.handle()));

  ultralight::Config config;
  config.face_winding = _gpu_context->face_winding();
  config.device_scale_hint = _window.scale();

  // Setup our Platform API handlers
  ultralight::Platform& platform = ultralight::Platform::instance();
  platform.set_config(config);
  platform.set_gpu_driver(_gpu_context->driver());
  platform.set_font_loader(CreatePlatformFontLoader());
  platform.set_file_system(CreatePlatformFileSystem("assets/"));

  _driver = _gpu_context->driver();
  _renderer = ultralight::Renderer::Create();
  _view = _renderer->CreateView(window.width(), window.height(), true);

  // Screen geometry
  _vertices.resize(4);
  _indices = { 0, 3, 1, 1, 3, 2 };
  _geometry_id = _driver->NextGeometryId();

  Matrix identity;
  identity.SetIdentity();

  RenderTarget target = _view->render_target();
  _gpu_state.viewport_width = (float)_window.width();
  _gpu_state.viewport_height = (float)_window.height();
  _gpu_state.transform = ConvertAffineTo4x4(identity);
  _gpu_state.enable_blend = true;
  _gpu_state.enable_texturing = true;
  _gpu_state.shader_type = kShaderType_Fill;
  _gpu_state.render_buffer_id = 0; // default render target view (screen)
  _gpu_state.texture_1_id = target.texture_id;

  // At this point, GL Context is defined
  updateGeometry();

  _view->set_view_listener(this);
  _view->set_load_listener(this);

  // GUI file
  _view->LoadURL("file:///gui.html");
}

GUI::~GUI() {
  // Destroy our Platform handlers
  ultralight::Platform& platform = ultralight::Platform::instance();
  delete platform.file_system();
  platform.set_file_system(nullptr);
  delete platform.font_loader();
  platform.set_font_loader(nullptr);
  platform.set_gpu_driver(nullptr);

  _driver->DestroyGeometry(_geometry_id);

  _view->set_load_listener(nullptr);
  _view->set_view_listener(nullptr);
}

void GUI::OnDOMReady(ultralight::View* caller) {
  SetJSContext(_view->js_context());
  JSObject global = JSGlobalObject();

  // C++ -> JS
  setButtonText = global["setButtonText"];

  // JS -> C++
  global["onButtonClick"] = BindJSCallback(&GUI::onButtonClick);
}

void GUI::draw() {
  _renderer->Update();
  _driver->BeginSynchronize();

  _renderer->Render();
  _driver->DrawCommandList();

  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_NEVER);

  _driver->DrawGeometry(_geometry_id, 6, 0, _gpu_state);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  _driver->EndSynchronize();
}

void GUI::fireKeyEvent(const ultralight::KeyEvent& evt) {
  _view->FireKeyEvent(evt);
}

void GUI::fireMouseEvent(const ultralight::MouseEvent& evt) {
  _view->FireMouseEvent(evt);
}

void GUI::fireScrollEvent(const ultralight::ScrollEvent& evt) {
  _view->FireScrollEvent(evt);
}

void GUI::resize(int width, int height) {
  _gpu_context->resize(width, height);
  _view->Resize(width, height);

  updateGeometry();

  // Update these now since they were invalidated
  RenderTarget target = _view->render_target();
  _gpu_state.texture_1_id = target.texture_id;
  _gpu_state.viewport_width = (float)_window.width();
  _gpu_state.viewport_height = (float)_window.height();
}

void GUI::OnChangeCursor(ultralight::View* caller,
    ultralight::Cursor cursor) {
  _window.setCursor(cursor);
}

void GUI::onButtonClick(const JSObject& obj, const JSArgs& args) {
  _listener->onButtonClick();
}

void GUI::onShowEvent(bool show) {
  const char* text = show ? "Hide" : "Show";
  setButtonText({ text });
}

void GUI::updateGeometry() {
  RenderTarget target = _view->render_target();

  Vertex_2f_4ub_2f_2f_28f v;
  memset(&v, 0, sizeof(v));

  v.data0[0] = 1; // Fill Type: Image

  v.color[0] = 255;
  v.color[1] = 255;
  v.color[2] = 255;
  v.color[3] = 255;

  float left = 0.0f;
  float top = 0.0f;
  float right = (float)_window.width();
  float bottom = (float)_window.height();

  // TOP LEFT
  v.pos[0] = v.obj[0] = left;
  v.pos[1] = v.obj[1] = top;
  v.tex[0] = target.uv_coords.left;
  v.tex[1] = target.uv_coords.top;

  _vertices[0] = v;

  // TOP RIGHT
  v.pos[0] = v.obj[0] = right;
  v.pos[1] = v.obj[1] = top;
  v.tex[0] = target.uv_coords.right;
  v.tex[1] = target.uv_coords.top;

  _vertices[1] = v;

  // BOTTOM RIGHT
  v.pos[0] = v.obj[0] = right;
  v.pos[1] = v.obj[1] = bottom;
  v.tex[0] = target.uv_coords.right;
  v.tex[1] = target.uv_coords.bottom;

  _vertices[2] = v;

  // BOTTOM LEFT
  v.pos[0] = v.obj[0] = left;
  v.pos[1] = v.obj[1] = bottom;
  v.tex[0] = target.uv_coords.left;
  v.tex[1] = target.uv_coords.bottom;

  _vertices[3] = v;

  ultralight::VertexBuffer vbuffer;
  vbuffer.format = ultralight::kVertexBufferFormat_2f_4ub_2f_2f_28f;
  vbuffer.size = (uint32_t)(sizeof(ultralight::Vertex_2f_4ub_2f_2f_28f) * _vertices.size());
  vbuffer.data = (uint8_t*)_vertices.data();

  ultralight::IndexBuffer ibuffer;
  ibuffer.size = (uint32_t)(sizeof(ultralight::IndexType) * _indices.size());
  ibuffer.data = (uint8_t*)_indices.data();

  if (!_initialized) {
    _driver->CreateGeometry(_geometry_id, vbuffer, ibuffer);
    _initialized = true;
    return;
  }

  _driver->UpdateGeometry(_geometry_id, vbuffer, ibuffer);
}
