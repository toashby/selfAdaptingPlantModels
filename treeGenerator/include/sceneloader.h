#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include "nlohmann/json.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>

#include "model.h"
#include "linemodel.h"
#include "collisionmanager.h"

/// \author T ASHBY
/// \version 1.0
/// \date Last Revision 05/08/19 \n

/// \class sceneloader.h
/// \brief handles loading the scene. loads obj's described in scene.json
/// \todo

class sceneLoader
{
public:
    sceneLoader();

    /// @brief reads scene.json and creates models for colliison
    /// @param[in] *_models pointer to a list of models
    /// @param[in] *_lineModels pointer to a list of lineModels
    /// @param[in] *_cMan pointer to an instance of a collision manager
    void loadScene(std::vector<Model> *_models, std::vector<LineModel> *_lineModels, collisionManager *_cMan);

};

#endif // SCENELOADER_H
