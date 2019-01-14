#pragma once
#include "ultralight/gpu_context.h"
#include "ultralight/js_helpers.h"
#include "window.h"
#include "listener.h"

#include <Ultralight/Renderer.h>
#include <Ultralight/View.h>
#include <Ultralight/platform/GPUDriver.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

class GUI : public ultralight::ViewListener,
            public ultralight::LoadListener,
            public EngineListener {
public:
  GUI(Window& window);
  ~GUI();

  void set_listener(GUIListener* listener) { _listener = listener; }
  GUIListener* listener() { return _listener; }

  void draw();
  void fireKeyEvent(const ultralight::KeyEvent& evt);
  void fireMouseEvent(const ultralight::MouseEvent& evt);
  void fireScrollEvent(const ultralight::ScrollEvent& evt);
  void resize(int width, int height);

  // Inherited from ultralight::Listener::View
  void OnChangeCursor(ultralight::View* caller,
      ultralight::Cursor cursor);

  // Inherited from ultralight::Listener::Load
  void OnDOMReady(ultralight::View* caller);

  // Inherited from EngineListener
  void onShowEvent(bool show);

  // Called by GUI JavaScript
  void onButtonClick(const JSObject& obj, const JSArgs& args);

private:
  void updateGeometry();

  GUIListener* _listener = nullptr;

  Window& _window;
  ultralight::RefPtr<ultralight::View> _view;
  ultralight::RefPtr<ultralight::Renderer> _renderer;
  ultralight::GPUDriver* _driver;
  ultralight::GPUState _gpu_state;
  ultralight::Cursor _cur_cursor;
  std::unique_ptr<GPUContext> _gpu_context;

  uint32_t _geometry_id;
  std::vector<ultralight::Vertex_2f_4ub_2f_2f_28f> _vertices;
  std::vector<ultralight::IndexType> _indices;

  bool _needs_update = true;

  JSFunction setButtonText;
};
