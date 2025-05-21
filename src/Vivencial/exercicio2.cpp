#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Vertex shader
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)";

// Fragment shader
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.3, 0.7, 0.9, 1.0);
    }
)";

// Função para criar triângulo
GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    float vertices[] = {
        x0, y0,
        x1, y1,
        x2, y2
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
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Exercicio 2 - Varios Triangulos", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Erro ao inicializar GLAD\n";
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

    // Criar múltiplos triângulos
    std::vector<GLuint> triangleVAOs;
    triangleVAOs.push_back(createTriangle(-0.8f, -0.8f, -0.6f, -0.8f, -0.7f, -0.6f)); // Triângulo 1
    triangleVAOs.push_back(createTriangle(-0.4f, -0.8f, -0.2f, -0.8f, -0.3f, -0.6f)); // Triângulo 2
    triangleVAOs.push_back(createTriangle(0.0f, -0.8f, 0.2f, -0.8f, 0.1f, -0.6f));    // Triângulo 3
    triangleVAOs.push_back(createTriangle(0.4f, -0.8f, 0.6f, -0.8f, 0.5f, -0.6f));    // Triângulo 4
    triangleVAOs.push_back(createTriangle(-0.2f, -0.4f, 0.0f, -0.4f, -0.1f, -0.2f));  // Triângulo 5

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        for (GLuint vao : triangleVAOs) {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
