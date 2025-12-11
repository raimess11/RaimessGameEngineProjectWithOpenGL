#include "Render.h"
#include "Shader.h"
#include "Main.h"

using namespace std;
using namespace chrono;

// Timing variables

milliseconds _process_duration, elapsed_time;

//_mainloop variable
time_point<high_resolution_clock>
_process_start_time = high_resolution_clock::now(),
_process_end_time = high_resolution_clock::now(),
_lates_process_time = high_resolution_clock::now(),
_lates_render_time = high_resolution_clock::now();

//_setup variable
GLFWwindow* window;
GLuint VAO;
GLuint VBO;
GLuint EBO;
Shader shader;

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

void _draw() {

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
}

void _render() {
    // fps limit
    if (duration_cast<milliseconds>(_process_start_time.time_since_epoch() - _lates_render_time.time_since_epoch()) < 1000ms / fps) {
        cout << "render skiped at " << high_resolution_clock::now().time_since_epoch().count() << endl;
        return;
    }
    _lates_render_time = high_resolution_clock::now();
    cout << "rendering at " << high_resolution_clock::now().time_since_epoch().count() << endl;

    _draw();

    // do render here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
}