#include "sceneloader.h"

sceneLoader::sceneLoader()
{

}

void sceneLoader::loadScene(std::vector<Model> *_models, std::vector<LineModel> *_lineModels, collisionManager *_cMan)
{
    std::ifstream ifs;
    ifs.open("trees/scene.json", std::ifstream::in);

    if(!ifs.is_open()){
        std::cout << "error opening file" <<std::endl;
    }
    if(ifs.fail()){
        std::cout << "error: some sort of file failure" <<std::endl;
    }

        nlohmann::json j; //j.size is the number of elements to make
        ifs >> j; //Fill j with i
        nlohmann::json::iterator it = j.begin();

        for(unsigned int i = 0; i < j.size(); i++){

            std::string oType = it->at("oType").dump(); //get as a string

            if(oType == "\"plant\""){//we are loading a plant
                unsigned int amount = it->at("amount");
                unsigned int sizeX = it->at("xSpread");
                unsigned int sizeY = it->at("ySpread");
                float scale = it->at("scale");
                float maxBend = it->at("maxBend");
                float bendMultiplier = it->at("bendMultiplier");
                for(unsigned int i = 0; i < amount; i++)
                {

                    float randomX;
                    float randomY;
                    //check overlaps here, try x times then give up
                    for(int j = 0; j < 50; j++)
                    {
                        randomX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f) * sizeX; //random x position
                        randomY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f) * sizeY; //random y position
                        bool canPlace = true;

                        for(unsigned int k = 0; k < _lineModels->size(); k++)
                        {
                            if(glm::distance(glm::vec3(randomX, 0.0f, randomY), glm::vec3(_lineModels->at(k).modelMatrix[3])) < 2.0f)//distance between them is less than (plant size, how do I get this?) //lets say 1 for now?
                            {
                                //std::cout<<"plant is too close, trying again"<<std::endl;
                                canPlace = false;
                                break;
                            }
                        }

                        if(canPlace == true)
                        {
                            LineModel lineModel(std::string(it->at("name")), _cMan);

                            //decide the number of generations to grow for here using a sigmoidal type function
                            float generationVariation = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
                            generationVariation = 2*(generationVariation/(0.25f+generationVariation));
                            if (generationVariation > 1)
                                generationVariation = 1;
                            lineModel.createModel(generationVariation);

                            lineModel.setupMesh();
                            lineModel.m_color = glm::vec4((static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f),  (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f),  (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f), 1.0f);

                            lineModel.m_maxBend = maxBend;
                            lineModel.m_bendMultiplier = bendMultiplier;
                            //choose a random rotation
                            lineModel.modelMatrix = glm::rotate(lineModel.modelMatrix, (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f) * 6.28319f, glm::vec3(0, 1, 0));
                            //translate to the random x y we found
                            lineModel.modelMatrix = glm::translate(lineModel.modelMatrix, glm::vec3(randomX, 0.0f, randomY));
                            float scaleVariation = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f) + 49)/50;//random 2% variation
                            scale *= scaleVariation;
                            //scale based on the json spec
                            lineModel.modelMatrix = glm::scale(lineModel.modelMatrix, glm::vec3(scale, scale, scale));
                            _lineModels->push_back(lineModel);
                            break;
                        }
                    }
                }

            }else if(oType == "\"obj\""){//we are loading an obj file
                Model model("objects/" + std::string(it->at("name"))); //create a model with name specified in scene.json
                model.modelMatrix = glm::translate(model.modelMatrix, glm::vec3(it->at("x"), it->at("y"), it->at("z")));
                model.modelMatrix = glm::scale(model.modelMatrix, glm::vec3(it->at("sx"), it->at("sy"), it->at("sz")));
                _models->push_back(model); //add the created model to our vector
            }

            it++;//set iterator to next element
        }
}
