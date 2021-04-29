#include "SurfaceRender.hpp"
/*
SurfaceRender::SurfaceRender(const MeshSur& surface)
{
    loadSurface(surface);
}



void SurfaceRender::loadEdgeSurface(const MeshSur& mesh)
{
    verticeNum = mesh.getEdgeIndices().size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(Vertex), &mesh.getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getEdgeIndices().size() * sizeof(unsigned int), &mesh.getEdgeIndices()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    //Load mesh surface bind VAO
    //finish your code here.
}
void SurfaceRender::loadSurface(const MeshSur& mesh)
{

    verticeNum = mesh.getFaceIndices().size();


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(Vertex), &mesh.getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getFaceIndices().size() * sizeof(unsigned int), &mesh.getFaceIndices()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float )));
    //Load mesh surface bind VAO
    //finish your code here.
}

void SurfaceRender::draw(const Shader& shader)
{
    //shader settings.


  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // cout << faceNum<<endl;
    glDrawElements(GL_TRIANGLES, verticeNum
                   , GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SurfaceRender::drawEdge(const Shader& shader)
{

    //shader settings.
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, verticeNum, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
*/