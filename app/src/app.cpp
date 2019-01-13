#include "app.h"

#include <chrono>
#include <iostream>
#include <thread>

Application::Application(Window& window) : _window(window) {
  _window.set_listener(this);

  _gui.reset(new GUI(_window));
  _engine.reset(new Engine(_window));

  _gui->set_listener(_engine.get());
  _engine->set_listener(_gui.get());
}

Application::~Application() {
  _window.set_listener(nullptr);
}

void Application::run() {
  // White splash screeen
  float none[] = { 1.0f, 1.0f, 1.0f, 0.0f };
  glClearBufferfv(GL_COLOR, 0, none);
  glfwSwapBuffers(_window.handle());

  // Dry run (takes a while)
  _gui->draw();

  float t_init = (float)glfwGetTime();
  while (!glfwWindowShouldClose(_window.handle())) {
    float t = (float)glfwGetTime() - t_init;
    glViewport(0, 0, _window.width(), _window.height());

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float none[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, none);

    _engine->update(t);
    _gui->draw();

    glfwSwapBuffers(_window.handle());
    glfwPollEvents();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Application::onKeyEvent(const ultralight::KeyEvent& evt) {
  _gui->fireKeyEvent(evt);
}

void Application::onMouseEvent(const ultralight::MouseEvent& evt) {
  _gui->fireMouseEvent(evt);
}

void Application::onScrollEvent(const ultralight::ScrollEvent& evt) {
  _gui->fireScrollEvent(evt);
}

void Application::onResize(int width, int height) {
  _gui->resize(width, height);
}
