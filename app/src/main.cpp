#include "app.h"
#include "window.h"

int main(int argc, char** argv) {
  WindowOptions opt;
  opt.height = 520;
  opt.width = 720;

  Window window(opt);
  Application app(window);

  app.run();

  return 0;
}
