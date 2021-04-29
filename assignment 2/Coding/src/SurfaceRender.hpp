#ifndef SURFACERENDER_HPP
#define SURFACERENDER_HPP
#include "../inc/shader_m.h"
#include "MeshSur.hpp"


class SurfaceRender {
public:

    SurfaceRender() = default;
    SurfaceRender(const MeshSur&);
    ~SurfaceRender() = default;
    void loadSurface(const MeshSur& mesh);
    void loadEdgeSurface(const MeshSur& mesh);
    void draw(const Shader& shader);
    void drawEdge(const Shader& shader);
    void change();

private:

    int faceNum, edgeNum;
    //you can add anything to the skelton code, but remember to construct it in the constructor function and destuctor function
};

#endif
