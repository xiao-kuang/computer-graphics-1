#ifndef VERTEX_HPP
#define VERTEX_HPP
#include "header.hpp"
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texture
    glm::vec2  TexCoords;
};
#endif