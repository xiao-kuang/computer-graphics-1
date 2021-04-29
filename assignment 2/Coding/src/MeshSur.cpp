#include "MeshSur.hpp"

void MeshSur::initalize(const vector<vector<glm::vec3>>& controlpoint)
{
    mysrf = BsplineSurface(controlpoint, knots_u, knots_v);
    mysrf.getBuffer_object(vers, face_indices, edge_indices, sampleStep);
}

void MeshSur::initalize_nurbs(const vector<vector<glm::vec3>>& controlpoint, const vector<vector<float>>& weight){
    vector<vector<glm::vec4> > qq(controlpoint.size(), vector<glm::vec4>(controlpoint[0].size()) );
    for(int i = 0; i < controlpoint.size(); i++){
        for(int j = 0; j < controlpoint[0].size(); j++)
            qq[i][j] = glm::vec4(controlpoint[i][j], weight[i][j]);

    }
    mysrf2 = nurBsplineSurface(qq, knots_u, knots_v);
    mysrf2.getBuffer_object(vers, face_indices, edge_indices, sampleStep);
}

void change_point(const vector<vector<glm::vec3>>& controlpoint, glm::vec3& new_point){

}


void MeshSur::setSampleStep(float step)
{
    this->sampleStep = step;
}

void MeshSur::change(int x_pos, int  y_pos, glm::vec3 a) {
    this->mysrf2.change(x_pos,  y_pos, a);
}
