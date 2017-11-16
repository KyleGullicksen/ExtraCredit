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
        add(transition);
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

int TransitionTable::getStartStateNumber()
{
    if(table.empty())
        return -1;

    return 0;
}

void TransitionTable::add(Transition& transition)
{
    if(transition.sourceState < table.size())
    {
        table.resize(transition.sourceState + 1);

        //Init the new entires in the table

        for(int index = transition.sourceState - table.size(); index < (transition.sourceState + 2); ++index)
        {
            //Do stuff
        }
    }


    //Add the transiton to the table
    for(char transitionChar : transition.transitionChars)
        for(int targetState : transition.targetStates)
            table[transition.sourceState][getIndex(transitionChar)].push_back(targetState);
}

TransitionTable::TransitionTable(int sourceState, char transitionChar, int endState)
{
    Transition transition;
    transition.set(sourceState, transitionChar, endState);
    add(transition);
}

TransitionTable::TransitionTable(Transition transition)
{
    add(transition);
}

Transition TransitionTable::getLastState()
{
    vector<vector<int>> transitionCharsAndTargets = table[table.size() - 1];

    Transition lastTransition;
    lastTransition.set(table.size() - 1, transitionCharsAndTargets)
}
