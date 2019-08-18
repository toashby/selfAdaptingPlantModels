#include "meshbuilder.h"

meshBuilder::meshBuilder()
{

}

meshBuilder::meshBuilder(float _cameraDistance)
{
    std::cout<<_cameraDistance<<std::endl;
    //set the number quality of branch cylinders based on the distance to camera, a fixed LOD
    m_axialResolution = 8 - int(_cameraDistance / 10);
    if(m_axialResolution < 3)
    {
        m_axialResolution = 3;
    }
}

void meshBuilder::createTube(glm::vec3 _startVector, glm::vec3 _endVector, float _thickness, float _parentThickness)
{
    glm::vec3 random1 = glm::vec3(1,0,0);//'random' vector to cross with branch to get perpendicular vector
    glm::vec3 random2 = glm::vec3(0,1,0);//in case random1 is identical to branch, we use this

    //find perpendicular vector
    glm::vec3 perpendicularVec = glm::cross(_endVector - _startVector, random1);
    if(perpendicularVec == glm::vec3(0.0,0.0,0.0))//vector is in same direction?
    {
        perpendicularVec = glm::cross(_endVector - _startVector, random2);//use this one instead
    }

    //set perpendicular vec length to 1/2*thickness
    perpendicularVec = glm::normalize(perpendicularVec) * (_thickness/2);

    //our quaternion describing the rotation of perpendicularvec
    glm::quat tubeRotate = glm::angleAxis(glm::radians(360.0f/m_axialResolution), glm::normalize(glm::vec3(_endVector - _startVector))); //(vector, angle)

    //if flag is set saying the least note was an end/the last visible node
    if(m_pushStartVecsFlag == true){
        if(m_counter > 0)
            //set the thickness to the parent thickness to stop a jump
            perpendicularVec = glm::normalize(perpendicularVec) * (_parentThickness/2);

        for(int i = 0; i < m_axialResolution; i++)
        {
            //push the start vectors, not just end, as we have jumped back down the tree
            perpendicularVec = tubeRotate*perpendicularVec;
            meshVertices.push_back(modelMatrix * glm::vec4(_startVector + perpendicularVec, 1));
        }
        m_pushStartVecsFlag = false;
    }
    if(m_counter == 1){
        for(int i = 0; i < m_axialResolution; i++)
        {
            //push start vectors on first branch// stops the tree appearing to be floating
            perpendicularVec = tubeRotate*perpendicularVec;
            meshVertices.push_back(modelMatrix * glm::vec4(_startVector + perpendicularVec, 1));
        }
    }

    for(int i = 0; i < m_axialResolution; i++)
    {
        perpendicularVec = tubeRotate*perpendicularVec;
        meshVertices.push_back(modelMatrix * glm::vec4(_endVector + perpendicularVec, 1));
    }

    //we dont run this the first time(can't access -indicies)
    if(m_counter > 0)
    {
        for(int i = 0; i < m_axialResolution; i++)
        {
            if(i != m_axialResolution - 1)
            {
                //top left triangle
                meshIndices.push_back(meshVertices.size() - i + m_offset);
                meshIndices.push_back(meshVertices.size() - i - 1 + m_offset);
                meshIndices.push_back(meshVertices.size() - i - m_axialResolution + m_offset);
                //bottob right triangle
                meshIndices.push_back(meshVertices.size() - i - 1 + m_offset);
                meshIndices.push_back(meshVertices.size() - i - m_axialResolution - 1 + m_offset);
                meshIndices.push_back(meshVertices.size() - i - m_axialResolution + m_offset);
            }else{//the end 2 triangles / these access size and 0 - special case, they wrap around
                meshIndices.push_back(meshVertices.size() - i + m_offset);
                meshIndices.push_back(meshVertices.size() - 0 + m_offset);
                meshIndices.push_back(meshVertices.size() - i - m_axialResolution + m_offset);

                meshIndices.push_back(meshVertices.size() - 0 + m_offset);
                meshIndices.push_back(meshVertices.size() - i - 1 + m_offset);
                meshIndices.push_back(meshVertices.size() - i - m_axialResolution + m_offset);
            }
        }
    }

    m_counter++;
}

void meshBuilder::saveMesh(std::string _modelID)
{
    std::ofstream outdata; // outdata is like cin

    outdata.open("output/output.obj", std::ios::app); // opens the file
       if( !outdata ) { // file couldn't be opened
          std::cerr << "Error: file could not be opened" << std::endl;
          exit(1);
       }

    outdata << "o " << _modelID <<std::endl;

    for (unsigned int i=0; i<meshVertices.size(); i++)
          outdata <<"v " << meshVertices.at(i)[0] << " " << meshVertices.at(i)[1] << " " << meshVertices.at(i)[2] << std::endl;

    for (unsigned int i=0; i<meshIndices.size();)
    {
        outdata <<"f " << meshIndices.at(i) << " " << meshIndices.at(i+1) << " " << meshIndices.at(i+2) << std::endl;
        i +=3;
    }

    outdata.close();
}
