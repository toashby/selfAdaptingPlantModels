#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

/// \author T ASHBY
/// \version 1.0
/// \date Last Revision 18/08/19 \n

/// \class meshBuilder.h
/// \brief builds the mesh and saves it to disk
/// \todo

class meshBuilder
{
public:
    meshBuilder();
    /// @brief constructor
    /// @param[in] _cameraDistance The distance of the current tree root to the camera.
    meshBuilder(float _cameraDistance);
    /// @brief A list of mesh vertices to be saved to disk
    std::vector<glm::vec3> meshVertices;
    /// @brief A list of mesh indices to be saved to disk
    std::vector<int> meshIndices;
    /// @brief save indices when branching off, to use when we reach an end node
    std::vector<std::vector<int>> savedIndices;
    /// @brief the resolution of the tube mesh
    int m_axialResolution = 3;
    /// @brief counter representing the current cylinder being drawn
    int m_counter = 0;
    /// @brief The offset index to allow multiple meshes to be stored in the same obj
    unsigned int m_offset;
    /// @brief flag set when starting a new branch, cases the start and end of the branch to be saved to disk instead of just the end
    bool m_pushStartVecsFlag = false;
    /// @brief matrix sotring the tree position, rotation and scale
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    /// @brief create a cylinder ready to save it to disk
    /// @param[in] _startVector The start position of the branch
    /// @param[in] _endVector The end position of the branch
    /// @param[in] _thickness The thickness of the branch
    /// @param[in] _parentThickness the thickness of the parent branch
    void createTube(glm::vec3 _startVector, glm::vec3 _endVector, float _thickness, float _parentThickness);

    /// @brief save the built mesh to output.obj
    /// @param[in] _modelID a unique mesh id taken from a pointer to it, ensures each mesh has a unique name when saved to the .Obj
    void saveMesh(std::string _modelID);
};

#endif // MESHBUILDER_H
