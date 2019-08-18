#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "rule.h"
#include "internode.h"
using namespace std;

/// \author T ASHBY
/// \version 1.0
/// \date Last Revision 31/07/19 \n

/// \class lsystem.h
/// \brief the l system. contains all nodes and methods to build the tree
/// \todo

class lSystem
{
public:
    lSystem();
    lSystem(string m_directory);

    /// @brief the expanded lsystem
    std::string m_fullString;
    /// @brief the vertices of the initial tree
    vector<glm::vec3> m_vertices;
    /// @brief the indices of the initial tree
    vector<unsigned int> m_indices;
    /// @brief the first node with position 0,0,0
    interNode m_firstNode;
    /// @brief pointer to current node for tree traversal and construction
    interNode *m_currentNode;
    /// @brief pointers to branching nodes, popped on and off as needed to construct the branches
    vector<interNode*> m_branchPoints; //array of nodes to save branching points
    /// @brief pointers to the nodes at the end of the branches
    vector<interNode*> m_endNodes;
    /// @brief copy of the render transform for collision detection
    glm::mat4 m_collisionTransform; //for collision detection, same as render transform
    /// @brief branch bend(rotate) angle init to default
    float m_angleInRadians = 0.755f;
    /// @brief number of times to expand the axiom
    int m_generations = 1.0f;
    /// @brief initial branch thickness
    float m_thickness;
    /// @brief initial string
    string m_axiom;
    /// @brief copy of the axiom for tree reconstruction
    string m_originalAxiom;
    /// @brief branch length
    float m_distance = 1.0f;
    /// @brief reduction in branch length per segment
    float m_lengthMultiplier = 0.7f;
    /// @brief reduction in thickness through the tree
    float m_thicknessMultiplier = 0.7f;
    /// @brief the rough radiud of the tree based on the furthest point
    /// from the trunk
    float m_radius = 0.0f;
    /// @brief directory of plant description file
    string m_directory;
    /// @brief direction vector of currentNode
    glm::vec3 currentDirection = glm::vec3(0.0f, 1.0f, 0.0f);
    /// @brief std::vector of directions saved when branching
    std::vector<glm::vec3> dirs;
    /// @brief std::vector of left vectors saved when branching
    std::vector<glm::vec3> lefts;
    /// @brief stores a rotation describing a lean along the x axis
    glm::mat4 coneMat = glm::mat4(1.0f);
    /// @brief stores a rotation describing a swivel along the up axis
    glm::mat4 swivelMat = glm::mat4(1.0f);

    /// @brief Parse the expanded string and call functions to build tree
    void generate();

    /// @brief replace the rules with their replacements for x times
    void expandString();

    /// @brief read the plant description txt and create rule objects
    void readFile();

private:
    /// @brief the tree description file split into strings
    vector<string> m_fileStrings;
    /// @brief vector of rule objects
    vector<rule> m_rules;

    /// @brief draw a branch
    void drawLine();

    /// @brief push the current node to vector to return to it later
    void push();

    /// @brief make the last node in the vector the currentNode and pop it off
    void pop();

    /// @brief draw a leaf @todo make it actually draw leaves?
    void leaf();

    /// @brief rotate currentNode matrix right
    void rotR();

    /// @brief rotate currentNode matrix left
    void rotL();

    /// @brief roll currentNode matrix left
    void rollL();

    /// @brief roll currentNode matrix right
    void rollR();

    /// @brief pitch the currentNode matrix up
    void pitchU();

    /// @brief pitch the currentNode matrix down
    void pitchD();

};

#endif // LSYSTEM_H
