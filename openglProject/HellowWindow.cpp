#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "camera.h"

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
    //打开深度缓冲
    glEnable(GL_DEPTH_TEST);

    Shader myShader("..\\shader\\vertex.txt", "..\\shader\\fragment.txt");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    //绑定VAO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container2.jpg", &width, &height, &nrChannels, 0);

    unsigned int diffuseMap, specularMap, emissionMap;
    glGenTextures(1, &diffuseMap);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glGenTextures(1, &specularMap);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    data = stbi_load("lighting_maps_specular_color.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    //创建光源
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Shader lightShader("..\\shader\\lightVertex.txt", "..\\shader\\lightFragment.txt");
    
    myShader.use();
    myShader.setInt("material.diffuse", 0);
    myShader.setInt("material.specular", 1);
    myShader.setInt("material.emission", 2);

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
    };

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
        
        // directional light
        myShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        myShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        myShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        myShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        myShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        myShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("pointLights[0].constant", 1.0f);
        myShader.setFloat("pointLights[0].linear", 0.09f);
        myShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        myShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        myShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        myShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("pointLights[1].constant", 1.0f);
        myShader.setFloat("pointLights[1].linear", 0.09f);
        myShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        myShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        myShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        myShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("pointLights[2].constant", 1.0f);
        myShader.setFloat("pointLights[2].linear", 0.09f);
        myShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        myShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        myShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        myShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("pointLights[3].constant", 1.0f);
        myShader.setFloat("pointLights[3].linear", 0.09f);
        myShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        myShader.setVec3("spotLight.position", camera.Position);
        myShader.setVec3("spotLight.direction", camera.Front);
        myShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        myShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        myShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("spotLight.constant", 1.0f);
        myShader.setFloat("spotLight.linear", 0.09f);
        myShader.setFloat("spotLight.quadratic", 0.032f);
        myShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        myShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

        myShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        myShader.setFloat("material.shininess", 32);
        myShader.setVec3("viewPos", camera.Position);
        myShader.setMatirx4f("view", view);
        myShader.setMatirx4f("project", project);
        


        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            model=glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            myShader.setMatirx4f("model", model);

            //先在cpu上计算变换法线的矩阵，然后再把矩阵传递到GPU，避免在着色器中进行矩阵计算
            glm::mat4 toWorldNormal = glm::inverse(view * model);
            toWorldNormal = glm::transpose(toWorldNormal);
            myShader.setMatirx3f("toWorldNormal", glm::mat3(toWorldNormal));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2));
        
        lightShader.use();
        lightShader.setMatirx4f("model", model);
        lightShader.setMatirx4f("view", view);
        lightShader.setMatirx4f("project", project);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}