
#pragma once

//#ifndef GLEW_STATIC
//#define GLEW_STATIC
//#endif

// GLEW need to include before GLFW.
//#include <GL/glew.h>

#include "GL/gl3w.h"

#include <GLFW/glfw3.h>

//#include "sb7ext.h"

#include <vector>
#include <stdexcept>
#include <iostream>

#include "shader_reader.h"

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "SOIL.h"


namespace shader{


class Shaders
{
    const char * vertexShaderSrcFilePath   = SRC_WITH_SHADERS "/simple.vs.glsl";
    const char * fragmentShaderSrcFilePath = SRC_WITH_SHADERS "/simple.fs.glsl";

    const char * vertexShaderSrcFilePathTex   = SRC_WITH_SHADERS "/texture.vs.glsl";
    const char * fragmentShaderSrcFilePathTex = SRC_WITH_SHADERS "/texture.fs.glsl";

public:

    void load_texture( GLuint & texture, std::string texturePath )
    {
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture ); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
                                                 // Set the texture wrapping parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
        // Set texture filtering parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // Load image, create texture and generate mipmaps
        int width, height;
        unsigned char* image = SOIL_load_image( ( std::string( TEXTURE_FOLDER ) + texturePath ).c_str(), &width, &height, 0, SOIL_LOAD_RGB );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
        glGenerateMipmap( GL_TEXTURE_2D );

        SOIL_free_image_data( image );
        glBindTexture( GL_TEXTURE_2D, 0 ); // Unbind texture when done, so we won't accidentily mess up our texture.
    }

    //GLuint create_VAO(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
    //{
    //    GLuint VBO, EBO, VAO;
    //    glGenVertexArrays(1, &VAO);
    //    glGenBuffers(1, &VBO);
    //    glGenBuffers(1, &EBO);

    //    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    //    glBindVertexArray(VAO);

    //    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(decltype(vertices)::value_type), vertices.data(), GL_STATIC_DRAW);

    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(decltype(indices)::value_type), indices.data(), GL_STATIC_DRAW);

    //    // Position attribute
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    //    glEnableVertexAttribArray(0);
    //    // Color attribute
    //    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //    glEnableVertexAttribArray(1);
    //    // TexCoord attribute
    //    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    //    glEnableVertexAttribArray(2);

    //    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    //    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    //    return VAO;
    //}

    GLuint create_shader_program()
    {
        std::string vertexShaderSrc = ShaderReader().read( vertexShaderSrcFilePath );
        std::string fragmentShaderSrc = ShaderReader().read( fragmentShaderSrcFilePath );

        GLuint vertexShader   = create_gl_vertex_shader( vertexShaderSrc );
        GLuint fragmentShader = create_gl_fragment_shader( fragmentShaderSrc );

        // Link shaders
        GLuint shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        glLinkProgram(shaderProgram);

        // Check for compile time errors
        if (!is_compilation_successful(shaderProgram, true))
            throw std::runtime_error("Linking shader was failed!");

        // Delete used shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }

    GLuint create_gl_vertex_shader(const std::string & shaderSrc)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char * file = shaderSrc.c_str();
        glShaderSource(vertexShader, 1, &file, NULL);
        glCompileShader(vertexShader);

        // Check for compile time errors
        if (!is_compilation_successful(vertexShader))
            throw std::runtime_error("Vertex shader compilation error!");

        return vertexShader;
    }

    GLuint create_gl_fragment_shader(const std::string & shaderSrc)
    {
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char * file = shaderSrc.c_str();
        glShaderSource(fragmentShader, 1, &file, NULL);
        glCompileShader(fragmentShader);

        // Check for compile time errors
        if (!is_compilation_successful(fragmentShader))
            throw std::runtime_error("Fragment shader compilation error!");

        return fragmentShader;
    }

    bool is_compilation_successful(GLuint shader, bool isProgram = false)
    {
        GLint success;
        GLchar infoLog[512];

        if (isProgram)
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
        else
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }

        return true;
    }

};


}//namespace shader
