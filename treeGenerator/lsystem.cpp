#include "lsystem.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

lSystem::lSystem()
{

}

lSystem::lSystem(string directory)
{
    this->m_directory = directory;
}

void lSystem::generate()
{
    //add an empty node here because we access size - 1
    interNode in;
    in.m_segmentType = 1;
    m_firstNode = in;
    m_currentNode = &m_firstNode;

    std::string ch = "";
    /// @todo change this to a switch statement for speed
    for (unsigned int i = 0; i < m_fullString.length(); i++){
        ch = m_fullString.at(i);

        if (ch.compare("F") == 0 || ch.compare("X") == 0){
            drawLine();
        } else if (ch.compare("[") == 0){//start a branch
            push();
        } else if (ch.compare("]") == 0){//end a branch
            pop();
        } else if (ch.compare("V") == 0){//leaf?
            leaf();
        } else if (ch.compare("+") == 0){//rotate cw
            rotR();
        } else if (ch.compare("-") == 0){//rotate ccw
            rotL();
        } else if (ch.compare("&") == 0){//rotate cw
            pitchU();
        } else if (ch.compare("^") == 0){//rotate ccw
            pitchD();
        } else if (ch.compare("Y") == 0){//swivel
            rollL();
        } else if (ch.compare("U") == 0){//swivel
            rollR();
            }
    }
}

void lSystem::expandString()
{
    for(int gen = 0; gen < m_generations; gen++){
    string ch = "";

        for (unsigned int i = 0; i < m_axiom.length(); ++i){
            bool foundRule = false;
            ch = m_axiom.at(i);

            //loop rules vector here
            for(std::size_t i=0; i<m_rules.size(); i++)
            {
                if (ch.compare(m_rules.at(i).m_ruleString) == 0){
                    string tmpStr = m_rules.at(i).getReplacement();
                    m_fullString = m_fullString + tmpStr;
                    i = i + tmpStr.length() - 1;
                    foundRule = true;
                }
            }

            if(foundRule == false){
                m_fullString = m_fullString + m_axiom.at(i);
            }

        }
        if(gen != m_generations-1){
            m_axiom = m_fullString;
            m_fullString.clear();
        }
    }
    std::cout<<"Expanded String: "<<std::endl;
        std::cout<<m_fullString<<std::endl;
}

void lSystem::readFile()
{
    const int MAXLINE=256;
    ifstream infile(m_directory);
    if(infile.fail())//is it ok?
       {
           cout<<"Input file error\n"<<endl;
       }

    char oneline[MAXLINE];

    while (infile)
       {
           infile.getline(oneline, MAXLINE);
           //cout << oneline << endl;
           m_fileStrings.push_back(string(oneline));
       }

    //read first 3 lines
    //recursion spec
    m_generations = stoi(m_fileStrings.at(0));
    //angle spec
    m_angleInRadians = stof(m_fileStrings.at(1));
    //thickness spec
    m_thickness = stof(m_fileStrings.at(2));
    //axiom
    m_axiom = m_fileStrings.at(3);
    m_originalAxiom = m_axiom;
    //rules
    //read rest and split by =
    for(std::size_t i=4; i<m_fileStrings.size()-1; ++i)
    {
        int match = -1;//store a rule match index if found

        std::string str = m_fileStrings.at(i);

        //check if rule already exists with same name
        std::string tmprl;
        tmprl = str.at(0);
        for(unsigned int i = 0; i<m_rules.size(); i++)
        {
            if (tmprl.compare(m_rules.at(i).m_ruleString)==0)
            {
                //found a match
                match = i;
            }
        }
        if(match > -1){
            str.erase(0, 2);
            //find : here
            int colonPos = str.find(":");
            if(colonPos > 0){
                //std::cout<<"found colon dupe rule"<<std::endl;
                m_rules.at(match).m_probabilities.push_back(stof(str.substr(colonPos + 1, str.length()-1)));
                //std::cout<<r.probabilities.at(0)<<std::endl;
                str.erase(colonPos, str.length() - colonPos);
                //std::cout<<str<<std::endl;
            }

            m_rules.at(match).m_replacements.push_back(str);

        }else{
            rule r;
            r.m_ruleString = str.at(0);
            str.erase(0, 2);

            //find : here
            int colonPos = str.find(":");
            if(colonPos > 0){
                //std::cout<<"found colon"<<std::endl;
                r.m_stochastic = true;
                r.m_probabilities.push_back(0.0f);
                r.m_probabilities.push_back(stof(str.substr(colonPos + 1, str.length()-1)));
                //std::cout<<r.probabilities.at(1)<<std::endl;
                str.erase(colonPos, str.length() - colonPos);
                //std::cout<<str<<std::endl;
            }

            //std::cout<<str<<std::endl;
            r.m_replacements.push_back(str);
            m_rules.push_back(r);
        }
    }
}

void lSystem::drawLine()
{
    //new node
    interNode in;
    in.m_startPosition = m_currentNode->m_endPosition;

    glm::mat4 tMat = glm::mat4(1.0f);
    glm::vec3 dirLength = m_currentNode->m_dirVec * pow(m_lengthMultiplier, m_branchPoints.size());
    tMat[3] = glm::vec4(in.m_startPosition + dirLength, 1);

    in.m_endPosition = glm::vec3(tMat[3]);

    //push start vec to gpu
    m_vertices.push_back(in.m_startPosition);
    //in.m_startVec = glm::vec4(in.m_startPosition, 1);
    m_indices.push_back(m_indices.size());

    //push end vec to gpu
    m_vertices.push_back(in.m_endPosition);
    m_indices.push_back(m_indices.size());

    //set the parent
    in.m_parent = m_currentNode;

    //set segment type
    if(m_currentNode->m_segmentType == 1){//main branch
        if(m_branchPoints.size() < 1){
            //cout<<"setting as trunk"<<endl;
            in.m_segmentType = 1;//trunk
        }else{
            in.m_segmentType = 2;//shoot
        }
    }


    //set the thickness of the branch
    in.m_thickness = m_thickness;
    in.m_thickness = in.m_thickness * pow(m_thicknessMultiplier, m_branchPoints.size());

    //set an updated radius if necessary
    if(glm::distance(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(in.m_endPosition[0], 0.0f, in.m_endPosition[2])) > m_radius)
    {
        m_radius = glm::distance(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(in.m_endPosition[0], 0.0f, in.m_endPosition[2]));
    }

    in.m_dirVec = glm::normalize(in.m_endPosition - in.m_startPosition);

    m_currentNode->m_children.push_back(in);
    m_currentNode = &m_currentNode->m_children.back();
}

void lSystem::push()//create branch
{
    lefts.push_back(m_currentNode->m_leftVec);
    dirs.push_back(m_currentNode->m_dirVec);

    m_branchPoints.push_back(m_currentNode);//save pointer to node before branch
}

void lSystem::pop()//end branch
{
    m_currentNode = m_branchPoints.back();

    m_currentNode->m_leftVec = lefts.back();
    lefts.pop_back();

    m_currentNode->m_dirVec = dirs.back();
    dirs.pop_back();

    m_branchPoints.pop_back();

}

void lSystem::leaf()
{

}

void lSystem::rotR()
{   
    coneMat = glm::rotate(glm::mat4(1.0f), m_angleInRadians, m_currentNode->m_leftVec);

    m_currentNode->m_dirVec = coneMat * glm::vec4(m_currentNode->m_dirVec, 0);
}

void lSystem::rotL()
{
    coneMat = glm::rotate(glm::mat4(1.0f), -m_angleInRadians, m_currentNode->m_leftVec);

    m_currentNode->m_dirVec = coneMat * glm::vec4(m_currentNode->m_dirVec, 0);
}

void lSystem::rollL()//swivel
{
    swivelMat = glm::rotate(glm::mat4(1.0f), m_angleInRadians, m_currentNode->m_dirVec);

    m_currentNode->m_leftVec = glm::vec4(m_currentNode->m_leftVec, 0) * swivelMat;
}

void lSystem::rollR()//swivel
{
    swivelMat = glm::rotate(glm::mat4(1.0f), -m_angleInRadians, m_currentNode->m_dirVec);

    m_currentNode->m_leftVec = swivelMat * glm::vec4(m_currentNode->m_leftVec, 0);
}

void lSystem::pitchU()
{
    //m_currentNode->m_endPosition = glm::rotate(m_currentNode->m_endPosition, m_angleInRadians, glm::vec3(0, 0, 1));
}

void lSystem::pitchD()
{
   //m_currentNode->m_endPosition = glm::rotate(m_currentNode->m_endPosition, -m_angleInRadians, glm::vec3(0, 0, 1));
}


