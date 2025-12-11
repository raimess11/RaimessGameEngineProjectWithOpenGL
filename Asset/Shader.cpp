#include <Asset/Shader.h>

using namespace std;

string get_file_content(string path) {
    std::ifstream in(path);
    if (in.is_open()) {
        std::string contents((std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());
        in.close();
        cout << contents << endl;
        return contents;
    }
    cerr << path << " FILE NOT FOUND!\n";
    return "";
}

Shader::Shader() : ID(_ID) {
    _ID = 0;
}

Shader::Shader(string vertexPath, string fragmentPath) : ID(_ID) {
    string vertexFile = get_file_content(vertexPath);
    string fragmentFile = get_file_content(fragmentPath);

    const char* vertexShaderSource = vertexFile.c_str();
    const char* fragmentShaderSource = fragmentFile.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    int success;
    char infoLog[512];

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    _ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINK_FAILED\n"
            << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::bind() {
    glUseProgram(ID);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

Shader& Shader::operator=(Shader&& other) noexcept {
    // Move resources from 'other' to 'this'
    _ID = other._ID;

    // ... move other resources ...
    other._ID = 0; // Invalidate moved-from object
    return *this;
}
