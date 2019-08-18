#include "linemodel.h"


LineModel::LineModel()
{

}

LineModel::LineModel(string directory)
{
    //set directory
    this->directory = directory;
}

LineModel::LineModel(string directory, collisionManager *cMan)
{
    //set directory and collision manager
    this->directory = directory;
    this->cMan = cMan;
}

void LineModel::createModel(float _generations){
    //set lsystem directory to point to plant txt file
    m_lSystem.m_directory = this->directory;
    //read the file
    m_lSystem.readFile();
    //vary the number of generations
    m_lSystem.m_generations = _generations * m_lSystem.m_generations;
    //expand the axiom based on specified rules
    m_lSystem.expandString();
    //generate the tree skeleton
    m_lSystem.generate();
}

void LineModel::react()
{
        //clear any previous collisions
        cMan->m_hitBranches.clear();
        //reset the mesh
        m_lSystem.m_firstNode.straighten();
        //trnasform with model matrix
        m_lSystem.m_collisionTransform = modelMatrix;
        //recursively check collisions
        m_lSystem.m_firstNode.checkCollision(cMan, modelMatrix);

        //if we hit something
        if(cMan->m_hitBranches.size() > 0)
        {
            //get average of hit node directions
            glm::vec3 hitDir = glm::vec3(0.0f, 0.0f, 0.0f);
            for(unsigned int i = 0; i < cMan->m_hitBranches.size(); i++)
            {
                hitDir = hitDir + glm::vec3(cMan->m_hitBranches.at(i)->m_startVec);
            }

            //calculate x and z bend amount proportions
            float xz = abs(hitDir[0]) + abs(hitDir[2]);
            float xBendAmount = hitDir[0] / xz;
            float zBendAmount = -hitDir[2] / xz;

            //now multiply by max bend(radians)
            //xBendAmount *= maxBend;
            //xBendAmount *= maxBend;

            vertices.clear();
            indices.clear();

            m_lSystem.m_firstNode.bend(xBendAmount, zBendAmount, m_bendMultiplier, m_maxBend);

            //prune the branches here
            cMan->m_hitBranches.clear();
            m_lSystem.m_firstNode.checkCollision(cMan, modelMatrix);
            for(unsigned int i = 0; i < cMan->m_hitBranches.size(); i++)
            {
                //never hide the trunk, only lateral shoots
                if(cMan->m_hitBranches.at(i)->m_segmentType == 2)
                {
                    cMan->m_hitBranches.at(i)->setInvisible();
                }
            }

            //work out length of pruned branches
            m_lSystem.m_firstNode.getLengthOfPrunedBranches(&m_energySavedFromPruning, &m_numVisibleBranches);
            std::cout<< "m_energySavedFromPruning: " << m_energySavedFromPruning<<std::endl;
            std::cout<< "m_numVisibleBranches: " << m_numVisibleBranches<<std::endl;

            //grow the branches using saved energy
            m_lSystem.m_firstNode.grow(m_energySavedFromPruning, m_numVisibleBranches);

            m_lSystem.m_firstNode.getSimulatedVertices(&vertices, &indices);
            setupMesh();

        }else{//reset, no collisions
            vertices.clear();
            indices.clear();

            m_lSystem.m_firstNode.getSimulatedVertices(&vertices, &indices);
            setupMesh();
        }  
}

int LineModel::createMesh(string _modelID, unsigned int _offset, glm::vec3 _cameraPosition)
{
    std::cout<<"creating a mesh"<<std::endl;
    //calculate LOD
    //abs transform - camera
    float cameraDistance = glm::distance(_cameraPosition, glm::vec3(m_lSystem.m_collisionTransform[3]));

    //pass the camera distance to mesh builder
    meshBuilder mb(cameraDistance);

    mb.m_offset = _offset; //offset for vertex indices
    mb.modelMatrix = modelMatrix;
    //make a cylinder at the current node
    m_lSystem.m_firstNode.makeTube(&mb, m_lSystem.m_firstNode.m_thickness);
    //save the mesh to file with a unique id
    mb.saveMesh(_modelID);
    //reutrn the vertices size to use as an offset for the next mesh
    return mb.meshVertices.size();
}
