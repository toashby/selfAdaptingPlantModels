#include "internode.h"

interNode::interNode()
{

}

void interNode::checkCollision(collisionManager *_cm, glm::mat4 _modelMatrix)
{
    //default is no hit
    m_hit = false;
    //calculate the globally transformed branch origin
    m_origin = _modelMatrix * m_startVec;// + modelMatVec;
    //calculate direction vector
    m_direction = m_endVec - m_startVec;

    //construct triangles here
    //#pragma omp parallel for //no visible performance gained by parallelising here
    for(unsigned int i = 0; i < _cm->m_indLists.size(); i++)
    {
        //attempt to run this for() on all cores
        #pragma omp parallel for
        for(unsigned int j = 0; j < _cm->m_indLists.at(i)->size();j += 3)
        {
            //get triangle vertices
            glm::vec3 v0 = _cm->m_vertLists.at(i)->at(j).Position;
            glm::vec3 v1 = _cm->m_vertLists.at(i)->at(j+1).Position;
            glm::vec3 v2 = _cm->m_vertLists.at(i)->at(j+2).Position;

            v0 = _cm->m_modelMatrices.at(i) * glm::vec4(v0, 1);
            v1 = _cm->m_modelMatrices.at(i) * glm::vec4(v1, 1);
            v2 = _cm->m_modelMatrices.at(i) * glm::vec4(v2, 1);

            _cm->triHit(v0, v1, v2, &m_origin, &m_direction, &m_startVec, &m_endVec, &m_hit);
        }
    }

    if(m_hit == true)
    {
        //if we hit then make the branch invisible
        m_visible = false;
        //add add a pointer to it in hitBranches
        _cm->m_hitBranches.push_back(this);
    }

    for(unsigned int k = 0; k < m_children.size(); k++)
    {
        //check collision of all children
        m_children.at(k).checkCollision(_cm, _modelMatrix);
    }
}


void interNode::setInvisible()
{   //set visibility to false
    m_visible = false;
    //set all children to invisible
    for(unsigned int i = 0; i < m_children.size(); i++)
    {
        m_children.at(i).setInvisible();
    }
}


void interNode::makeTube(meshBuilder *mb, float _thickness)
{
    if(m_visible == true)
    {
        //create vertices around the current branch using meshBuilder
        mb->createTube(m_startVec, m_endVec, m_thickness, _thickness);
    }

    if(m_visible == false || m_children.size() == 0){
        //we are at branch end so set the flag so the next branch has start vertices
        mb->m_pushStartVecsFlag = true;
    }

    if(m_visible == true)
    {
        for(unsigned int i = 0; i < m_children.size(); i++)
        {
            //recursively make meshes around children
            m_children.at(i).makeTube(mb, m_thickness);
        }
    }
}


void interNode::getLengthOfPrunedBranches(float *_energysavedFromPruning, unsigned int *_numVisibleBranches)
{
    if(m_visible == false)
    {
        //add length of invisible branch to _energySavedFromPruning
        *_energysavedFromPruning += glm::distance(m_startVec, m_endVec);
    }else{
        //if visible is true then increment visible branch counter
        *_numVisibleBranches += 1;
    }

    for(unsigned int i = 0; i < m_children.size(); i++)
    {
        //recursive call to perfom same operation on children
        m_children.at(i).getLengthOfPrunedBranches(_energysavedFromPruning, _numVisibleBranches);
    }
}


void interNode::straighten()
{
    //reset start and end vectors to thier iriginal positions
    m_startVec = glm::vec4(m_startPosition, 1);
    m_endVec = glm::vec4(m_endPosition, 1);
    //set visible to true to show all branches
    m_visible = true;

    for(unsigned int i = 0; i < m_children.size(); i++)
    {
        //call straighten on children
        m_children.at(i).straighten();
    }
}


void interNode::bend(float _xBendAmount, float _zBendAmount, float _bendMultiplier, float _maxBend)
{
    //new transform for creating a pivot matrix
    glm::mat4 transform = glm::mat4(1.0);
    //translate by endVec
    transform = translate(transform, glm::vec3(m_endVec));//ev +/- are reversed here
    //rotate along x and y axes
    transform = rotate(transform, _xBendAmount * _maxBend, glm::vec3(0.0, 0.0, 1.0)); //up is y-direction
    transform = rotate(transform, _zBendAmount * _maxBend, glm::vec3(1.0, 0.0, 0.0));
    //translate by negative endVec
    transform = translate(transform, glm::vec3(-m_endVec));

    //transform children in parallel
    #pragma omp parallel for
    for(unsigned int i = 0; i < m_children.size(); i++)
    {
        m_children.at(i).transformChild(transform);
    }

    //bend children
    for(unsigned int i = 0; i < m_children.size(); i++)
    {
        m_children.at(i).bend(_xBendAmount, _zBendAmount, _bendMultiplier, _maxBend);
    }
}


void interNode::grow(float _energySavedFromPruning, unsigned int _numVisibleBranches)
{
    //IF visible == TRUE
    if(m_visible == true)
    {
        //decide a growth value = energySFP / number of visible branches
        float growthLength = _energySavedFromPruning / _numVisibleBranches;
        //get length of branch
        //std::cout<< glm::to_string(m_endVec) <<std::endl;
        glm::vec4 branchVector = m_endVec - m_startVec;
        //set to length 1
        branchVector = glm::normalize(branchVector);
        branchVector *= growthLength;
        glm::mat4 branchMatrix = glm::mat4(1.0f);
        branchMatrix[3] = branchVector;
        //std::cout<< glm::to_string(branchMatrix) <<std::endl;

        //call grow method in all children
        for(unsigned int i = 0; i < m_children.size(); i++)
        {
            m_children.at(i).grow(_energySavedFromPruning, _numVisibleBranches);
        }
    }
}


void interNode::moveChild(glm::vec4 _growthTranslate)
{
    //used for growth
    //work out new branch position
    m_startVec = m_startVec + _growthTranslate;
    m_endVec = m_endVec + _growthTranslate;

    //do the same for children
    for(unsigned int i = 0; i < m_children.size(); i++)
    {
        m_children.at(i).moveChild(_growthTranslate);
    }
}


void interNode::transformChild(glm::mat4 _transform)
{
    //adjust vectors by transform
    m_startVec = _transform * m_startVec;
    m_endVec = _transform * m_endVec;

    //do the same for children
    for(unsigned int i = 0; i < m_children.size(); i++)
    {
        m_children.at(i).transformChild(_transform);
    }
}


void interNode::getSimulatedVertices(vector<glm::vec3> *_vertices, vector<unsigned int> *_indices)
{
    //if visible grab branch positions to send to GPU
    if(m_visible == true)
    {
        _vertices->push_back(glm::vec3(m_startVec));
        _indices->push_back(uint(_indices->size()));
        _vertices->push_back(m_endVec);
        _indices->push_back(uint(_indices->size()));

        //do same for children
        for(unsigned int i = 0; i < m_children.size(); i++)
        {
            m_children.at(i).getSimulatedVertices(_vertices, _indices);
        }
    }
}
