#pragma once

#include <glad\glad.h> //place before any depdency used OpenGL
#include <GLFW/glfw3.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

// Forward declare classes
class Shader;

// Window constants
extern const GLuint SCREEN_WIDTH;
extern const GLuint SCREEN_HEIGHT;

// Global variables defined in main.cpp
extern GLFWwindow* window;
extern GLuint VAO;
extern GLuint VBO;
extern GLuint EBO;
extern Shader shader;

// Timing variables
extern std::chrono::time_point<std::chrono::high_resolution_clock>
_process_start_time,
_process_end_time,
_lates_process_time,
_lates_render_time;

extern std::chrono::milliseconds _process_duration;
extern std::chrono::milliseconds elapsed_time;

extern float fps;

// Data arrays
extern GLfloat vertices[];
extern GLuint indices[];

extern bool isDebugMode;

// File outputs
extern std::ofstream fileOut;
extern std::ofstream fileErr;

// Function declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int _setup();
void _terminate();

