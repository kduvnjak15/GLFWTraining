#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GT_Shader.h"


struct Vertex {

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

};

struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};

class GT_Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    GLuint VAO_;

    GT_Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices  = indices;
        this->textures = textures;

        this->setupMesh();
    }

    void Draw(GT_Shader shader_)
    {
        GLuint diffuseNr    = 1;
        GLuint specularNr   = 1;
        GLuint normalNr     = 1;
        GLuint heightNr     = 1;

        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); //Active proper texture unit
            // Retrieve texture number (the N in diffuse_textureN)
            std::stringstream ss;
            std::string number;
            std::string name = this->textures[i].type;

            if (name == "texture_diffuse")
                ss << diffuseNr++; // Transfer GLuint to stream
            else if (name == "texture_specular")
                ss << specularNr++;
            else if (name == "texture_normal")
                ss << normalNr++;
            else if (name == "texture_height")
                ss << heightNr++;
            number = ss.str();
            // Now set the sampler to the correct texture unit
            glUniform1i( glGetUniformLocation(shader_.shaderProgram_, (name + number).c_str()), i);
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }

        // Draw mesh
        glBindVertexArray(this->VAO_);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //Always good practice to set everything back to defaults once configured
        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i );
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    }

private:

    GLuint VBO, EBO;

    // Initializes all the buffer objects/arrays
    void setupMesh()
    {

        //Create buffers/arrays
        glGenVertexArrays(1, &this->VAO_);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO_);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()*sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
        // Vertex Tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
        // Vertex Bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);

    }

};
