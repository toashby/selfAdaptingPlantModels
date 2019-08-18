#ifndef INTERNODE_H
#define INTERNODE_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "glm/ext.hpp"
#include <vector>
#include "collisionmanager.h"
#include "meshbuilder.h"

/// \author T ASHBY
/// \version 1.0
/// \date Last Revision 31/07/19 \n

/// \class internode.h
/// \brief represents a branch. contains the original position and the bent transform
/// \todo shift collision detection to another class

class interNode
{
public:
    interNode();

    /// @brief pointer to the branch parent
    interNode *m_parent;
    /// @brief vector of child internodes
    std::vector<interNode> m_children;
    /// @brief matrix to store start position of the node
    glm::vec3 m_startPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    /// @brief end position of the node
    glm::vec3 m_endPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    /// @brief the current recursion depth, set from the branchpoint size
    int m_depth;
    /// @brief origin transformed by model matrix for collisions
    glm::vec3 m_origin; //transformed for collision detection
    /// @brief endPosition transformed by model matrix for collisions
    glm::vec3 m_direction; //transformed for collision detection
    /// @brief start vertex for building the rendered model
    glm::vec4 m_startVec;
    /// @brief end vertex for building the rendered model
    glm::vec4 m_endVec;
    /// @brief the type of segment, 0-base 1-straight 2-lateral
    int m_segmentType; //0-base 1-straight 2-lateral
    /// @brief thickness of the branch
    float m_thickness = 0.2f;
    /// @brief has the branch been hit?
    bool m_hit = false;
    /// @brief visibility for pruning on collision after bending
    bool m_visible = true;
    /// @brief Direction vector for tree construction
    glm::vec3 m_dirVec = glm::vec3(0.0f, 1.0f, 0.0f);
    /// @brief left pointing vector relative to branch direction, for tree construction
    glm::vec3 m_leftVec = glm::vec3(1.0f, 0.0f, 0.0f);

    /// @brief reset the ev sv positions to match the startPosition and endPosition matrices
    void straighten();

    /// @brief check if ray hits triangle
    /// @param[in] _xBendAmount amount to bend the tree along the x axis
    /// @param[in] _zBendAmount amount to bend the tree along the z axis
    void bend(float _xBendAmount, float _zBendAmount, float _bendMultiplier, float _maxBend);

    /// @brief this uses up saved energy from pruning to grow other branches
    void grow(float _energySavedFromPruning, unsigned int _numVisibleBranches);

    /// @brief recursive, multiply sv and ev by model transform
    /// @param[in] _transform the model matrix
    void transformChild(glm::mat4 _transform);

    /// @brief send the current sv and ev to the GPU to render
    /// @param[in] _vertices pointer to list of vertices
    /// @param[in] _indices pointer to list of indices
    void getSimulatedVertices(vector<glm::vec3>* _vertices, vector<unsigned int>* _indices);

    /// @brief recursive, sends verts to trihit to detect collisions
    /// @param[in] _cm collisionManager containing all info to do collisons
    /// @param[in] _modelMatrix the transform of the model
    void checkCollision(collisionManager* _cm, glm::mat4 _modelMatrix);

    /// @brief recursive, set bool visibility to false + all children too
    void setInvisible();

    /// @brief recursive, call meshbuilder with thickness and positions to create tubes
    void makeTube(meshBuilder* mb, float _thickness);

    /// @brief recursive, get the total length of branches after&including the specified node
    /// used with ~vigor approximations and energy redistrubution of pruned branches
    void getLengthOfPrunedBranches(float *_energysavedFromPruning, unsigned int *_numVisibleBranches);

    /// @brief recursively transform all children of the current node
    /// @param[in] _growthTranslate The new position to transfor branches to
    void moveChild(glm::vec4 _growthTranslate);
};

#endif // INTERNODE_H
