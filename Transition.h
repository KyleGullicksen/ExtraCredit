//
// Created by stoffel on 11/14/17.
//

#ifndef EXTRACREDIT_TRANSITION_H
#define EXTRACREDIT_TRANSITION_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Transition
{
public:
    int sourceState = 0;
    vector<char> transitionChars;
    vector<int> targetStates;
    Transition(int sourceState, const vector<char>& transitionChars, int targetState);
    Transition(int sourceState, const string & transitionChars, int targetState);

    void setTransitions(string transitionsString);
};

#endif //EXTRACREDIT_TRANSITION_H
