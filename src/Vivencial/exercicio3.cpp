#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Shader de vértice
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    
    uniform mat4 transform;

    void main() {
        gl_Position = transform * vec4(aPos, 0.0, 1.0);
    }
)";

// Shader de fragmento
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec3 color;

    void main() {
        FragColor = vec4(color, 1.0);
    }
)";

// Estrutura Triangle
struct Triangle {
    glm::vec2 position;
    glm::vec3 color;
};

// Triângulos ativos
std::vector<Triangle> triangles;

// Callback de mouse
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Converter coordenadas do mouse para OpenGL (-1 a 1)
        float ndcX = static_cast<float>(x) / width * 2.0f - 1.0f;
        float ndcY = 1.0f - static_cast<float>(y) / height * 2.0f;

        Triangle t;
        t.position = glm::vec2(ndcX, ndcY);
        t.color = glm::vec3(
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX
        );

        triangles.push_back(t);
    }
}

// Criar VAO para triângulo padrão
GLuint createDefaultTriangleVAO() {
    float vertices[] = {
        -0.1f, -0.1f,
         0.1f, -0.1f,
         0.0f,  0.1f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

// Compilar shaders
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Erro ao compilar shader: " << log << std::endl;
    }

    return shader;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) {
        std::cerr << "Erro ao iniciar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangulos com Transformacao", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Erro ao carregar GLAD\n";
        return -1;
    }

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint triangleVAO = createDefaultTriangleVAO();

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(triangleVAO);

        for (const Triangle& t : triangles) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(t.position, 0.0f));
            GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

            GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
            glUniform3fv(colorLoc, 1, &t.color[0]);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
