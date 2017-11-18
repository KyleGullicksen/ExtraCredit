#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define NO_TRANSITION '*'
#define NO_STATE -1
#define EPSILON '!'

struct Transition
{
    int source = 0;
    char acceptedCharacter = 'a';
    vector<int> destinations;
};

vector<vector<Transition>> machines;
int stateIndex = 0;

// ------------- Put utility functions here --------------------

Transition makeTransition(int sourceState, char acceptedChar, int target)
{
    Transition transition;
    transition.source = sourceState;
    transition.acceptedCharacter = acceptedChar;
    transition.destinations.push_back(target);

    return transition;
}

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

        // create transition from M1's end to M2's start
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
        cout << "Enter different number for the second machine";
        cin >> M2;
    }

    vector<Transition> & firstMachine = machines[M1];
    vector<Transition> & secondMachine = machines[M2];

    //Need a new start, and a new end
    Transition start = makeTransition();
}

void processStar()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;

    //get the requested machine
    vector<Transition> & machine = machines[M1];

    //Make a new start state [linking this new start state to the old start state]
    Transition & oldStart = machine[0];
    Transition & oldEnd = machine[machine.size() - 1];

    //Make a new starting node
    Transition start = makeTransition(stateIndex++, EPSILON, oldStart.source);
    start.destinations.push_back(stateIndex);
    oldEnd.destinations.push_back(stateIndex);
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
    vector<char> alphabet;
    Transition start;
    Transition end;
    vector<Transition> currentMachine;

    ifstream fin("re.txt", ios::in);
    while(fin >> c)
    {
        currentMachine.clear();

        // build a transition for arrow label c
        start.source = stateIndex++;
        start.acceptedCharacter = c;
        start.destinations.push_back()

        currentMachine.push_back(start);

        machines.push_back(currentMachine);

        // display the transition
//            0---a---1
//            Initial=0
//            Final=1

        cout << start.source << "--" << start.acceptedCharacter << "--" << start.destinstion << endl;
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
