#include "SurfaceRender.hpp"
#include "header.hpp"


using namespace std;

//timing
float delta_time = 0.0f; //
float last_time = 0.0f; //

//camera
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double now_x, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float last_x = 400.0f;
float last_y = 300.0f;
float fov = 45.0f;

//lighting

glm::vec3 lightPos[]{
    glm::vec3(0.2f, 2.0f, 3.0f),
    glm::vec3(0.2f, -4.0f, -3.0f)
// to be added
};
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
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    glEnable(GL_MULTISAMPLE);
    //above are what I will not need to use
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
    };
    string s;
    int pumpkin_vertices_num, bunny_vertices_num, pumpkin_surface_num, bunny_surface_num;
    ifstream fin1("C:\\Users\\Administrator\\Desktop\\Assiment1_yjw_2019533095\\Coding\\resources\\pumpkin_simlified.object");
    fin1 >> pumpkin_vertices_num >> pumpkin_surface_num;
    vector <Vertex> vertices;
    vector <unsigned int> indices;
    float x, y, z;
    for (int i = 0; i < pumpkin_vertices_num ; i++)
    {
        fin1 >> x >> y >> z;
        glm::vec3 a = glm::vec3(x, y, z);
        glm::vec3 b = glm::vec3(x, y, z);
        Vertex t = { a, b };
        vertices.push_back(t);
    }
    for (int i = 0; i < pumpkin_vertices_num ; i++)
    {
        fin1 >> x >> y >> z;
        vertices[i].Normal = glm::vec3(x, y, z);
    }
    for (int i = 0; i < pumpkin_surface_num * 3; i++)
    {
        unsigned int a;
        fin1 >> a;
        indices.push_back(a - 1);
    }

    ifstream fin2("C:\\Users\\Administrator\\Desktop\\Assiment1_yjw_2019533095\\Coding\\resources\\bunny_simlified.object");
    fin2 >> bunny_vertices_num >> bunny_surface_num;
    for (int i = pumpkin_vertices_num; i <   pumpkin_vertices_num + bunny_vertices_num; i++)
    {
        fin2 >> x >> y >> z;
        glm::vec3 a = glm::vec3(x, y+2, z);
        glm::vec3 b = glm::vec3(x, y, z);
        Vertex t = { a, b };
        vertices.push_back(t);
    }
    for (int i =  pumpkin_vertices_num; i < pumpkin_vertices_num + bunny_vertices_num; i++)
    {
        fin2 >> x >> y >> z;
        vertices[i].Normal = glm::vec3(x, y, z);
    }
    for (int i = 0; i < bunny_surface_num * 3; i++)
    {
        unsigned int a;
        fin2 >> a;
        indices.push_back(a - 1 + pumpkin_vertices_num);
    }

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float )));
    glBindVertexArray(0);



    Shader object_shader("..\\src\\shader.vs", "..\\src\\shader.fs");

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightPos[0].x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos[0].y = sin(glfwGetTime() / 2.0f) * 1.0f;

        lightPos[1].x = 1.0f + cos(glfwGetTime()) * 2.0f;
        lightPos[1].z = cos(glfwGetTime() / 2.0f) * 1.0f;
        // be sure to activate shader when setting uniforms/drawing objects
        object_shader.use();

        object_shader.setVec3("objectColor", 0.80f, 0.8f, 0.91f);
        object_shader.setVec3("lightColor[0]", 1.0f, 0.6f, 0.5f);
        object_shader.setVec3("lightColor[1]", 0.5f, 0.4f, 1.0f);
        object_shader.setVec3("lightPos[0]", lightPos[0]);
        object_shader.setVec3("lightPos[1]", lightPos[1]);
        object_shader.setVec3("viewPos", cameraPos);
        // view/projection/world transformations
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        object_shader.setMat4("projection", projection);
        object_shader.setMat4("view", view);
        object_shader.setMat4("model", model);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
        glfwSwapBuffers(window);
        glfwPollEvents();
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double now_x, double now_y)
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}