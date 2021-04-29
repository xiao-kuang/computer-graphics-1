#include "BsplineSurface.hpp"

/**
 * Evaluate Normal of two vector
 * @param[in] vector1
 * @param[in] vector2
 * @return glm::vec3 is the normal of these two vector.
 */
inline glm::vec3 BsplineSurface::calculateNormal(const glm::vec3& vector1, const glm::vec3& vector2)
{
    //finish code in calculateNormal
    return glm::normalize(glm::cross(vector1, vector2));
}

/**
 * Evaluate point on a Bspline surface
 * @param[in] u 
 * @param[in] v
 * @return glm::vec3 point on the bspline surface  at parameter (u,v).
 */
glm::vec3 BsplineSurface::calulatePos(const float& u, const float& v)
{
    vector<glm::vec3> temp(this->m_cnPoint.size());
    glm::vec3 final;
    for(int j = 0; j < this->m_cnPoint.size(); j++){
        for(int i = 0; i < this->m_cnPoint[j].size(); i++){
            temp[i] += DeBoorCurve(this->m_cnPoint[i],this->m_knots_v, v);
        }
        final += DeBoorCurve(temp, this->m_knots_u, u);
    }
   // cout << final.x<< " "<<final.y<< " "<<final.z<< " "<<endl;
    return final;
}//if true

/**
 * Evaluate point on a Bspline curve
 * @param[in] control_point Control points of the curve.
 * @param[in] knots Knot vector of the curve.
 * @param[in] t Parameter to evaluate the curve at.
 * @return glm::vec3 point on the curve at parameter t.
 */
glm::vec3 BsplineSurface::DeBoorCurve(const vector<glm::vec3>& controlpoint, const vector<float>& knots, const float& t)
{

    int k; // [u_k,u_k+1]
    int n = controlpoint.size() - 1;
    int m = knots.size() - 1;
    int p = m - n - 1;
    int s = 0;
    int h = p;
    float aJi;
    //get the intend index
    for(int l = 0; l < knots.size(); l++) {
        if (knots[l] <= t && t < knots[l + 1])
        {
            k = l;
            if (knots[l] == t) {
                s++;
                h--;
            }
        }
    }
    glm::vec3 d[n + 1][controlpoint.size()];
    for(int i = 0; i <= n; i++)
        d[i][0] = controlpoint[i];
    for(int r = 1; r <= h ; r++){
        for(int i = k - p + r; i <= k - s; i++){
            aJi = (t - knots[i])/(knots[i + p - r + 1] - knots[i]);
            d[i][r] = (1 - aJi) * d[i - 1][r - 1] + aJi * d[i][r - 1];
        }
    }
    return d[k - s][p - s];

}

/**
 * Sample point on a given step Bspline surface
 * @param[in] step Step size for you sampling on the surface.
 * @param[out] vertices the point position that for your sampling resolution.
 */
void BsplineSurface::sampleVertices(float step, vector<glm::vec3>& vertices)
{

    for(float i = 0; i <= 1; i += step){
        for(float j = 0; j <= 1; j += step){
            //cout<<i<<' '<< j<<' '<< i * step<<' '<< j*step<<endl;
            glm::vec3 t = calulatePos(i , j);
            vertices.push_back(t);
        }
    }

    //finish your code here.
    return;
}

/**
 * GenerateBuffer object for opengl to render
 * float step  Step size for you sampling on the surface. 
 * @param[out] vers including position and normal
 * @param[out] face indices for render triangles
 * @param[out] edge_indices for render lines
 */
void BsplineSurface::getBuffer_object(vector<Vertex>& vers, vector<unsigned int>& face_indices,
    vector<unsigned int>& edge_indices, float step)
{

    vector<glm::vec3> vertices, normal;
    glm::vec3 face_normal, x, y;
    vector<int> num;
    int t = int (1 / step);

    int a, b, c, d;//four point of a single polygon


    sampleVertices(step, vertices);
    cout << t<<endl;

    for(int i =0; i <  pow(t, 2); i++){
        //add index of triangle and line

        a = i +  i / (t);                   //    b-----d
        b = a + 1;                          //    | \   |
        c = a + t + 1;                      //    |   \ |
        d = c + 1;                          //    a-----c
     //   cout << a<<" "<< b<<" "<< c<<" "<<endl;
      // cout << " "<< b<<" "<< c<<" "<<d<<endl;
        face_indices.push_back(a);
        face_indices.push_back(b);
        face_indices.push_back(c);
        face_indices.push_back(b);
        face_indices.push_back(c);
        face_indices.push_back(d );
        if(i % t == 0) {/**???*/
            //top line
            edge_indices.push_back(a );
            edge_indices.push_back(c);
        }
        if(i < t) {
            //top line
            edge_indices.push_back(a );
            edge_indices.push_back(b );
        }
        edge_indices.push_back(d );
        edge_indices.push_back(c);
        edge_indices.push_back(d);
        edge_indices.push_back(b );

        x = vertices[c] - vertices[a];
        y = vertices[b] - vertices[a];
        face_normal = calculateNormal(x, y);
        normal.push_back(face_normal);

        x = vertices[b] - vertices[d];
        y = vertices[c] - vertices[d];
        face_normal = calculateNormal(x ,y);
        normal.push_back(face_normal);
    }
        glm::vec3 normal_temp;
        for(int i = 0; i < vertices.size(); i++){
           // cout<<vertices.size() - 1 - t <<" "<< i << " "<< vertices.size() - 1<<"  "<<t <<endl;
            glm::vec3 temp;

             /*   temp = normal[(i / (t + 1 )) * 2 * t  + 2 * (i % (t + 1))] ;
                temp += normal[(i / (t + 1 )) * 2 * t  + 2 * (i % (t + 1)) + 2] ;
                temp += normal[(i / (t + 1 )) * 2 * t  + 2 * (i % (t + 1)) + 1] ;
               /* temp +=  normal[(i / (t + 1 )) * 2 * t  + 2 * (i % (t + 1)) - 2 * t  + 3] ;
                temp +=  normal[(i / (t + 1 )) * 2 * t  + 2 * (i % (t + 1)) - 2 * t  + 1];
                temp +=  normal[(i / (t + 1 )) * 2 * t  + 2 * (i % (t + 1)) - 2 * t + 2];*/
           temp = normal[ i * 2 - 2 * i / t];
         //  cout << (i / t) * (t - 1)* 2 + 2 * (i % t)<< "  "<<i<< endl;
            normal_temp = glm::normalize(temp);
            glm::vec2 texture;
            float qq = i % ( t + 1 );
            texture.x =qq / (t + 1);
            float mm = i / (t + 1);
            texture.y = mm / (t + 1);


          //  cout << texture.x << " "<<texture.y <<" "<< endl;
            Vertex u= {vertices[i], normal_temp, texture};
            // << vertices[i].x<<' '<<vertices[i].y<<' '<<vertices[i].z<<"   "<<endl;
            vers.push_back(u);
        }
        // creat new struct


    //finish your code here.
    return;
}


inline glm::vec3 nurBsplineSurface::calculateNormal(const glm::vec3& vector1, const glm::vec3& vector2)
{
    //finish code in calculateNormal
    return glm::normalize(glm::cross(vector1, vector2));
}

/**
 * Evaluate point on a Bspline surface
 * @param[in] u
 * @param[in] v
 * @return glm::vec3 point on the bspline surface  at parameter (u,v).
 */
glm::vec3 nurBsplineSurface::calulatePos(const float& u, const float& v)
{
    vector<glm::vec4> temp(this->m_cnPoint.size());
    glm::vec4 final;
    glm::vec3 last;
    for(int j = 0; j < this->m_cnPoint.size(); j++){
        for(int i = 0; i < this->m_cnPoint[j].size(); i++){
            temp[i] += DeBoorCurve(this->m_cnPoint[i],this->m_knots_v, v);
        }
        final += DeBoorCurve(temp, this->m_knots_u, u);
    }
    last.x = final.x ;
    last.y = final.y ;
    last.z = final.z ;

    // cout << final.x<< " "<<final.y<< " "<<final.z<< " "<<endl;
    return last;
}//if true

/**
 * Evaluate point on a Bspline curve
 * @param[in] control_point Control points of the curve.
 * @param[in] knots Knot vector of the curve.
 * @param[in] t Parameter to evaluate the curve at.
 * @return glm::vec3 point on the curve at parameter t.
 */
glm::vec4 nurBsplineSurface::DeBoorCurve(const vector<glm::vec4>& controlpoint, const vector<float>& knots, const float& t)
{
    int k; // [u_k,u_k+1]
    int n = controlpoint.size() - 1;
    int m = knots.size() - 1;
    int p = m - n - 1;
    int s = 0;
    int h = p;
    float aJi;
    //get the intend index
    for(int l = 0; l < knots.size(); l++) {
        if (knots[l] <= t && t < knots[l + 1])
        {
            k = l;
            if (knots[l] == t) {
                s++;
                h--;
            }
        }
    }
    glm::vec4 d[n + 1][controlpoint.size()];
    for(int i = 0; i <= n; i++) {
        d[i][0] = controlpoint[i];
        d[i][0].x = controlpoint[i].x * controlpoint[i] .w;
        d[i][0].y = controlpoint[i].y * controlpoint[i] .w;
        d[i][0].z = controlpoint[i].z * controlpoint[i] .w;
    }
    for(int r = 1; r <= h ; r++){
        for(int i = k - p + r; i <= k - s; i++){
            aJi = (t - knots[i])/(knots[i + p - r + 1] - knots[i]);
            d[i][r] = (1 - aJi) * d[i - 1][r - 1] + aJi * d[i][r - 1];

        }
    }
    d[k - s][p - s].x = d[k - s][p - s].x / d[k - s][p - s].w;
    d[k - s][p - s].y = d[k - s][p - s].y / d[k - s][p - s].w;
    d[k - s][p - s].z = d[k - s][p - s].z / d[k - s][p - s].w;
    return d[k - s][p - s];
}

/**
 * Sample point on a given step Bspline surface
 * @param[in] step Step size for you sampling on the surface.
 * @param[out] vertices the point position that for your sampling resolution.
 */
void nurBsplineSurface::sampleVertices(float step, vector<glm::vec3>& vertices)
{

    for(float i = 0; i <= 1; i += step){
        for(float j = 0; j <= 1; j += step){
           // cout<<i<<' '<< j<<' '<< i * step<<' '<< j*step<<endl;
            glm::vec3 t = calulatePos(i , j);
            vertices.push_back(t);
        }
    }

    //finish your code here.
    return;



}

/**
 * GenerateBuffer object for opengl to render
 * float step  Step size for you sampling on the surface.
 * @param[out] vers including position and normal
 * @param[out] face indices for render triangles
 * @param[out] edge_indices for render lines
 */
void nurBsplineSurface::getBuffer_object(vector<Vertex>& vers, vector<unsigned int>& face_indices,
                                      vector<unsigned int>& edge_indices, float step)
{

    vector<glm::vec3> vertices, normal;
    glm::vec3 face_normal, x, y;
    vector<int> num;
    int t = int (1 / step);
    int a, b, c, d;//four point of a single polygon


    sampleVertices(step, vertices);
    //cout << t<<endl;

    for(int i =0; i <  pow(t, 2); i++){
        a = i +  i / (t );           //    b-----d
        b = a + 1;                      //    | \   |
        c = a + t +1;                      //    |   \ |
        d = c + 1;                      //    a-----c
        //   cout << a<<" "<< b<<" "<< c<<" "<<endl;
        // cout << " "<< b<<" "<< c<<" "<<d<<endl;

        face_indices.push_back(a);
        face_indices.push_back(b);
        face_indices.push_back(c);
        face_indices.push_back(b);
        face_indices.push_back(c);
        face_indices.push_back(d );
        if(i % t == 0) {/**???*/
            //top line
            edge_indices.push_back(a );
            edge_indices.push_back(c);
        }
        if(i < t) {
            //top line
            edge_indices.push_back(a );
            edge_indices.push_back(b );
        }
        edge_indices.push_back(d );
        edge_indices.push_back(c);
        edge_indices.push_back(d);
        edge_indices.push_back(b );

        x = vertices[c] - vertices[a];
        y = vertices[b] - vertices[a];
        face_normal = calculateNormal(x, y);
        normal.push_back(face_normal);

        x = vertices[b] - vertices[d];
        y = vertices[c] - vertices[d];
        face_normal = calculateNormal(x ,y);
        normal.push_back(face_normal);
    }


    //calculate vertex normal
    glm::vec3 normal_temp;

    for(int i = 0; i < vertices.size(); i++){
        glm::vec3 temp;


        if( i == 0){
            temp = normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2] ;
        }// left down{
        else if(i  == t + 1){
            temp = normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 1] ;
            temp += normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2] ;
        }  //leftUp
        else if(i == vertices.size() -1){
            temp = normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t  -1] ;
        } //rightUP
        else if(i == vertices.size() - 2 - t){
            temp = normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t- 1] ;
            temp += normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t] ;
        }//right down
        else if(0 < i && i < t + 1){
            temp = normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2] ;
            temp += normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2] ;
            temp += normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 1] ;
        }//left
        else if(vertices.size() - 2 - t < i && i < vertices.size() -1){
            temp =  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t  -1] ;
            temp +=  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t  + 1];
            temp +=  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t ];
        }//right
        else if((i + 1) % (t + 1) == 0){
            temp = normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2] ;
            temp += normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 1] ;
            temp +=  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t  -1] ;
        }//up
        else if(i % (t + 1) == 0){
            temp = normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2] ;
            temp +=  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t  + 1];
            temp +=  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t ];

        }//down
        else{
            temp = normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2] ;
            temp += normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2] ;
            temp += normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 1] ;
            temp +=  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t  -1] ;
            temp +=  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t  + 1];
            temp +=  normal[i / ( t + 1 ) * 2 * t  + i % ( t + 1) * 2 - 2 * t ];
        }//other

       //cout<< temp.x<<temp.y<< endl;

        //  cout << (i / t) * (t - 1)* 2 + 2 * (i % t)<< "  "<<i<< endl;
        normal_temp = glm::normalize(temp);
        glm::vec2 texture;

               float qq = i % (t + 1);
               texture.y = qq / (t + 1) ;
               float mm = i / (t + 1);
               texture.x = mm / (t + 1) ;
              // cout << texture.x << " "<<texture.y <<" "<< endl;

/*
      float left  = vertices[i % (t + 1)].x;
        float right = vertices[vertices.size() - ((t + 1) - i % (t + 1))].x;
        float up    = vertices[(t + 1) * (i / (t + 1) + 1)-1].y;
        float down  = vertices[(t + 1) * (i / (t + 1))].y;
        texture.x = (vertices[i].x - left) / (right - left);
        texture.y = (vertices[i].y - down) / ( up - down);
        */

       //  cout << texture.x << " "<<texture.y <<" "<< endl;
        Vertex u= {vertices[i], normal_temp, texture};
        // << vertices[i].x<<' '<<vertices[i].y<<' '<<vertices[i].z<<"   "<<endl;
        vers.push_back(u);
    }
    // creat new struct


    //finish your code here.
    return;
}

void nurBsplineSurface::change(int x_pos, int  y_pos, glm::vec3 a) {
    cout<<"changed!!!";
    cout<< x_pos<<" "<< y_pos<<" "<<endl;



}

