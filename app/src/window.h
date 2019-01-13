#pragma once
#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <Ultralight/KeyEvent.h>
#include <Ultralight/Listener.h>
#include <Ultralight/MouseEvent.h>
#include <Ultralight/ScrollEvent.h>
#include <memory>

struct WindowOptions {
  int width = 1024;
  int height = 720;
  bool enable_vsync = true;
};

class WindowListener {
public:
  virtual ~WindowListener() {}

  virtual void onKeyEvent(const ultralight::KeyEvent& evt) = 0;
  virtual void onMouseEvent(const ultralight::MouseEvent& evt) = 0;
  virtual void onScrollEvent(const ultralight::ScrollEvent& evt) = 0;
  virtual void onResize(int width, int height) = 0;
};

class Window {
public:
  Window(const WindowOptions& opt);

  ~Window();
  void set_listener(WindowListener* listener) { _listener = listener; }

  WindowListener* listener() { return _listener; }
  GLFWwindow* handle() const { return _window; }

  int width() const;
  int height() const;
  double scale() const;
  bool is_fullscreen() const;

  int deviceToPixels(int val) const;
  int pixelsToDevice(int val) const;
  void setTitle(const char* title);
  void setCursor(ultralight::Cursor cursor);

  void close();

private:
  GLFWwindow* _window;
  WindowListener* _listener = nullptr;
  GLFWcursor* _cursor_ibeam;
  GLFWcursor* _cursor_crosshair;
  GLFWcursor* _cursor_hand;
  GLFWcursor* _cursor_hresize;
  GLFWcursor* _cursor_vresize;
};
