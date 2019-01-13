#include "engine.h"
#include "gui.h"
#include "window.h"

class Application : public WindowListener {
public:
  Application(Window& window);
  ~Application();

  void run();

  // Inherited from WindowListener
  void onKeyEvent(const ultralight::KeyEvent& evt);
  void onMouseEvent(const ultralight::MouseEvent& evt);
  void onScrollEvent(const ultralight::ScrollEvent& evt);
  void onResize(int width, int height);

protected:
  Window& _window;

  std::unique_ptr<GUI> _gui;
  std::unique_ptr<Engine> _engine;
};
