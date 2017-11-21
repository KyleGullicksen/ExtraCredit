#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <climits>

using namespace std;

vector<Machine> machines;
int stateNumberCounter = 0;

#define NO_TRANSITION '*'
#define NO_STATE -1
#define EPSILON '!'

struct Transition
{
    int source;
    unordered_set<char> acceptedChars;
    int destination;
};

struct Node
{
    int number;
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

        nodes[node.number] = node;
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

    virtual void link(int source, char transitionChar, int destination)
    {
        Transition transition;
        transition.source = source;
        transition.acceptedChars.push_back(transitionChar);
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
        }transition.acceptedCharacters.push_back(transitionChar);
        if(nodes.find(transition.destination) == nodes.end())
        {
            Node end;
            end.number = transition.destination;
            add(end);
        }

        Node & node = nodes.at(transition.source);
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
}

void processStar()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;

}

void processPlus()
{
    int M1;
    cout << "Enter number of the machine:";
    cin >> M1;

    Machine & machine = machines.at(M1);

    int oldStart = machine.getStartNumber();
    int oldEnd = machine.getEndNumber();

    int newStart = nextStateNumber();
    int newEnd = nextStateNumber();

    machine.link(newStart, EPSILON, oldStart);
    machine.link(oldEnd, EPSILON, newEnd);
    machine.link(newEnd, EPSILON, newStart);
}

void exportToFile()
{
    Node currentNode;
    ofstream output("reToNFAEOutput.txt", ios::out);

    for(Machine machine : machines)
    {
        //Export each machine to file...

        //Format: <source> <transition chars> <destination> newline
        machine.visit([](Node node) ->void {
            for(Transition transition : node.transitions)
            {
                output << node.number;
                output << " ";
                output << transition.acceptedCharacters;
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
