#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using namespace std;

#define NO_TRANSITION '*'
#define NO_STATE -1
#define EPSILON 'e'

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
private:
    void print(Node * node, unordered_set<int> & seenNodes)
    {
        if(node == nullptr)
            return;

        bool seen = seenNodes.find(node->number) != seenNodes.end();

        if(node->transitions.empty() && !seen)
        {
            cout << node->number << " has no transitions" << endl;
            seenNodes.insert(node->number);
            return;
        }
        if(seen)
            return;

        seenNodes.insert(node->number);

        //Print all of the transitions for the current node
        Transition transition;
        for(auto transitionElement : node->transitions)
        {
            transition = transitionElement.second;

            for(char c : transition.acceptedChars)
            {
                cout << node->number << "--" << c << "--" << transition.destination << endl;
            }
        }

        //Print all of the linked nodes
        for(auto transitionElement : node->transitions)
        {
            transition = transitionElement.second;
            Node & nextNode = get(transition.destination);
            print(&nextNode, seenNodes);
        }

    }

protected:
    int smallestNodeNumber = INT_MAX;
    int largesstNodeNumber = INT_MIN;
    int start = 0; //The start and the end nodes imply intent; the program cannot guess which nodes should be the start and the end nodes
    int end = 0;

    unordered_map<int, Node> nodes;
public:
    void add(Node & node)
    {
        //new start?
        if(node.number < smallestNodeNumber)
            smallestNodeNumber = node.number;

        //new end?
        if(node.number > largesstNodeNumber)
            largesstNodeNumber = node.number;

        if(nodes.find(node.number) == nodes.end())
        {
            cout << "Node number " << node.number << " added for the first time" << endl;
            nodes[node.number] = node;
            return;
        }

        cout << "Node " << node.number << " was already there -- merging" << endl;

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
        return nodes.at(smallestNodeNumber);
    }

    Node & getEndNode()
    {
        return nodes.at(largesstNodeNumber);
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

        cout << "newStartNumber: " << newStartNumber << endl;
        cout << "oldStartNumber: " << oldStartNumber << endl;

        link(newStartNumber, transitionToOldStart, oldStartNumber);
        start = newStartNumber; //Update the start index
    }

    void newEnd(char transitionToOldEnd)
    {
        int newEndNumber = stateNumberCounter++;
        int oldEndNumber = getEndNumber();

        cout << "newEndNumber: " << newEndNumber << endl;
        cout << "oldEndNumber: " << oldEndNumber << endl;

        link(oldEndNumber, EPSILON, newEndNumber);
        end = newEndNumber; //Update the end index
    }

    virtual void link(int source, char transitionChar, Machine & machine, int destination, char destinationTransitionChar)
    {
        int machineStartIndex = machine.getStartNumber();
        int machineEndIndex = machine.getEndNumber();

        machine.visit([&](Node & node) -> void {
            add(node);
        });

        link(source, transitionChar, machineStartIndex);
        link(machineEndIndex, transitionChar, destination);
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
            cout << "Making the source node: " << transition.source << endl;
            Node start;
            start.number = transition.source;
            add(start);
        }
        if(nodes.find(transition.destination) == nodes.end())
        {
            cout << "Making the destination node: " << transition.destination << endl;
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

    void setStart(int start)
    {
        Machine::start = start;
    }

    void setEnd(int end)
    {
        Machine::end = end;
    }

    void print()
    {
        unordered_set<int> seenNodes;
        print(&(get(getStartNumber())), seenNodes);
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
        int newEnd = nextStateNumber();
        machine.link(machine.getEndNumber(), EPSILON, machine, newEnd, EPSILON);
        machine.setEnd(newEnd);
        machine.print();
    }
    else
    {
        Machine & firstMachine = machines.at(M1);
        Machine & secondMachine = machines.at(M1);

        int newEnd = nextStateNumber();

        firstMachine.link(firstMachine.getEndNumber(), EPSILON, secondMachine, newEnd, EPSILON);
        firstMachine.setEnd(newEnd);

        firstMachine.print();
    }
}


//Good
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

    firstMachine.newStart(EPSILON);
    firstMachine.newEnd(EPSILON);

    firstMachine.link(firstMachine.getStartNumber(), EPSILON, secondMachine, firstMachine.getEndNumber(), EPSILON);
    firstMachine.print();
}

//Good
void processStar()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;

    Machine & machine = machines.at(M1);

    machine.newStart(EPSILON);
    machine.newEnd(EPSILON);

    cout << "New Start: " << machine.getStartNumber() << endl;
    cout << "New end: " << machine.getEndNumber() << endl;

    machine.link(machine.getStartNumber(), EPSILON, machine.getEndNumber());
    machine.link(machine.getEndNumber(), EPSILON, machine.getStartNumber());

    machine.print();
}

//Good
void processPlus()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;
    Machine& machine = machines.at(M1);
    machine.newStart(EPSILON);
    machine.newEnd(EPSILON);
    machine.link(machine.getEndNumber(), EPSILON, machine.getStartNumber());

    machine.print();
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
        machine.setStart(start);
        machine.setEnd(end);

        //Display the transiton
        cout << start << "--" << c << "--" << end << endl;
        cout << "Initial=" << start << endl;
        cout << "Final=" << end << endl;
        cout << "" << endl;

        machines.push_back(machine);
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
