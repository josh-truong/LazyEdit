#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

class Shader
{
    public:
        unsigned int ID;

        Shader();
        ~Shader();

        void InitShader(const char* vertexPath, const char* fragmentPath);
        void Use();

        void SetBool1(const std::string &name, bool value) const;
        void SetInt1(const std::string &name, int value) const;
        void SetFloat1(const std::string &name, float value) const;
};

#endif