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

Camera camera(glm::vec3(0, 0, 55.0f));
float deltaTime = 0.0f;	// ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��
float lastX = screenWidth/2, lastY = screenHeight/2; //��ʼ���λ��
bool firstMouse = false;  //�Ƿ��һ�ε�������¼�

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) // ���bool������ʼʱ���趨Ϊtrue��
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float deltaX = xpos - lastX;
    float deltaY = lastY - ypos;   //ע����Ļ�ռ����Ͻ�Ϊԭ�㣬y�����Զ������𽥱��

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //���ð汾��Ϊ3.3
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
    stbi_set_flip_vertically_on_load(true);
    //����Ȼ���
    glEnable(GL_DEPTH_TEST);

    Shader myShader("..\\shader\\vertex.txt", "..\\shader\\fragment.txt");
    Model planet("..\\model\\planet\\planet.obj");
    Model rock("..\\model\\rock\\rock.obj");

    unsigned int amount = 10000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // ��ʼ���������	
    float radius = 50.0f;
    float offset = 2.5f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model(1.0f);
        // 1. λ�ƣ��ֲ��ڰ뾶Ϊ 'radius' ��Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // �����Ǵ��ĸ߶ȱ�x��z�Ŀ��ҪС
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. ���ţ��� 0.05 �� 0.25f ֮������
        float scale = (rand() % 20) /100 + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. ��ת������һ�����룩���ѡ�����ת�����������������ת
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. ��ӵ������������
        modelMatrices[i] = model;
    }

    unsigned int rockVBO;
    glGenBuffers(1, &rockVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rockVBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMatrices, GL_STATIC_DRAW);

    for (int i = 0; i < rock.meshes.size(); i++) {
        unsigned int VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, 0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)vec4Size);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2*vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3*vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    unsigned int planetVBO;
    glGenBuffers(1, &planetVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planetVBO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    glBufferData(GL_ARRAY_BUFFER, sizeof(model), &model, GL_STATIC_DRAW);

    for (int i = 0; i < planet.meshes.size(); i++) {
        unsigned int VAO = planet.meshes[i].VAO;
        glBindVertexArray(VAO);
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, 0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)vec4Size);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }


    while (!glfwWindowShouldClose(window))
    {
        //����
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window);

        //��Ⱦָ��
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();

        glm::mat4 project = glm::mat4(1.0f);
        project = glm::perspective(glm::radians(camera.Zoom), screenWidth/screenHeight, 0.1f, 1000.0f);
        
        myShader.use();
       
        myShader.setMatirx4f("view", view);
        myShader.setMatirx4f("project", project);

        glBindTexture(GL_TEXTURE_2D, planet.textures_loaded[0].id);
        for (int i = 0; i < planet.meshes.size(); i++) {
            glBindVertexArray(planet.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, planet.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, 1);
        }

        glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id);
        for (int i = 0; i < rock.meshes.size(); i++) {
            glBindVertexArray(rock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        }
        

        //��鲢�����¼�����������
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(myShader.ID);
    glfwTerminate();
    return 0;
}