#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

// transform NFAe into NFA  (nfa-e.txt into nfa.txt)

vector<int> NFA1[10][5]; // with e's in slot 4
int maxState;

// ----- put utility functions here ----------------------------------------


void buildnfa()
{ 
  int state; int dest; char c; 

  ifstream fin ("nfa-e.txt", ios::in);

  fin >> maxState; // states are 0 through this

  // ** store nfa-e.txt into NFA and display it

  fin.close();
}


int main()
{ char a;

  buildnfa();

  // for each state-char pair of NFA1, compute e*chare* to add to NFA2

  cout << "All the states reachable using only e's" << endl; 
  for (int s = 0; s <= maxState;  s++)
    {
      // from s, find all the states rechable using only e's
      // These are the Eends
    }

  cout << "Computing e* c e* for each state-c pair.. " << endl;
  for (int s = 0; s < maxState; s++)
    for (int ci = 0; ci < 4; ci++)
      { 
        // get direct on ci destinations

        // add on (Eend, ci) destinations 

        // add on all destinations' Eends

        // send the result for (s, ci) pair to output file
      }
   cout << "Sent NFA to nfa.txt .... " << endl;

}

 
