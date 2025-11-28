#if !defined(SHADER_CLASS_H)
#define SHADER_CLASS_H

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <string>

using namespace std;

string get_file_content(string path);


class Shader {
private:
    GLuint _ID;

public:
    const GLuint& ID;
    Shader();
    Shader(string vertexPath, string fragmentPath);
    void bind();
    ~Shader();

    Shader& operator=(Shader&& other) noexcept;
};

#endif // SHADER_CLASS_H
