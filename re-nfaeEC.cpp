#include <iostream>
#include <fstream>
#include <vector>
#include "TransitionTable.h"
#include "Transition.h"

using namespace std;

int stateNumber = 0;

vector<TransitionTable> machines;

// ------------- Put utility functions here --------------------


void processConcat()
{
    int M1, M2;
    cout << "Enter number of the first machine:";
    cin >> M1;
    cout << "Enter number of the second machine:";
    cin >> M2;
    if(M1 == M2)
    {
        cout << "copying the machine first ...." << endl;
        // copy the machine and display it }

        // create trasition from M1's end to M2's start
        // add the new machine to NFA
    }
}

void processOr()
{
    int M1, M2;
    cout << "Enter number of the first machine:";
    cin >> M1;
    cout << "Enter number of the second machine:";
    cin >> M2;
    if(M1 == M2)
    { // must be a different one from M1
        cout << "Enter differnt number for the second machine";
        cin >> M2;
    }

    // create new start
    // create new final
    // For the new machine in NFA:
    // add transition from new start to M1 start
    // add transition from new start to M2 start
    // add M1 and M2
    // add transition from M1 end to new final
    // add transition from M2 end to new final

}

void processStar()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;

    //get the requested machine
    TransitionTable & machine = machines[M1];

    //Make a new start state [linking this new start state to the old start state]
    Transition start;
    start.set(++stateNumber, machine.getStartStateNumber());


    //Create a new final state
    Transition final;
    final.set(machine.get)

    // create new start
    // create new final
    // For the new machine in NFA:
    // add transition from new start to M1 start
    // add M1
    // add transition (back) from M1 end to M1 start
    // add transition from M1 end to new final
    // add transition (skip) from new to new final

}

void processPlus()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;



    // create new start
    // create new final
    // For the new machine in NFA:
    // add transition from new start to M1 start
    // add M1
    // add transition (back) from M1 end to M1 start
    // add transition from M1 end to new final

}

int main()
{
    cout << "This program will transform regular expressions into NFA-e" << endl;
    cout << "But it is done interactively by asking you to apply operators" << endl;
    cout << "to component machines." << endl;

    // build a transition for arrow label c
    // display the transition
    // add it to NFA
    char c;
    Transition s;
    vector<char> alphabet;
    ifstream fin("re.txt", ios::in);
    while(fin >> c)
    {
        // build a transition for arrow label c
        s.set(++stateNumber, c, stateNumber++);

        // display the transition
//            0---a---1
//            Initial=0
//            Final=1
        cout << s.sourceState << "--" << s.getTopTransitionChar() << "--" << s.getTopTargetState() << endl;
        machines.emplace_back(TransitionTable(s));
    }
    fin.close();
    char A;
    while(A != 'n')
    {
        cout << "---- MENU -----" << endl;
        cout << "* for a machine" << endl;
        cout << "+ for a machine" << endl;
        cout << "| for two machines" << endl;
        cout << "- for appending two machines" << endl;
        cout << "n to quit" << endl;
        cin >> A;
        if(A == '*') processStar();
        else if(A == '+') processPlus();
        else if(A == '|') processOr();
        else if(A == '-') processConcat();
    }
    cout << "Outputting all machines to nfa-e.txt... " << endl;
    ofstream fout("nfa-e.txt", ios::out);
    // send all NFA contents to the output file
}
