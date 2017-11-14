//
// Created by stoffel on 11/14/17.
//

#ifndef EXTRACREDIT_TRANSITIONTABLE_H
#define EXTRACREDIT_TRANSITIONTABLE_H

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
    int size();
    void populate(vector<Transition> & transitions);
    vector<int> getSourceStates();
    int getStartState();
};

#endif //EXTRACREDIT_TRANSITIONTABLE_H