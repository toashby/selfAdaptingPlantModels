#include "collisionmanager.h"

collisionManager::collisionManager()
{

}

bool collisionManager::triHit(glm::vec3 _t0, glm::vec3 _t1, glm::vec3 _t2, glm::vec3 *_origin, glm::vec3 *_direction, glm::vec4 *_startVec, glm::vec4 *_endVec, bool *_hit)
{
    //miller trumbore algorithm
    const float EPSILON = 0.0000001f;
        //vertex 1 of the triangle
        glm::vec3 vertex0 = _t0;
        //vertex 2 of the triangle
        glm::vec3 vertex1 = _t1;
        //vertex 3 of the triangle
        glm::vec3 vertex2 = _t2;
        glm::vec3 edge1, edge2, h, s, q;
        float a,f,u,v;
        //calculate any 2 edge vectors
        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;
        //cross ray direction with any edge
        h = glm::cross(*_direction, edge2);
        //dot with other edge
        a = glm::dot(edge1, h);
        //if close to 0 then we a parallel
        if (a > -EPSILON && a < EPSILON)
        {
            return false;    //This ray is parallel to this triangle.
        }
        f = 1.0f/a;
        s = *_origin - vertex0;
        u = f * glm::dot(s, h);
        if (u < 0.0f || u > 1.0f)
        {
            return false;
        }
        q = glm::cross(s, edge1);
        v = f * glm::dot(*_direction, q);
        if (v < 0.0f || u + v > 1.0f)
        {
            return false;
        }
        //the ray collides
        //compute t to find out where the intersection point is on the line.
        float t = f * glm::dot(edge2, q);
        if (t > EPSILON) // ray intersection
        {
            //glm::vec3 outIntersectionPoint = origin + direction * t; //TODO Check if this is less than length of branch
            if(t < glm::distance(glm::vec3(*_startVec), glm::vec3(*_endVec))) //check until end of line length

            {
                //std::cout<<"Branch has collided!!!"<<std::endl;
                *_hit = true;
                return true;
            }
        }
        //else// This means that there is a line intersection but not a ray intersection.
        return false;
}
