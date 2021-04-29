#include "SurfaceRender.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../inc/stb_image.h"

#include "header.hpp"


//timing
float delta_time = 0.0f; //
float last_time = 0.0f; //

vector<vector<glm:: vec3  > > points_all;
void get_point(vector<vector< glm::vec3> > &a);

//camerassssssssdsssssssssssssspw
glm::vec3 cameraPos = glm::vec3(2.0f, 2.0f, 50.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback_camera(GLFWwindow* window, double now_x, double now_y);
void mouse_callback_point(GLFWwindow* window, double now_x, double now_y);
void mouse_callback_change(GLFWwindow* window, double now_x, double now_y);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float last_x = 400.0f;
float last_y = 300.0f;
float fov = 45.0f;

int x_pos, y_pos;

//lighting

glm::vec3 lightPos[]{
                  glm::vec3(0.0f, 40.0f, 53.0f),
        glm::vec3(0.0f, 33.0f, 53.0f)
// to be added
};

using namespace std;
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "helloworld", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback_camera);
    glfwSetMouseButtonCallback(window,mouse_button_callback);
   // glfwset
    //glfwSetCursorPosCallback(window, mouse_callback_point);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    ControlPoint m_controlPoint;
    m_controlPoint.readBinary("../resources/controlPoint_0.dat");
    vector<vector<glm::vec3>> controlpoint = m_controlPoint.getControlPoint();


    vector<float> knots_u = {0, 0, 0, 0.2 ,0.5 ,0.7, 1.0, 1.0 ,1.0};
    vector<float> knots_v = {0, 0, 0, 0.3 ,0.6 ,0.7, 1.0 , 1.0 ,1.0};
    vector<vector< float> > weight ={
            {500, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1}
    };
    float sampleStep = 0.01;
    cout << controlpoint.size();
    MeshSur mymeshsur = MeshSur(knots_u, knots_v, sampleStep);

   // controlpoint[1][1].x -= 3;
    get_point(controlpoint);

    // mymeshsur.initalize(controlpoint);
    mymeshsur.initalize_nurbs(points_all, weight);

    Shader object_shader("../src/shader.vs", "../src/shader.fs");
    Shader point_shader("../src/point.vs", "../src/point.fs");

    unsigned int VBO, EBO;
    unsigned int VAO;
    int verticeNum;

/*
    verticeNum = mymeshsur.getEdgeIndices().size() ;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mymeshsur.getVertices().size() * sizeof(Vertex), &mymeshsur.getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mymeshsur.getEdgeIndices().size() * sizeof(unsigned int), &mymeshsur.getEdgeIndices()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

*/
    verticeNum = mymeshsur.getFaceIndices().size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mymeshsur.getVertices().size() * sizeof(Vertex), &mymeshsur.getVertices()[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mymeshsur.getFaceIndices().size() * sizeof(unsigned int), &mymeshsur.getFaceIndices()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glBindVertexArray(0);

    vector<float> points;
    for(int i = 0; i < points_all.size(); i++){
        for(int j = 0; j < points_all[i].size(); j++) {
            points.push_back(points_all[i][j].x);
            points.push_back(points_all[i][j].y);
            points.push_back(points_all[i][j].z);

        }
    }

    unsigned int VAOpoint;

    glGenVertexArrays(1, &VAOpoint);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAOpoint);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float ), (void*)0);
    glBindVertexArray(0);


    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("../resources/checkerBox.png", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    object_shader.use();
int a = 0;
    while (!glfwWindowShouldClose(window)) {
        float current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;
        processInput(window);

        glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


       // mymeshsur.change(x_pos, y_pos,points_all[x_pos][y_pos]);

        MeshSur mymeshsur = MeshSur(knots_u, knots_v, sampleStep);

        // controlpoint[1][1].x -= 3;
        mymeshsur.initalize_nurbs(points_all, weight);
        verticeNum = mymeshsur.getFaceIndices().size();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mymeshsur.getVertices().size() * sizeof(Vertex), &mymeshsur.getVertices()[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mymeshsur.getFaceIndices().size() * sizeof(unsigned int), &mymeshsur.getFaceIndices()[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glBindVertexArray(0);



/*
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mymeshsur.getVertices().size() * sizeof(Vertex), &mymeshsur.getVertices()[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mymeshsur.getFaceIndices().size() * sizeof(unsigned int), &mymeshsur.getFaceIndices()[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glBindVertexArray(0);
*/

        glBindTexture(GL_TEXTURE_2D, texture1);

        object_shader.use();
        object_shader.setVec3("objectColor", 0.80f, 0.8f, 0.91f);
        object_shader.setVec3("lightColor[0]", 1.0f, 0.6f, 0.5f);
       // object_shader.setVec3("lightColor[1]", 0.5f, 0.4f, 1.0f);
         object_shader.setVec3("lightPos[0]", lightPos[0]);
      //object_shader.setVec3("lightPos[1]", lightPos[1]);
        object_shader.setVec3("viewPos", cameraPos);
        // view/projection/world transformations
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        object_shader.setMat4("projection", projection);
        object_shader.setMat4("view", view);
        object_shader.setMat4("model", model);



        glGenBuffers(1, &VBO);
        glBindVertexArray(VAOpoint);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float ), (void*)0);
        glBindVertexArray(0);

        glBindVertexArray(VAO);


        glDrawElements(GL_TRIANGLES ,verticeNum, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAOpoint);

        object_shader.setVec3("objectColor", 0.0f, 0.0f, 0.41f);


        vector<float> points;
        for(int i = 0; i < points_all.size(); i++){
            for(int j = 0; j < points_all[i].size(); j++) {
                points.push_back(points_all[i][j].x);
                points.push_back(points_all[i][j].y);
                points.push_back(points_all[i][j].z);

            }
        }
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_DYNAMIC_DRAW);

        glPointSize(3);
        glDrawArrays(GL_POINTS, 0, points.size()/3 );
       //glDrawPixels(10,10,0,0,&points);

       glfwSwapBuffers(window);
        glfwWaitEventsTimeout(0.7);

        //glfwWaitEventsTimeout(0.7);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        glfwSetCursorPosCallback(window, mouse_callback_point);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        glfwSetCursorPosCallback(window, mouse_callback_camera);

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback_change(GLFWwindow* window, double now_x, double now_y){
    double x_delta, y_delta;
    if (firstMouse){
        last_x = now_x;
        last_y = now_y;
        firstMouse = false;
    }

    x_delta = (now_x - last_x) * 0.25/fov;
    y_delta = (last_y - now_y) * 0.25/fov;

    last_x = now_x;
    last_y = now_y;

    points_all[x_pos][y_pos].x += x_delta;
    points_all[x_pos][y_pos].y += y_delta;

}


void mouse_callback_camera(GLFWwindow* window, double now_x, double now_y)
{
    if (firstMouse){
        last_x = now_x;
        last_y = now_y;
        firstMouse = false;
    }
    yaw += (now_x - last_x) * 0.05;
    pitch += (last_y - now_y) * 0.05;
    last_x = now_x;
    last_y = now_y;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}


void mouse_callback_point(GLFWwindow* window, double now_x, double now_y)
{
    if (firstMouse){
        last_x = now_x;
        last_y = now_y;
        firstMouse = false;
    }
    float xoffset = now_x - last_x;
    float yoffset = last_y - now_y;
    last_x = now_x;
    last_y = now_y;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float) yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_RELEASE && button ==  GLFW_MOUSE_BUTTON_LEFT)
    {
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        cout <<" botton release";
        glm::vec2 dd;
//        glm::unProject( ,model,projection,view)
        glfwSetCursorPosCallback(window, mouse_callback_point);
    }
    if (action == GLFW_PRESS&& button ==  GLFW_MOUSE_BUTTON_LEFT)
        {
            double x, y ,xx, yy, ww, min, min_x = 0,  min_y = 0;
            glfwGetCursorPos(window, &x, &y);
            //  cout << x <<" "<< y<<endl;
            //  glfwmouse
            //  glfwGetMonitorPos()
            glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

            glm::vec4 aa;
            for( int i = 0; i < points_all.size(); i++){
                for(int j = 0; j < points_all[i].size(); j++) {
                    aa =  projection * view * model  * glm::vec4(points_all[i][j], 1.0f);
                    xx = ( projection * view * model  * glm::vec4(points_all[i][j], 1.0f)).x;
                    yy = ( projection * view * model  * glm::vec4(points_all[i][j], 1.0f)).y;
                    ww = ( projection * view * model  * glm::vec4(points_all[i][j], 1.0f)).w;
                    xx = xx / ww;
                    yy = yy / ww;
                    xx++;
                    xx *= 400;
                    yy--;
                    yy *= -300;
                 //     cout << xx <<" "<< yy <<" "<<x<< " "<<y <<endl;
                 //   cout << pow((xx - x), 2) + pow((yy - y), 2)<< " "<< i <<"  "<<j<<endl;
                    if(i == 0 && j == 0)
                        min = pow((xx - x), 2) + pow((yy - y), 2);
                    if(pow((xx - x), 2) + pow((yy - y), 2) < min ) {
                        min = pow((xx - x), 2) + pow((yy - y), 2);
                        min_x = i;
                        min_y = j;
                    }
                }
            }
            if(min < 25) {
                x_pos = min_x;
                y_pos = min_y;
              //  cout << min << " " << min_x << "  " << min_y << endl;
                cout << " catch a point"<<endl;
                glfwSetCursorPosCallback(window, mouse_callback_change);

                //glm::unProject();
            }
        }

    if (action == GLFW_PRESS&& button ==  GLFW_MOUSE_BUTTON_RIGHT)
    {
        cout << "Dssdsdd          " << endl ;
    }
    return;
}

void get_point(vector<vector<glm::vec3>>& a){
     points_all = a ;
}