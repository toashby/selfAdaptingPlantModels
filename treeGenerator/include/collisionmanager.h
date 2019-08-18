#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
//#include "internode.h"

/// \author T ASHBY
/// \version 1.0
/// \date Last Revision 31/07/19 \n

/// \class collisionmanager.h
/// \brief stores information on meshes and hit branches for collision detection
/// \todo

class interNode;//forward declare to avoid an include loop

class collisionManager
{
public:
    collisionManager();

    /// @brief vector of vectors of vertex
    std::vector<std::vector<Vertex>*> m_vertLists; //vector of ptr to vert lists
    /// @brief vector of vector to indices
    std::vector<std::vector<unsigned int>*> m_indLists; //vector of ptr to indices lists
    /// @brief vector of model matrix for each object, for correct collision detection w/ transformed points
    std::vector<glm::mat4> m_modelMatrices;
    /// @brief vector of pointers to hit internodes
    std::vector<interNode*> m_hitBranches;

    bool triHit(glm::vec3 _t0, glm::vec3 _t1, glm::vec3 _t2, glm::vec3 *_origin, glm::vec3 *_direction, glm::vec4 *_startVec, glm::vec4 *_endVec, bool *_hit);
};

#endif // COLLISIONMESHES_H
