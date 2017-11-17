#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include "TransitionTable.h"
#include "Transition.h"
#include <unordered_set>
#include "Utilities.h"

using namespace std;

// transform NFAe into NFA  (nfa-e.txt into nfa.txt)

TransitionTable nfa;
int maxState;
unordered_set<char> alphas;

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

/*
 * If c is the goal
        Exit
    Else
        Mark c "Visit In Progress"
        Foreach neighbor n of c
            If n "Unvisited"
                Depth-First-Search( n )
        Mark c "Visited"
 */


vector<int> & addAllEpsilonAndTargetTransitions(int currentStateNumber, char targetTransitionChar, vector<int> & results)
{
    //Add the current state
    results.push_back(currentStateNumber);

    //get the target transitions
    vector<int> * targetTransitions = nfa.getTransitions(currentStateNumber, targetTransitionChar);

    if(targetTransitions != nullptr)
        for(int stateNumber : *targetTransitions)
            addAllEpsilonAndTargetTransitions(stateNumber, targetTransitionChar, results);

    //Get all epsilon transitions
    vector<int> * epsilonTransitions = nfa.getTransitions(currentStateNumber, EPSILON);

    if(epsilonTransitions != nullptr)
        for(int stateNumber: * epsilonTransitions)
            addAllEpsilonAndTargetTransitions(stateNumber, targetTransitionChar, results);
}

vector<int> & removeEpsilons(int currentStateNumber, char targetTransitionChar, vector<int> & results)
{
    vector<int> * targetTransitions = nfa.getTransitions(currentStateNumber, targetTransitionChar);

    if(targetTransitions != nullptr)
        for(int stateNumber : *targetTransitions)
            addAllEpsilonAndTargetTransitions(currentStateNumber, targetTransitionChar, results);

    //Get all the epsilon transitions for the current state
    vector<int> * epsilonTransitions = nfa.getTransitions(currentStateNumber, EPSILON);

    if(epsilonTransitions != nullptr)
        for(int stateNumber : *epsilonTransitions)
            removeEpsilons(stateNumber, targetTransitionChar, results);

}

int main()
{
    char a;
    buildnfa();

    // for each state-char pair of NFA1, compute e*chare* to add to NFA2
    vector<int> results;
    vector<Transition> nfaWithoutEpsilonTransitions;
    Transition transition;

    for(int stateNumber = 0; stateNumber < maxState; ++stateNumber)
    {
        results.clear();

        for(char alpha : alphas)
            removeEpsilons(stateNumber, alpha, results);

        //Make transitions
        for(int result : results)
        {
            transition.set(stateNumber, result);
            nfaWithoutEpsilonTransitions.push_back(transition);
        }
    }

    TransitionTable nfaWithoutEpsilon(nfaWithoutEpsilonTransitions);


    cout << "All the states reachable using only e's" << endl;
    for(int s = 0; s <= maxState; s++)
    {
        // from s, find all the states rechable using only e's
        // These are the Eends
    }
    cout << "Computing e* c e* for each state-c pair.. " << endl;
    for(int s = 0; s < maxState; s++)
        for(int ci = 0; ci < 4; ci++)
        {
            // get direct on ci destinations

            // add on (Eend, ci) destinations

            // add on all destinations' Eends

            // send the result for (s, ci) pair to output file
        }
    cout << "Sent NFA to nfa.txt .... " << endl;
}

 
