#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>
#include <iostream>
#include <time.h>
using namespace std;

/// \author T ASHBY
/// \version 1.0
/// \date Last Revision 01/08/19 \n

/// \class rule.h
/// \brief a rule object containing a rule char and the possible replacements
/// \todo

class rule
{
public:
    rule();

    /// @brief the char to replace
    string m_ruleString;
    /// @brief list of possible replacement strings
    vector<string> m_replacements;
    /// @brief the probabilities of choosing each replacement
    vector<float> m_probabilities;
    /// @brief does the rule choose between multiple replacements?
    bool m_stochastic = false;

    /// @brief returns a replacment string from 1 or more choices
    /// @return string replacement
    string getReplacement();

private:

};

#endif // RULE_H
