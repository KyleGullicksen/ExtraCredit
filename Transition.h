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
private:
    string transitionCharsToString();
    string destinations();
public:
    int sourceState = 0;
    vector<char> transitionChars;
    vector<int> targetStates;
    Transition() = default;
    Transition(int sourceState, const vector<char>& transitionChars, int targetState);
    Transition(int sourceState, const string & transitionChars, int targetState);

    void setTransitions(string transitionsString);
    virtual void set(int sourceState, vector<int>& targets);
    virtual void set(int sourceState, int targetState);
    virtual void set(int sourceState, char transitionChar, int targetState);
    virtual void set(int sourceState, const vector<char>& transitionChars, int targetState);
    int getTopTargetState();
    char getTopTransitionChar();
    string toString();
};

#endif //EXTRACREDIT_TRANSITION_H
