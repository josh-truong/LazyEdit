#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

class Shader
{
    // private:

    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();

        void use();

        void setBool_u1i(const std::string &name, bool value) const;
        void setInt_u1i(const std::string &name, int value) const;
        void setFloat_u1f(const std::string &name, float value) const;
};

#endif