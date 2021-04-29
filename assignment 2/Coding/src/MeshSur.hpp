#ifndef MESHSUR_HPP
#define MESHSUR_HPP

#include "BsplineSurface.hpp"
#include "ControlPoint.hpp"
#include "vertex.hpp"

class MeshSur {
public:
    MeshSur() = default;
    MeshSur(const vector<float> knots_u, const vector<float>& knots_v, float step)
    {
        this->knots_u = knots_u;
        this->knots_v = knots_v;
        this->sampleStep = step;
    }
    ~MeshSur() = default;
    //func
    void initalize(const vector<vector<glm::vec3>>& controlpoint);

    void initalize_nurbs(const vector<vector<glm::vec3>>& controlpoint, const vector<vector<float>>& weight);

   // void change_point(const vector<vector<glm::vec3>>& controlpoint, glm::vec3& new_point);
    //set
    void setSampleStep(float step);

    void change(int x_pos,int  y_pos, glm::vec3 a);
    //getter
    vector<Vertex> getVertices() const
    {
        return vers;
    }

    vector<unsigned int> getFaceIndices() const
    {
        return face_indices;
    }
    vector<unsigned int> getEdgeIndices() const
    {
        return edge_indices;
    }

private:
    vector<vector<glm::vec3>> controlpoint;
    vector<float> knots_u;
    vector<float> knots_v;
    float sampleStep = 0.01;
    BsplineSurface mysrf;
    nurBsplineSurface mysrf2;
    //out
    vector<Vertex> vers;
    vector<unsigned int> face_indices;
    vector<unsigned int> edge_indices;
    unsigned int VBO, EBO;
    unsigned int VAO;
    //you can add anything to the skelton code, but remember to construct it in the constructor function and destuctor function
};

#endif