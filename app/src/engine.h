#pragma once
#include "linmath.h"
#include "listener.h"
#include "window.h"

class Engine : public GUIListener {
public:
  Engine(Window& window);
  ~Engine() {}

  void set_listener(EngineListener* listener) { _listener = listener; }
  EngineListener* listener() { return _listener; }

  void update(float t);

  // Inherited from GUIListener
  void onButtonClick();

private:
  void setMatrices();

  EngineListener* _listener = nullptr;

  Window& _window;

  GLuint _program;
  GLuint _vao;
  mat4x4 _mvp;
  float _t = 0;

  bool _show_cube = false;
  bool _show_cube_target = true;
  float _anim_time = 0;
};
