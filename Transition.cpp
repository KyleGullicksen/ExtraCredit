//
// Created by stoffel on 11/14/17.
//

#include "Transition.h"


Transition::Transition(int sourceState, const vector<char>& transitionChars, int targetState)
        :sourceState(sourceState), transitionChars(transitionChars)
{
    targetStates.push_back(targetState);
}

Transition::Transition(int sourceState, const string& transitionChars, int targetState) : sourceState(sourceState)
{
    for(char ch : transitionChars)
        this->transitionChars.push_back(ch);

    targetStates.push_back(targetState);
}

void Transition::setTransitions(string transitionsString)
{
    for(char ch : transitionsString)
        transitionChars.push_back(ch);
}

void Transition::set(int sourceState, const vector<char>& transitionChars, int targetState)
{
    this->sourceState = sourceState;
    this->transitionChars = transitionChars;
    this->targetStates.push_back(targetState);
}

void Transition::set(int sourceState, char transitionChar, int targetState)
{
    this->sourceState = sourceState;
    this->transitionChars.push_back(transitionChar);
    this->targetStates.push_back(targetState);
}

int Transition::getTopTargetState()
{
    if(targetStates.size() == 0)
        return -1;

    return targetStates[0];
}

char Transition::getTopTransitionChar()
{
    if(transitionChars.size() == 0)
        return -1;

    return transitionChars[0];
}

void Transition::set(int sourceState, int targetState)
{
    this->sourceState = sourceState;
    this->targetStates.push_back(targetState);
}

void Transition::set(int sourceState, vector<int>& targets)
{
    this->sourceState = sourceState;
    this->targetStates.insert(this->targetStates.end(), targets.begin(), targets.end());
}

string Transition::toString()
{
    string str;

    str.append(std::to_string(sourceState));
    str.append("--");
    str.append(transitionCharsToString());
    str.append("--");
    str.append(destinations());

    return str;
}

string Transition::transitionCharsToString()
{
    string str;

    for(char c : transitionChars)
        str.push_back(c);

    return str;
}

string Transition::destinations()
{
    string str;

    for(int i : targetStates) {
        str.append(std::to_string(i));
        str.push_back(' ');
    }

    return str;
}
