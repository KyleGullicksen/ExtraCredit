#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using namespace std;

#define NO_TRANSITION '*'
#define NO_STATE -1
#define EPSILON '!'

int stateNumberCounter = 0;

struct Transition
{
    int source = 0;
    unordered_set<char> acceptedChars;
    int destination = 0;
};

struct Node
{
    int number = 0;
    unordered_map<int, Transition> transitions;
};

class Machine
{
protected:
    int start = INT_MAX;
    int end = INT_MIN;
    unordered_map<int, Node> nodes;
public:
    void add(Node node)
    {
        //new start?
        if(node.number < start)
            start = node.number;

        //new end?
        if(node.number > end)
            end = node.number;

        if(nodes.find(node.number) == nodes.end())
        {
            nodes[node.number] = node;
            return;
        }

        //This node already exists, so let's merge them together [heavy operation]
        Node & existingNode = nodes.at(node.number);
        nodes[node.number] = merge(existingNode, node); //Overwrite our existing entry
    }

    Node & merge(Node & firstNode, Node & secondNode)
    {
        if(firstNode.number != secondNode.number)
            return firstNode;

        Transition firstTransition;
        Transition secondTransition;

        //Cases: Both share a source and a destination
        //Sol: Merge
        //Case 2: Other source not in this source
        //Add the other's element verbatim
        //Case 3:
        //this source contains something not in the other
        //Nothing to be done


        //Merge matching transitions...
        for(auto firstElement : firstNode.transitions)
        {
            firstTransition = firstElement.second;

            for(auto secondElement : secondNode.transitions)
            {
                secondTransition = secondElement.second;

                if((firstTransition.source == secondTransition.source) && (firstTransition.destination == secondTransition.destination))
                    mergeTransition(firstTransition, secondTransition);
            }
        }

        //Find source nodes not present in the firstNode in the secondNode
        unordered_set<int> firstNodeSources;
        for(auto firstElement : firstNode.transitions)
            firstNodeSources.insert(firstElement.second.source);

        for(auto secondElement : secondNode.transitions)
            if(firstNodeSources.find(secondElement.second.source) == firstNodeSources.end())
                firstNode.transitions[secondElement.second.source] = secondElement.second;

        return firstNode;
    }

    Transition & mergeTransition(Transition & firstTransition, Transition & secondTransition)
    {
        for(char c : secondTransition.acceptedChars)
            firstTransition.acceptedChars.insert(c);
    }

    Node & get(int number)
    {
        return nodes.at(number);
    }

    Node & getStartNode()
    {
        return nodes.at(start);
    }

    Node & getEndNode()
    {
        return nodes.at(end);
    }

    void appendTransition(int nodeNumber, Transition transition)
    {
        if(nodes.find(nodeNumber) == nodes.end())
            return;

        Node& selectedNode = nodes.at(nodeNumber);

        if(selectedNode.transitions.find(transition.destination) == selectedNode.transitions.end())
        {
            selectedNode.transitions[transition.destination] = transition;
            return;
        }

        Transition & currentTransition = selectedNode.transitions[transition.destination];
        for(char c : transition.acceptedChars)
            currentTransition.acceptedChars.insert(c);
    }

    template <typename T>
    void visit(T&& callback)
    {
        Node currentNode;

        for(auto element : nodes)
        {
            currentNode = element.second;
            callback(currentNode);
        }
    }

    void newStart(char transitionToOldStart)
    {
        int newStartNumber = stateNumberCounter++;
        int oldStartNumber = getStartNumber();
        link(newStartNumber, transitionToOldStart, oldStartNumber);
    }

    void newEnd(char transitionToOldEnd)
    {
        int newEndNumber = stateNumberCounter++;
        int oldEndNumber = getEndNumber();
        link(newEndNumber, transitionToOldEnd, oldEndNumber);
    }

    virtual void link(int source, char transitionChar, Machine & machine)
    {
        int oldMachineStart = machine.getStartNumber();
        int oldMachineEnd = machine.getEndNumber();

        machine.visit([&](Node & node) -> void {
            add(node); //Hopefully this is the right add being called here
        });

        link(source, EPSILON, oldMachineStart);
        link(oldMachineEnd, EPSILON, machine.getEndNumber());
    }

    virtual void link(int source, char transitionChar, int destination)
    {
        Transition transition;
        transition.source = source;
        transition.acceptedChars.insert(transitionChar);
        transition.destination = destination;
        link(transition);
    }

    virtual void link(Transition transition)
    {
        if(nodes.find(transition.source) == nodes.end())
        {
            Node start;
            start.number = transition.source;
            add(start);
        }
        if(nodes.find(transition.destination) == nodes.end())
        {
            Node end;
            end.number = transition.destination;
            add(end);
        }

        appendTransition(transition.source, transition);
    }

    Node nextNode()
    {
        Node nextNode;
        nextNode.number = stateNumberCounter++;

        return nextNode;
    }

    int getStartNumber()
    {
        return start;
    }

    int getEndNumber()
    {
        return end;
    }
};

vector<Machine> machines;

string setToString(unordered_set<char> set)
{
    string str;

    for(char c : set)
        str.push_back(c);
    return str;
}

int nextStateNumber()
{
    int next = stateNumberCounter;
    ++stateNumberCounter;

    return next;
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

        Machine & machine = machines[M1];
        machine.link(nextStateNumber(), EPSILON, machine);
    }
    else
    {
        Machine & firstMachine = machines.at(M1);
        Machine & secondMachine = machines.at(M1);
        firstMachine.link(nextStateNumber(), EPSILON, secondMachine);
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

    Machine & firstMachine = machines[M1];
    Machine & secondMachine = machines[M2];
    int endOfSecondMachine = secondMachine.getEndNumber();

    firstMachine.newStart(EPSILON);
    firstMachine.newEnd(EPSILON);

    firstMachine.link(firstMachine.getStartNumber(), EPSILON, secondMachine);
    firstMachine.link(endOfSecondMachine, EPSILON, firstMachine.getEndNumber());
}

void processStar()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;

    Machine & machine = machines.at(M1);

    machine.newEnd(EPSILON);
    machine.newEnd(EPSILON);

    machine.link(machine.getStartNumber(), EPSILON, machine.getEndNumber());
    machine.link(machine.getEndNumber(), EPSILON, machine.getStartNumber());
}

void processPlus()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;
    Machine& machine = machines.at(M1);
    machine.newStart(EPSILON);
    machine.newEnd(EPSILON);
    machine.link(machine.getEndNumber(), EPSILON, machine.getStartNumber());
}

void exportToFile()
{
    Node currentNode;
    ofstream output("reToNFAEOutput.txt", ios::out);
    Transition transition;

    for(Machine machine : machines)
    {
        //Export each machine to file...

        //Format: <source> <transition chars> <destination> newline
        machine.visit([&](Node node) ->void {
            for(auto transitionElement : node.transitions)
            {
                transition = transitionElement.second;

                output << node.number;
                output << " ";
                output << setToString(transition.acceptedChars);
                output << " ";
                output << transition.destination;
                output << "\n";
            }
        });
    }

    output.close();
}

int main()
{
    cout << "This program will transform regular expressions into NFA-e" << endl;
    cout << "But it is done interactively by asking you to apply operators" << endl;
    cout << "to component machines." << endl;


    ifstream input("re.txt", ios::in);
    char c;
    int start = 0;
    int end = 0;

    //parse the file to get our initial machines
    while(input >> c)
    {
        Machine machine;

        start = nextStateNumber();
        end = nextStateNumber();

        machine.link(start, c, end);

        //Display the transiton
        cout << start << "--" << c << "--" << end << endl;
        cout << "Initial=" << start << endl;
        cout << "Final=" << end << endl;
        cout << "" << endl;
    }

    input.close();

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

    exportToFile();
}
