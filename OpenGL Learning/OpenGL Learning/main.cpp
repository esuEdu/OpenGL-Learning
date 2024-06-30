//
//  main.cpp
//  OpenGL Learning
//
//  Created by Eduardo on 29/06/24.
//

#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include <iostream>


static unsigned int CompilerShader(unsigned int type, const std::string& source) {
    
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    
    glShaderSource(id, 1, &src, nullptr);
    
    glCompileShader(id);
    
    // TODO: error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    
    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    
    return  id;
    
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    
    unsigned int program = glCreateProgram();
    
    unsigned int vs = CompilerShader(GL_VERTEX_SHADER, vertexShader); // vertexShader
    unsigned int fs = CompilerShader(GL_FRAGMENT_SHADER, fragmentShader); //fragmentShader
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    // Check for linking errors
    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        int length;
        
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << message << std::endl;
    }

    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void) {
    GLFWwindow* window;
    
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
  
    
    glfwSetErrorCallback(error_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(640, 480, "OpenGL Learning", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    float positions[] = {
        -0.5f, -0.5f,//bottom left
        0.0f,  0.5f, //top
        0.5f, -0.5f //bottom right
    };
    
    unsigned int VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
    
    // unbinding VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // unbinding VAO
    glBindVertexArray(0);
    
    std::string vertexShader =
    "#version 410 core\n"
    "layout(location = 0) in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = position;\n"
    "}\n";
    
    std::string fragmentShader =
    "#version 410 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(1.0, 1.0, 0.0, 1.0);\n"
    "}\n";
    

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);
    glBindVertexArray(VAO);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
