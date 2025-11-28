#include <glad/glad.h> //place before any depdency used OpenGL
#include <GLFW/glfw3.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>


#include "Shader.h"
#include "Main.h"

using namespace std;
using namespace chrono;

const GLuint SCREEN_WIDTH = 640;
const GLuint SCREEN_HEIGHT = 640;

//_setup variable
GLFWwindow* window;
GLuint VAO;
GLuint VBO;
GLuint EBO;
Shader shader;

//_mainloop variable
time_point<high_resolution_clock>
    _process_start_time = high_resolution_clock::now(),
    _process_end_time = high_resolution_clock::now(),
    _lates_process_time = high_resolution_clock::now(),
    _lates_render_time = high_resolution_clock::now();

milliseconds _process_duration, elapsed_time;
float fps = 60;

GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
GLuint indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

bool isDebugMode;

//_terminate variable

ofstream fileOut("output.txt");
ofstream fileErr("error.txt");

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        isDebugMode = !isDebugMode;
    if (isDebugMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // after isDebugMode Updated
    else  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int _setup() {
    cout.rdbuf(fileOut.rdbuf());
    cerr.rdbuf(fileErr.rdbuf());
    cout << "file has ben created" << endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raimess OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    // ..:: Initialization code :: ..
    // 1. bind Vertex Array Object
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    // 3. copy our index array in a element buffer for OpenGL to use
    // 4. then set the vertex attributes pointers

    // vertex array object
    // Make sure Bind VAO before VBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element Buffer Object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // config Vertex Attribut
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Shader
    shader = std::move(Shader("default.vert", "default.frag"));
    shader.bind();

    glfwSetKeyCallback(window, key_callback);
    return 1;
}

void _gamelogic() {
}

void _render() {
    // fps limit
    if (duration_cast<milliseconds>(_process_start_time.time_since_epoch() - _lates_render_time.time_since_epoch()) < 1000ms / fps) {
        cout << "render skiped at " << high_resolution_clock::now().time_since_epoch().count() << endl;
        return;
    }
    _lates_render_time = high_resolution_clock::now();
    cout << "rendering at " << high_resolution_clock::now().time_since_epoch().count() << endl;

    // do render here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
}

void _eventHandler() {
    glfwPollEvents();
}

void _mainloop() {
    while (!glfwWindowShouldClose(window)) {
        _process_start_time = high_resolution_clock::now();
        elapsed_time = duration_cast<milliseconds>(_process_start_time.time_since_epoch() - _lates_process_time.time_since_epoch());
        _lates_process_time = _process_start_time;

        _render();
        _gamelogic();
        _eventHandler();

        _process_end_time = high_resolution_clock::now();
        _process_duration = duration_cast<milliseconds>(_process_end_time - _process_start_time);
        cout << "process time: " << _process_duration.count() << " ms at - " << high_resolution_clock::now().time_since_epoch().count() << endl;
    }
}

void _terminate() {
    fileOut.close();
    fileErr.close();
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    if (!_setup())
        return -1;
    _mainloop();
    _terminate();

    return 0;
}