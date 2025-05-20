#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

// Estrutura para representar um triângulo
struct Triangle {
    vec3 position;
    vec3 dimensions;
    vec3 color;
    GLuint vao; 
};

vector<Triangle> triangles;
vector<vec2> clickPoints;

// Cores dos triângulos
vector<vec3> colors = {
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(1.0f, 1.0f, 0.0f),
    vec3(1.0f, 0.0f, 1.0f),
    vec3(0.0f, 1.0f, 1.0f)
};
int iColor = 0;

GLuint shaderProgram;

GLuint createTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    float vertices[] = {
        (x1 / 400.0f - 1.0f), (1.0f - y1 / 300.0f), 0.0f,
        (x2 / 400.0f - 1.0f), (1.0f - y2 / 300.0f), 0.0f,
        (x3 / 400.0f - 1.0f), (1.0f - y3 / 300.0f), 0.0f,
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        cout << "Clique em: " << xpos << "  " << ypos << endl;

        clickPoints.push_back(vec2(xpos, ypos));

        if (clickPoints.size() == 3) {
            Triangle tri;
            tri.position = vec3(0.0, 0.0, 0.0);
            tri.dimensions = vec3(1.0, 1.0, 1.0);
            tri.color = colors[iColor];
            tri.vao = createTriangle(
                clickPoints[0].x, clickPoints[0].y,
                clickPoints[1].x, clickPoints[1].y,
                clickPoints[2].x, clickPoints[2].y
            );
            iColor = (iColor + 1) % colors.size();

            triangles.push_back(tri);
            clickPoints.clear();
        }
    }
}

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
void main()
{
    gl_Position = model * vec4(aPos, 1.0);
})";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 triangleColor;
void main()
{
    FragColor = vec4(triangleColor, 1.0f);
})";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vivencial Arthur,Raian,João", NULL, NULL);
    if (window == NULL) {
        cout << "Falha ao criar a janela GLFW" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Falha ao inicializar GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Compilação dos shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        for (Triangle& tri : triangles) {
            glBindVertexArray(tri.vao);

            mat4 model = mat4(1.0f);
            model = translate(model, tri.position);
            model = scale(model, tri.dimensions);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, value_ptr(model));

            glUniform3fv(glGetUniformLocation(shaderProgram, "triangleColor"), 1, value_ptr(tri.color));

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}