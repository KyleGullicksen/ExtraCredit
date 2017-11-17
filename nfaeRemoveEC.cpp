#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include "TransitionTable.h"
#include "Transition.h"
#include <unordered_set>
#include <unordered_map>
#include "Utilities.h"

using namespace std;

// transform NFAe into NFA  (nfa-e.txt into nfa.txt)

TransitionTable nfa;
int maxState;

// ----- put utility functions here ----------------------------------------


//0 e 1 -1
void buildnfa()
{
    ifstream input("nfa-e.txt");
    vector<string> whitespaceSplit;
    Transition transition;
    char transitionChar = 'e';

    vector<Transition> transitions;

    for(string currentLine; getline(input, currentLine);)
    {
        whitespaceSplit.clear();
        Utilities::split(currentLine, ' ', whitespaceSplit);

        if(whitespaceSplit.size() < 4)
        {
            cout << "Line [" << currentLine << "] was malformed. Skipping." << endl;
            continue;
        }

        transition.sourceState = stoi(whitespaceSplit.at(0));
        transition.transitionChars.push_back(whitespaceSplit.at(1)[0]);

        //Add all the target states
        string terminatingChar = "-1";
        for(int index = 2; index < whitespaceSplit.size() && whitespaceSplit[index] != terminatingChar; ++index)
            transition.targetStates.push_back(stoi(whitespaceSplit.at(index)));

        transitions.push_back(transition);
    }

    nfa.populate(transitions);
}


vector<int> & addTransitionsFromMatchingTargetChar(int currentState, char targetChar, vector<int>& results)
{
    vector<int> * targetTransitions = nfa.getTransitions(currentState, targetChar);

    if(targetTransitions != nullptr)
        for(int stateNumber : *targetTransitions)
        {
            results.push_back(stateNumber);
            addTransitionsFromMatchingTargetChar(stateNumber, EPSILON, results);
        }

    return results;
}

int main()
{
    char a;
    buildnfa();

    unordered_set<char> alphas;
    vector<vector<int>> epsilonTransitions;
    vector<int> currentEpsilonTransitions;
    epsilonTransitions.resize(maxState);
    vector<int> * epsilonTransitonsForCurrentState;

    //TODO: Get all the alphas

    //Get the epsilon transitions for all of them
    for(int stateNumber = 0; stateNumber < maxState; ++stateNumber)
    {
        currentEpsilonTransitions.clear();
        addTransitionsFromMatchingTargetChar(stateNumber, EPSILON, currentEpsilonTransitions);
        epsilonTransitions.push_back(currentEpsilonTransitions);
    }


    //Calculate e*<target transition> e* for all states...

    vector<Transition> nfaWithEpsilonTransitions;
    Transition transition;
    vector<int> * currentEpsilonTransitionState = nullptr;
    vector<int> results;

    for(int stateNumber = 0; stateNumber < maxState; stateNumber++)
    {
        for(char currentTransitionChar : alphas)
        {
            currentEpsilonTransitionState = &epsilonTransitions[stateNumber];

            for(int epsilonState : *currentEpsilonTransitionState)
                addTransitionsFromMatchingTargetChar(epsilonState, currentTransitionChar, results);
        }
    }

    //TODO: Export the results to a text document
}

 
