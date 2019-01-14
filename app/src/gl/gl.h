#pragma once
#include "glinclude.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void glfwErrorCallback(int error, const char* description) {
  cerr << "GLFW error " << error << ": " << description << endl;
}

inline string getShaderLog(GLuint shader_id) {
  GLint length, result;
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
  string str(length, ' ');
  glGetShaderInfoLog(shader_id, (GLsizei)str.length(), &result, &str[0]);
  return str;
}

GLuint createShaderProgram(string v, string f) {
  // Compile vertex shader
  GLint compileStatus;
  const char* v_str = v.c_str();
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &v_str, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus == GL_FALSE) {
    cerr << "Unable to compile shader.\n\tError:"
         << glGetError() << "\n\tLog: " << getShaderLog(vertex_shader) << endl;
    exit(1);
  }

  // Compile fragment shader
  const char* f_str = f.c_str();
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &f_str, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus == GL_FALSE) {
    cerr << "Unable to compile shader.\n\tError:"
         << glGetError() << "\n\tLog: " << getShaderLog(fragment_shader) << endl;
    exit(1);
  }

  // Create shader program
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  return program;
}

GLuint createVAO(vector<float> vertices, vector<float> normals) {
  GLuint vao;

  // Create the vertex array object (list of buffers)
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vboHandles[2];
  glGenBuffers(2, vboHandles);

  glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
  glBufferData(GL_ARRAY_BUFFER,
      vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
  glBufferData(GL_ARRAY_BUFFER,
      normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindVertexArray(0);

  return vao;
}
