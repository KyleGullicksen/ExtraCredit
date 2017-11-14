//
// Created by stoffel on 11/14/17.
//

#include "TransitionTable.h"

vector<int>* TransitionTable::getTransitions(int sourceState, char transitionChar)
{
    if(sourceState >= table.size())
        return nullptr;

    return &(table[sourceState][getIndex(transitionChar)]);
}

TransitionTable::TransitionTable(vector<Transition>& transitions)
{
    populate(transitions);
}

void TransitionTable::populate(vector<Transition>& transitions)
{
    if(transitions.empty())
        return;

    //Find the max source state number
    int maxSourceState = -1;
    for(Transition transition : transitions)
        if(transition.sourceState > maxSourceState)
            maxSourceState = transition.sourceState;

    //Populate the source state #
    table.resize(maxSourceState);
    maxState = maxSourceState;

    //Populate the table from the given transitions
    for(Transition transition : transitions)
        for(char transitionChar : transition.transitionChars)
            for(int targetState : transition.targetStates)
                table[transition.sourceState][getIndex(transitionChar)].push_back(targetState);
}

int TransitionTable::getIndex(char transitionChar)
{
    return transitionChar - 'a';
}

int TransitionTable::size()
{
    return table.size();
}

vector<int> TransitionTable::getSourceStates()
{
    //TODO: How to I check if one of the indicies in my table is empty?

    vector<int> sourceStates;

    for(int index = 0; index < maxState; ++index)
    {
        sourceStates.push_back(index);
    }

    return sourceStates;
}

int TransitionTable::getStartState()
{
    if(table.empty())
        return -1;

    return 0;
}