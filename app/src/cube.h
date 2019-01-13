#pragma once
#include <array>
#include <vector>

typedef std::array<float, 3> float3;

void createCube(std::vector<float>& vertices, std::vector<float>& normals) {
  float3 vertices_data[8] = {
    {-1, -1, -1},
    {1, -1, -1},
    {1, 1, -1},
    {-1, 1, -1},
    {-1, -1, 1},
    {1, -1, 1},
    {1, 1, 1},
    {-1, 1, 1}
  };

  float3 normals_data[6] = {
    {0, 0, 1},
    {1, 0, 0},
    {0, 0, -1},
    {-1, 0, 0},
    {0, 1, 0},
    {0, -1, 0}
  };

  int indices[6 * 6] = {
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
  };

  vertices.resize(18 * 6);
  for (int i = 0; i < 36; i++) {
    vertices[i * 3 + 0] = vertices_data[indices[i]][0];
    vertices[i * 3 + 1] = vertices_data[indices[i]][1];
    vertices[i * 3 + 2] = vertices_data[indices[i]][2];
  }

  normals.resize(18 * 6);
  for (int i = 0; i < 36; i++) {
    normals[i * 3 + 0] = normals_data[indices[i / 6]][0];
    normals[i * 3 + 1] = normals_data[indices[i / 6]][1];
    normals[i * 3 + 2] = normals_data[indices[i / 6]][2];
  }
}
