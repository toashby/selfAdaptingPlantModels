#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "lsystem.h"
#include "collisionmanager.h"

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc,argv);

    //std::cout<<"Raytracer Tests\n";

    return RUN_ALL_TESTS();
}

TEST(lSystem, defaultCtor)
{
    lSystem lsys = lSystem("testPlant.txt");

    lsys.readFile();
    lsys.expandString();
    EXPECT_EQ(lsys.m_axiom,"FFFFU[+F]FU[+F]FU[+F]");
}

TEST(lSystem, userCtor)
{
    lSystem lsys = lSystem("testPlant.txt");
    lsys.readFile();
    //manually redefine instead
    lsys.m_axiom = "FFFFAF";
    lsys.expandString();
    EXPECT_EQ(lsys.m_axiom,"FFFFAF");
}

TEST(lSystem, bigPlant)
{
    lSystem lsys = lSystem("testPlant2.txt");
    lsys.readFile();
    lsys.expandString();
    EXPECT_EQ(lsys.m_axiom,"FFF[+FFF[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]YYY[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]YYY[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]]YYY[+FFF[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]YYY[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]YYY[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]]YYY[+FFF[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]YYY[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]YYY[+FFF[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]YYY[+FFF[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]YYY[+FFF[+FFFT]YYY[+FFFT]YYY[+FFFT]]]]]");
}

TEST(meshBuilder, axialResolution)
{
    meshBuilder mb;
    mb.m_axialResolution = 4;
    mb.createTube(glm::vec3(0.0,0.0,0.0), glm::vec3(0.0, 1.0, 0.0), 2.5f, 2.8f);

    EXPECT_EQ(mb.meshVertices.size(), 4);
}

TEST(meshBuilder, axialResolutionDefaultCtor)
{
    meshBuilder mb;
    //mb.m_axialResolution = 4;
    mb.createTube(glm::vec3(0.0,0.0,0.0), glm::vec3(0.0, 1.0, 0.0), 2.5f, 2.8f);

    EXPECT_EQ(mb.meshVertices.size(), 3);
}

TEST(meshBuilder, meshVerticesZPosition)
{
    meshBuilder mb;
    mb.m_axialResolution = 4;
    mb.createTube(glm::vec3(0.0,0.0,0.0), glm::vec3(0.0, 1.0, 0.0), 2.5f, 2.8f);

    EXPECT_EQ(mb.meshVertices.at(0)[1], 1);
    EXPECT_EQ(mb.meshVertices.at(1)[1], 1);
    EXPECT_EQ(mb.meshVertices.at(2)[1], 1);
    EXPECT_EQ(mb.meshVertices.at(3)[1], 1);
}

TEST(collisionManager, trihitMISSES)
{
    collisionManager cm;
    bool hit;
    glm::vec3 origin = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 4.0f, 0.0f);
    glm::vec4 startVec = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 endVec = glm::vec4(0.0f, 1.0f, 14.0f, 1.0f);
    cm.triHit(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), &origin, &direction, &startVec, &endVec, &hit);

    EXPECT_EQ(hit, false);
}

TEST(collisionManager, trihitHITS)
{
    collisionManager cm;
    bool hit;
    glm::vec3 origin = glm::vec3(20.0f, 1.0f, -1.0f);
    glm::vec3 direction = glm::vec3(1.0f, 4.0f, 1.0f);
    glm::vec4 startVec = glm::vec4(0.0f, -10.0f, 0.0f, 1.0f);
    glm::vec4 endVec = glm::vec4(1.0f, 1.0f, 14.0f, 1.0f);
    cm.triHit(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f), &origin, &direction, &startVec, &endVec, &hit);

    EXPECT_EQ(hit, true);
}
