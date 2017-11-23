#include<iostream>
#include<fstream>
#include<vector>
#include "TransitionTable.h"
#include <unordered_set>
#include <unordered_map>
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
        cout << "CurrentLine: " << currentLine << endl;
        whitespaceSplit.clear();
        transition.transitionChars.clear();
        transition.targetStates.clear();
        Utilities::split(currentLine, ' ', whitespaceSplit);

        if(whitespaceSplit.size() < 3)
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

    //Get all the alphas while we have the transitions saved
    for(Transition transition : transitions)
        for(char transitionChar : transition.transitionChars)
            if(transitionChar != 'e')
                alphas.insert(transitionChar);

    //set the maxState
    maxState = -1;
    for(Transition transition : transitions)
    {
        if(transition.sourceState > maxState)
            maxState = transition.sourceState;

        for(int i : transition.targetStates)
            if(i > maxState)
                maxState = i;
    }
    maxState++;
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
            results << targetState;
            results << " ";
        }
        //Done
        results << "\n";
    }

    results.close();
}

vector<int>& matchesTargetCharStar(int currentState, char targetChar, vector<int> &results)
{
    vector<int>* targetTransitions = nfa.getTransitions(currentState, targetChar);

    if(targetTransitions != nullptr) {
        for(int stateNumber : *targetTransitions)
        {
            results.push_back(stateNumber);
            matchesTargetCharStar(stateNumber, EPSILON, results);
        }
    }
    else
        cout << "\tNo transitions for the target: " << targetChar << " on the state: " << currentState << endl;

    return results;
}

void matchesTargetFollowedByEpsilonStar(int currentStateNumber, char targetChar, vector<int> & results)
{
    vector<int> * targetTransitions = nfa.getTransitions(currentStateNumber, targetChar);

    if(targetTransitions != nullptr)
        for(int state : *targetTransitions)
        {
            results.push_back(state);
            matchesTargetCharStar(state, EPSILON, results);
        }
}

void matchesTargetChar(int currentStateNumber, char targetChar, vector<int> & results)
{
    vector<int> * transitions = nfa.getTransitions(currentStateNumber, targetChar);

    if(transitions != nullptr)
        for(int state : *transitions)
            results.push_back(state);
}

int main()
{
    char a;
    buildnfa();
    vector<vector<int>> epsilonTransitions;
    vector<int> currentEpsilonTransitions;

    //Get the epsilon transitions for all of them
    for(int stateNumber = 0; stateNumber < maxState; ++stateNumber)
    {
        currentEpsilonTransitions.clear();
        matchesTargetCharStar(stateNumber, EPSILON, currentEpsilonTransitions);
        epsilonTransitions.push_back(currentEpsilonTransitions);
    }

    //Calculate e*<target transition> e* for all states...
    vector<int> results;
    vector<Transition> transitions;
    Transition transition;

    for(int stateNumber = 0; stateNumber < maxState; stateNumber++)
    {
        for(char currentTransitionChar : alphas)
        {
            for(int epsilonState : epsilonTransitions[stateNumber])
            {
                matchesTargetFollowedByEpsilonStar(epsilonState, currentTransitionChar, results);

                if(results.empty())
                    continue;

                transition.sourceState = stateNumber;
                transition.transitionChars.push_back(currentTransitionChar);
                transition.targetStates.insert(transition.targetStates.end(), results.begin(), results.end());
                transitions.push_back(transition);

                results.clear();
                transition.transitionChars.clear();
                transition.targetStates.clear();
            }
        }
    }

    exportToFile("results.txt", transitions);
}

 
