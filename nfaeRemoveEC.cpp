#include<iostream>
#include<fstream>
#include<vector>
#include "TransitionTable.h"
#include <unordered_set>
#include "Utilities.h"

using namespace std;

// transform NFAe into NFA  (nfa-e.txt into nfa.txt)

TransitionTable nfa;

unordered_set<char> alphas;

int maxState;

// ----- put utility functions here ----------------------------------------


//0 e 1 -1
void buildnfa()
{
    ifstream input("nfa-e.txt");
    vector<string> whitespaceSplit;
    Transition transition;
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

    //Get all the alphas while we have the transitons saved
    for(Transition transition1 : transitions)
        for(char transitionChar : transition1.transitionChars)
            alphas.insert(transitionChar);

    //set the maxState
    maxState = nfa.getMaxState();
}

vector<int>& addTransitionsFromMatchingTargetChar(int currentState, char targetChar, vector<int>& results)
{
    vector<int>* targetTransitions = nfa.getTransitions(currentState, targetChar);
    if(targetTransitions != nullptr)
        for(int stateNumber : *targetTransitions)
        {
            results.push_back(stateNumber);
            addTransitionsFromMatchingTargetChar(stateNumber, EPSILON, results);
        }
    return results;
}

void exportToFile(const char* filename, vector<Transition>& transitions)
{
    //If we've already exported our results before, delete them
    remove(filename);

    //Create the file
    ofstream results(filename);
    for(Transition transition : transitions)
    {
        results << transition.sourceState;
        results << " ";
        for(char transitionChar : transition.transitionChars)
            results << transitionChar;
        results << " ";
        for(int targetState : transition.targetStates)
        {
            results << " ";
            results << targetState;
            results << " ";
        }

        //Write the EOL marker
        results << -1;

        //Done
    }
}

int main()
{
    char a;
    buildnfa();
    unordered_set<char> alphas;
    vector<vector<int>> epsilonTransitions;
    vector<int> currentEpsilonTransitions;
    epsilonTransitions.resize(maxState);
    vector<int>* epsilonTransitonsForCurrentState;

    //Get the epsilon transitions for all of them
    for(int stateNumber = 0; stateNumber < maxState; ++stateNumber)
    {
        currentEpsilonTransitions.clear();
        addTransitionsFromMatchingTargetChar(stateNumber, EPSILON, currentEpsilonTransitions);
        epsilonTransitions.push_back(currentEpsilonTransitions);
    }


    //Calculate e*<target transition> e* for all states...
    vector<Transition> nfaWithoutEpsilonTransitions;
    Transition transition;
    vector<int>* currentEpsilonTransitionState = nullptr;
    vector<int> results;
    for(int stateNumber = 0; stateNumber < maxState; stateNumber++)
    {
        for(char currentTransitionChar : alphas)
        {
            currentEpsilonTransitionState = &epsilonTransitions[stateNumber];
            for(int epsilonState : *currentEpsilonTransitionState)
                addTransitionsFromMatchingTargetChar(epsilonState, currentTransitionChar, results);

            //Add the transitions
            for(int result : results)
            {
                transition.set(stateNumber, currentTransitionChar, result);
                nfaWithoutEpsilonTransitions.push_back(transition);
            }
        }
    }
    exportToFile("results.txt", nfaWithoutEpsilonTransitions);
}

 
