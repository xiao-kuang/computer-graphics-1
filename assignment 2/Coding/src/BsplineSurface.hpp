#ifndef BSPLINESURFACE_HPP
#define BSPLINESURFACE_HPP
#include "vertex.hpp"
using namespace std;
class BsplineSurface {
public:
    BsplineSurface() = default;
    BsplineSurface(const vector<vector<glm::vec3>>& cnPoint, const vector<float>& knots_u, vector<float>& knots_v)
    {
        this->m_cnPoint = cnPoint;
        this->m_knots_u = knots_u;
        this->m_knots_v = knots_v;

    }
    BsplineSurface(const BsplineSurface& sur)
    {
        this->m_cnPoint = sur.m_cnPoint;
        this->m_knots_u = sur.m_knots_u;
        this->m_knots_v = sur.m_knots_v;

    }
    BsplineSurface& operator=(const BsplineSurface& sur)
    {
        this->m_cnPoint = sur.m_cnPoint;
        this->m_knots_u = sur.m_knots_u;
        this->m_knots_v = sur.m_knots_v;

        return *this;
    }
    ~BsplineSurface() = default;

    //func
    inline glm::vec3 calculateNormal(const glm::vec3& vector1, const glm::vec3& vector2);
    glm::vec3 calulatePos(const float& u, const float& v);
    glm::vec3 DeBoorCurve(const vector<glm::vec3>& controlpoint, const vector<float>& knots, const float& t);
    void sampleVertices(float step, vector<glm::vec3>& vertices);
    void getBuffer_object(vector<Vertex>& vers, vector<unsigned int>& face_indices, vector<unsigned int>& edge_indices, float step = 0.02f);

private:
    vector<vector<glm::vec3> > m_cnPoint;
    vector<float> m_knots_u;
    vector<float> m_knots_v;
    //you can add anything to the skelton code, but remember to construct it in the constructor function and destuctor function
};

class nurBsplineSurface {
public:
    nurBsplineSurface() = default;
    nurBsplineSurface(const vector<vector<glm::vec4>>& cnPoint, const vector<float>& knots_u, vector<float>& knots_v)
    {
        this->m_cnPoint = cnPoint;
        this->m_knots_u = knots_u;
        this->m_knots_v = knots_v;
    }
    nurBsplineSurface(const nurBsplineSurface& sur)
    {
        this->m_cnPoint = sur.m_cnPoint;
        this->m_knots_u = sur.m_knots_u;
        this->m_knots_v = sur.m_knots_v;
    }
    nurBsplineSurface& operator=(const nurBsplineSurface& sur)
    {
        this->m_cnPoint = sur.m_cnPoint;
        this->m_knots_u = sur.m_knots_u;
        this->m_knots_v = sur.m_knots_v;
        return *this;
    }
    ~nurBsplineSurface() = default;

    //func
    inline glm::vec3 calculateNormal(const glm::vec3& vector1, const glm::vec3& vector2);
    glm::vec3 calulatePos(const float& u, const float& v);
    glm::vec4 DeBoorCurve(const vector<glm::vec4>& controlpoint, const vector<float>& knots, const float& t );
    void sampleVertices(float step, vector<glm::vec3>& vertices);
    void getBuffer_object(vector<Vertex>& vers, vector<unsigned int>& face_indices, vector<unsigned int>& edge_indices, float step = 0.02f);
    void change(int x_pos, int  y_pos, glm::vec3 a);

private:
    vector<vector<glm::vec4> > m_cnPoint;
    vector<float> m_knots_u;
    vector<float> m_knots_v;
    //you can add anything to the skelton code, but remember to construct it in the constructor function and destuctor function
};


#endif