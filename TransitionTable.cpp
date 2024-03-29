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
    int maxSourceState = INT_MIN;
    for(Transition transition : transitions)
        if(transition.sourceState > maxSourceState)
            maxSourceState = transition.sourceState;

    cout << "maxSourceState: " << maxSourceState << endl;

    maxState = maxSourceState;

    vector<int> destinations;
    vector<vector<int>> emptyTransitions;

    for(int index = 0; index < NUMBER_OF_TRANSITION_CHARS; ++index)
        emptyTransitions.push_back(destinations);

    //Add an empty table entry for items [0, maxState]
    for(int index = 0; index <= maxSourceState; ++index)
        table.push_back(emptyTransitions);

//    //Each transition char vector has a fixed size of 52 (26 for the lower case letters, 26 for the uppers]
//    for(int index = 0; index < maxSourceState; ++index)
//        table[index].resize(NUMBER_OF_TRANSITION_CHARS);

    //Populate the table from the given transitions
    for(Transition transition : transitions)
        for(char transitionChar : transition.transitionChars)
            for(int targetState : transition.targetStates)
            {
                cout << "Requesting: Target State:  " << targetState << ", Source State: " << transition.sourceState << endl;
                table[transition.sourceState][getIndex(transitionChar)].push_back(targetState);
                cout << "Added transition [" << transition.toString() << "] to table"  << endl;
            }
}

int TransitionTable::getIndex(char transitionChar)
{
    return islower(transitionChar) ? transitionChar - 'a' : (transitionChar - 'A') + 26;
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

vector<Transition>& TransitionTable::getAllTransitions(int sourceState, vector<Transition>& results)
{
    if(sourceState >= table.size())
        return results;

    vector<vector<int>> & transitionCharsAndTargetsForSourceState = table.at(sourceState);
    Transition transition;
    char currentTransitonChar;
    bool isLower = true;
    for(int index = 0; index < NUMBER_OF_TRANSITION_CHARS; ++index)
    {
        if(transitionCharsAndTargetsForSourceState[index].empty())
            continue;

        if(index == 26)
            isLower = false;

        if(isLower)
            currentTransitonChar = 'a' + index;
        else
            currentTransitonChar = 'A' + index;

        //Make the transitons
        for(int target : transitionCharsAndTargetsForSourceState[index])
        {
            transition.set(sourceState, currentTransitonChar, target);
            results.push_back(transition);
        }
    }

    return results;
}

int TransitionTable::getMaxState() const
{
    return maxState;
}

TransitionTable::TransitionTable(Transition transition)
{
    vector<Transition> transitions;
    transitions.push_back(transition);

    populate(transitions);
}

