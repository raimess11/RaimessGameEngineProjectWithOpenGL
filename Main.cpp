#include "Main.h"


#include "Game.h"
#include "PhysicEngine.h"
#include "EventHandler.h"
#include "Render.h"

using namespace std;
using namespace chrono;

#ifdef _WIN32
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 1;               // NVIDIA laptops
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;        // AMD laptops
}
#endif

const GLuint SCREEN_WIDTH = 640;
const GLuint SCREEN_HEIGHT = 640;
float fps = 60;

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
    // log system
    cout.rdbuf(fileOut.rdbuf());
    cerr.rdbuf(fileErr.rdbuf());
    cout << "file has ben created" << endl;

    // glfw
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

    glfwSetKeyCallback(window, key_callback);

    return 1;
}

void _mainloop() {
    cout << "test" << endl;
    while (!glfwWindowShouldClose(window)) {
        _process_start_time = high_resolution_clock::now();
        elapsed_time = duration_cast<milliseconds>(_process_start_time.time_since_epoch() - _lates_process_time.time_since_epoch());
        _lates_process_time = _process_start_time;

        _render();
        _physicEngine();
        _eventHandler();
        _game();

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