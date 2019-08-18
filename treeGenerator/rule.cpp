#include "rule.h"

rule::rule()
{

}

string rule::getReplacement()
{
    if(!m_stochastic)
    {
        return m_replacements.at(0);
    }else
    {
        //return random replacement here
        double r = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
        //std::cout<<r<<std::endl;

        for(unsigned int i=0; i<m_probabilities.size(); i++)
        {
            double leftSide; //the less than part
            double rightSide; //the more than part

            for(unsigned int j = 0; j < i + 1; j++)
            {
                 leftSide += m_probabilities.at(j);
            }
            for(unsigned int j = 1; j < i + 2; j++)
            {
                if(j<m_probabilities.size())
                {
                    rightSide += m_probabilities.at(j);
                }
            }

            if(r > leftSide && r < rightSide)
            {
                return m_replacements.at(i);
            }
        }

        return m_replacements.at(0);
    }
}
