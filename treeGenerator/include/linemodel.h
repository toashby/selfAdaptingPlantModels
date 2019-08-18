#ifndef LINEMODEL_H
#define LINEMODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>
#include <lsystem.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "collisionmanager.h"
#include "meshbuilder.h"

using namespace std;

/// \author T ASHBY
/// \version 1.0
/// \date Last Revision 18/08/19 \n

/// \class linemodel.h
/// \brief stores a model as a set of line segments
/// \todo
class LineModel
{
public:
    LineModel();
    LineModel(string directory);
    LineModel(string directory, collisionManager *cMeshes);
    /*  Model Data */
    /*  Mesh Data  */
    /// @brief vector of vertices to render
    vector<glm::vec3> vertices;
    /// @brief vector of indices to the vertices for rendering
    vector<unsigned int> indices;
    //vector<Texture> textures;
    unsigned int VAO;
    /// @brief the lSystem object
    lSystem m_lSystem;
    /// @brief the transform of the model
    glm::mat4 modelMatrix = glm::mat4(1.0f); //old - may need?
    /// @brief collisionManager Object storing collision object info
    collisionManager *cMan;
    /// @brief the maximum initial bend angle
    float m_maxBend;
    ///@brief the bend multiplier
    float m_bendMultiplier;
    /// @brief the model color for visualisation and easier identification
    glm::vec4 m_color;
    /// @brief the energy saved by pruning branches, will be used elsewhere
    float m_energySavedFromPruning = 0.0f;
    /// @brief the current number of visible branches for calculting energy distribution
    unsigned int m_numVisibleBranches = 0.0f;

    /// @brief create the initial model, get vertices and indices
    /// /// @brief _generations the number of l-system generations when epanding the axiom
    void createModel(float _generations);

    /// @brief has the gerometry react to the environment, currently called on WASD keyup and or program start up
    void react();

    /// @brief create a mesh
    /// @param[in] _modelID
    /// @param[in] _offset mesh index offset
    /// @param[in] _cameraPosition Obvious? - used in LOD calculation
    int createMesh(string _modelID, unsigned int _offset, glm::vec3 _cameraPosition);

    /*  Functions   */
    /// @brief draw the model with the openGL window
    /// @param[in] shader our shader
    void Draw(Shader shader)
    {
        // draw mesh
        glUseProgram(shader.ID);
        //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        //glActiveTexture(GL_TEXTURE0);
    }


    /*  Functions    */
    /// @brief initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glBindVertexArray(0);
    }


private:
    /*  Functions   */

    /*  Render data  */
    ///@brief the model vertex array object and element buffer object
    unsigned int VBO, EBO;
    /// @brief directory of our lSystem description file
    string directory;

};

#endif // LINEMODEL_H
