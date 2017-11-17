//
// Created by stoffel on 11/14/17.
//

#ifndef EXTRACREDIT_TRANSITIONTABLE_H
#define EXTRACREDIT_TRANSITIONTABLE_H

#define EPSILON 'e'
#define NO_TRANSITION_CHAR '*'
#define NO_TRANSITION -2
#define NUMBER_OF_TRANSITION_CHARS 52

#include <vector>
#include <string>
#include <fstream>
#include "Transition.h"

using std::vector;
using std::string;
using std::ifstream;

class TransitionTable
{
protected:
    vector<vector<vector<int>>> table;
    int maxState = 0;
    int getIndex(char transitionChar);
public:
    TransitionTable() = default;
    explicit TransitionTable(vector<Transition> & transitions);
    vector<int> * getTransitions(int sourceState, char transitionChar);
    vector<Transition> & getAllTransitions(int sourceState, vector<Transition> & results);
    int size();
    void populate(vector<Transition> & transitions);
    vector<int> getSourceStates();
    int getStartStateNumber();
};

#endif //EXTRACREDIT_TRANSITIONTABLE_H
