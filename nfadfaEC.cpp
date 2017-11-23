#include<iostream>
#include<fstream>
#include<vector>
#include <queue>
#include <unordered_set>
#include "Transition.h"
#include "TransitionTable.h"
#include "Utilities.h"

using namespace std;
using Utilities::split;

#define LINE_END_MARKER = -1

TransitionTable dfa;
TransitionTable nfa;
unordered_set<char> alphas;
int maxState;  // the highest state indicated in nfa.txt first line

// ----- utility functions ----------------------------------------
string makeCompositeState(vector<int> & states)
{
    string state = "";
    bool isFirst = true;

    for(int stateNum : states)
    {
        if(!isFirst)
            state.push_back(' ');

        isFirst = false;
        state.append(std::to_string(stateNum));
    }

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

    cout << "Processing each line" << endl;

    for(string currentLine; getline(fin, currentLine);)
    {
        components.clear();
        transition.transitionChars.clear();
        transition.targetStates.clear();

        split(currentLine, ' ', components);

        cout << "Found these components" << endl;
        for(string comp : components)
            cout << "Comp: " << comp << endl;
        cout << "Done" << endl;

        if(components.size() != 4)
        {
            cout << "The input line [" << currentLine << "] was malformed. Ignoring this line." << endl;
            continue;
        }

        transition.sourceState = stoi(components[0]);
        transition.setTransitions(components[1]);

        //Add each target state
        for(int index = 2; index < components.size() && components[index] != "-1"; ++index)
        {
            currentState = stoi(components[index]);
            transition.targetStates.push_back(currentState);
        }

        cout << "Transition: " << transition.toString() << endl;
        transitions.push_back(transition);
    }

    cout << "Finished processing each line" << endl;

    //Populate the transitions table from our transitions list
    nfa.populate(transitions);

    //Populate the alphabet
    alphabet(transitions);

    fin.close();
}

vector<int> & reduce(vector<int> & transitions, vector<int> & results)
{
    unordered_set<int> uniqueTransitions;

    for(int state : transitions)
    {
        if(uniqueTransitions.find(state) != uniqueTransitions.end())
            continue;

        uniqueTransitions.insert(state);
        results.push_back(state);
    }

    return results;
}

// Main Driver
int main()
{
    cout << "Given your NFA in nfa.txt, this will produce the DFA in dfa.txt." << endl;
    cout << "State numbers must be 0 ... 9" << endl;
    cout << "Arrow labels can be anything from a .. e" << endl << endl;
    buildnfa();

    queue<vector<int>> newCompositeStates;
    vector<int> * nfaTransitions;
    vector<int> currentCompositeState;
    vector<int> transitions;
    vector<int> newCompositeState;
    unordered_set<string> seenStates;
    string compositeStateStr = "";
    Transition newDFATransition;
    vector<Transition> dfaTransitions;

    //Add the starting state
    vector<int> startingState;
    startingState.push_back(nfa.getStartStateNumber());
    newCompositeStates.push(startingState);

    while(!newCompositeStates.empty())
    {
        currentCompositeState = newCompositeStates.front();
        newCompositeStates.pop();

        cout << "Processing: [" << makeCompositeState(currentCompositeState) << "]" << endl;

        for(char alpha : alphas)
        {
            transitions.clear();
            newCompositeState.clear();

            for(int currentState : currentCompositeState)
            {
                nfaTransitions = nfa.getTransitions(currentState, alpha);

                if(nfaTransitions == nullptr)
                    continue;

                transitions.insert(transitions.end(), nfaTransitions->begin(), nfaTransitions->end());
            }

            reduce(transitions, newCompositeState);

            compositeStateStr = makeCompositeState(newCompositeState);

            if(compositeStateStr.empty() || (seenStates.find(compositeStateStr) != seenStates.end()))
                continue;

            newCompositeStates.push(newCompositeState);
            cout << "Pushing: " << compositeStateStr << endl;

            seenStates.insert(compositeStateStr);

            if(currentCompositeState.size() == 1)
                newDFATransition.sourceState = currentCompositeState[0];
            newDFATransition.sourceStates.insert(newDFATransition.sourceStates.end(), currentCompositeState.begin(), currentCompositeState.end());

            newDFATransition.transitionChars.push_back(alpha);
            newDFATransition.targetStates.insert(newDFATransition.targetStates.end(), newCompositeState.begin(), newCompositeState.end());

            dfaTransitions.push_back(newDFATransition);

            //clear the new DFA transition
            newDFATransition.sourceState = 0;
            newDFATransition.sourceStates.clear();
            newDFATransition.transitionChars.clear();
            newDFATransition.targetStates.clear();
        }
    }

    dfa.populate(dfaTransitions);

    remove("dfa.txt");
    ofstream fout("dfa.txt", ios::out);

    //Export the DFA to file
    //From <01> on a goes to <01>
    for(Transition dfaTrans : dfaTransitions)
    {
        fout << "From <";
        for(int i : dfaTrans.sourceStates)
            fout << i;
        fout << "> ";

        fout << "on ";
        for(char c : dfaTrans.transitionChars)
            fout << c;
        fout << " goes to <";
        for(int i : dfaTrans.targetStates)
            fout << i;
        fout << ">\n";
    }

    fout << "Any state with the original final state number is final" << endl;
    fout.close();
}

 
