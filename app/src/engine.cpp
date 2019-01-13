#include "engine.h"
#include "cube.h"
#include "gl/gl.h"

#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static string vert_shader = R"(
#version 330

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 norm;

uniform mat4 MVP;

out vec4 v_color;

vec3 light_dir = vec3(0.25, 0.94, -0.45);
vec3 base_color = vec3(0.180, 0.8, 0.443);

void main() {
  float i = 1.0 + 0.5 * dot(normalize(norm), normalize(light_dir));

  v_color = vec4(i * base_color, 1.0);
  gl_Position = MVP * vec4(vert, 1.0);
}
)";

static string frag_shader = R"(
#version 330

out vec4 frag_color;

in vec4 v_color;

void main() {
  frag_color = v_color;
}
)";

float smoothstep(float x) {
  if (x <= 0)
    return 0;
  if (x > 1)
    return 1;
  float x3 = x * x * x;
  return 6 * x * x * x3 - 15 * x * x3 + 10 * x3;
}

Engine::Engine(Window& window) : _window(window) {
  _program = createShaderProgram(vert_shader, frag_shader);

  vector<float> vertices, normals;
  createCube(vertices, normals);

  _vao = createVAO(vertices, normals);
}

void Engine::update(float t) {
  _t = t;

  setMatrices();

  glUseProgram(_program);
  GLint mvp_location = glGetUniformLocation(_program, "MVP");
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)_mvp);

  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
}

void Engine::onButtonClick() {
  _show_cube_target = !_show_cube_target;
  _anim_time = _t;
}

void Engine::setMatrices() {
  mat4x4 m, v, p;

  // Model
  mat4x4_identity(m);
  float k = _show_cube ? 1 : 0;
  if (_anim_time != -1) {
    k = smoothstep(1.3 * (_t - _anim_time));
    k = _show_cube_target ? k : 1 - k;

    if (k == 1 || k == 0) {
      _anim_time = -1;
      _show_cube = _show_cube_target;
      _listener->onShowEvent(_show_cube);
    }
  }
  mat4x4_scale_aniso(m, m, k, k, k);

  // View
  vec3 eye = { 3, 2.5, 3 };
  vec3 center = { 0, -0.5, 0 };
  vec3 up = { 0, 1, 0 };
  mat4x4_look_at(v, eye, center, up);
  mat4x4_rotate_Y(v, v, -_t * 0.7);

  // Perspective
  float ratio = _window.width() / (float)_window.height();
  mat4x4_perspective(p, 45.0f, ratio, 0.1f, 10.f);

  // MVP
  mat4x4_mul(v, v, m);
  mat4x4_mul(_mvp, p, v);
}
