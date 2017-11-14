#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <queue>
#include <unordered_set>
#include "Transition.h"
#include "TransitionTable.h"
#include "Utilities.h"

using namespace std;
using Utilities::split;

#define LINE_END_MARKER = -1

// transform NFA into DFA  (nfa.txt into dfa.txt)
// 0 a 0 1 -1 => <0> a <01>
// States allowed are 0 through 9
// Arrows allowed are a through e

ofstream fout("dfa.txt", ios::out);

//vector<int> NFA[10][5];
TransitionTable nfa;
unordered_set<char> alphas;
int maxState;  // the highest state indicated in nfa.txt first line
vector<string> DFA;

// ----- utility functions ----------------------------------------
string makeCompositeState(vector<int> & states)
{
    string state = "";

    for(int stateNum : states)
        state.append(std::to_string(stateNum));

    return state;
}

void alphabet(vector<Transition> & transitions)
{
    for(Transition transition : transitions)
        for(char ch : transition.transitionChars)
            alphas.insert(ch);

}

// ------------------ end of utility ------------------------------


// Reads nfa.txt to build the NFA data structure

//Format: 0 a 0 1 -1

void buildnfa()
{
    ifstream fin("nfa.txt", ios::in);

    //Build the transitions list
    vector<Transition> transitions;
    Transition transition;
    vector<string> components;
    int currentState = 0;

    for(string currentLine; getline(fin, currentLine);)
    {
        components = split(currentLine, ' ');

        if(components.size() != 4)
        {
            cout << "The input line [" << currentLine << "] was malformed. Ignoring this line." << endl;
            continue;
        }

        transition.sourceState = stoi(components[0]);
        transition.setTransitions(components[1]);

        //Add each target state
        for(int index = 2; index < components.size(); ++index)
        {
            currentState = stoi(components[index]);

            if(currentState == LINE_END_MARKER)
                break;

            transition.targetStates.push_back(currentState);
        }
    }

    //Populate the transitions table from our transitions list
    nfa.populate(transitions);

    //Populate the alphabet
    alphabet(transitions);

    fin.close();
}

// adds state to DFA if it is new
void addNewState(string state)
{
    //NOPE NOT USING THIS
}

// Main Driver
int main()
{
    cout << "Given your NFA in nfa.txt, this will produce the DFA in dfa.txt." << endl;
    cout << "State numbers must be 0 ... 9" << endl;
    cout << "Arrow labels can be anything from a .. e" << endl << endl;
    buildnfa();

//    // add new states to DFA
//    addNewState("0");  // start with state 0

    //Starting at our start state
    queue<vector<int>> stateQueue;
    vector<int> startingState;
    startingState.push_back(nfa.getStartState());
    stateQueue.push(startingState);
    unordered_set<string> seenStates;
    vector<int> currentState;

    while(!stateQueue.empty())
    {
        currentState = stateQueue.front();
        stateQueue.pop();

        for()
    }

    while(x < DFA.size()) // for each DFA state
    {
        // display current DFA state S
        // For each arrow
        // go through each component state of S
        // grab all destinations from NFA
        // append the destinations to a state name DS
        //If a transition on the arrow found, display it and send to dfa.txt
        // and DS is added to DFA if new
    }
    fout << "Any state with the original final state number is final" << endl;
    fout.close();
}

 
