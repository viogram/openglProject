#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float screenWidth = 800;
const float screenHeight = 600;

Camera camera(glm::vec3(0, 0, 3.0f));
float deltaTime = 0.0f;	// 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
float lastX = screenWidth/2, lastY = screenHeight/2; //初始鼠标位置
bool firstMouse = false;  //是否第一次调用鼠标事件

vector<std::string> faces
{
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "front.jpg",
    "back.jpg"
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) // 这个bool变量初始时是设定为true的
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float deltaX = xpos - lastX;
    float deltaY = lastY - ypos;   //注意屏幕空间左上角为原点，y坐标自顶相下逐渐变大

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(deltaX, deltaY);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

unsigned int loadCubemap(vector<std::string> faces) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    int width, height, nrChannels;
    for (int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //设置版本号为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //stbi_set_flip_vertically_on_load(true);
    //打开深度缓冲
    glEnable(GL_DEPTH_TEST);
    
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    unsigned int cubemap = loadCubemap(faces);

    //skyboxVAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    Shader myShader("..\\shader\\vertex.txt", "..\\shader\\fragment.txt");
    Shader skyShader("..\\shader\\skyVertex.txt", "..\\shader\\skyFragment.txt");
    Model myModel("..\\model\\nanosuit\\nanosuit.obj", cubemap);

    while (!glfwWindowShouldClose(window))
    {
        //输入
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window);

        //渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightPos(1.2f, 1.0f, 1.0f);
        glm::mat4 model=glm::mat4(1.0f);
        
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();

        glm::mat4 project = glm::mat4(1.0f);
        project = glm::perspective(glm::radians(camera.Zoom), screenWidth/screenHeight, 0.1f, 100.0f);

        myShader.use();
        myShader.setFloat("material.shininess", 32);
        myShader.setVec3("light.position", lightPos);
        myShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
        myShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        myShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("light.constant", 1.0f);
        myShader.setFloat("light.linear", 0.09f);
        myShader.setFloat("light.quadratic", 0.032f);
        myShader.setVec3("viewPos", camera.Position);
        myShader.setMatirx4f("model", model);
        myShader.setMatirx4f("view", view);
        myShader.setMatirx4f("project", project);
        myShader.setMatirx3f("toWorldNormal", glm::mat3(glm::transpose(glm::inverse(model))));
        myModel.Draw(myShader);

        glDepthFunc(GL_LEQUAL);
        skyShader.use();
        glBindVertexArray(skyboxVAO);
        skyShader.setMatirx4f("view", glm::mat4(glm::mat3(view)));
        skyShader.setMatirx4f("project", project);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LESS);
        //检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(myShader.ID);
    glfwTerminate();
    return 0;
}