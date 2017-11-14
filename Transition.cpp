//
// Created by stoffel on 11/14/17.
//

#include "Transition.h"


Transition::Transition(int sourceState, const vector<char>& transitionChars, int targetState)
        :sourceState(sourceState), transitionChars(transitionChars), targetState(targetState)
{ }

Transition::Transition(int sourceState, const string& transitionChars, int targetState) : sourceState(sourceState), targetState(targetState)
{
    for(char ch : transitionChars)
        this->transitionChars.push_back(ch);
}

void Transition::setTransitions(string transitionsString)
{
    for(char ch : transitionsString)
        transitionChars.push_back(ch);
}
