//
// Created by stoffel on 11/14/17.
//

#include "Utilities.h"

vector<string>& Utilities::split(string& str, char delimiter, vector<string>& results)
{
    string currentComponent = "";

    bool isDelimiter = false;
    for(char currentChar : str)
    {
        if(currentChar == delimiter)
        {
            isDelimiter = true;
            continue;
        }

        if(isDelimiter && !currentComponent.empty())
        {
            results.push_back(currentComponent);
            currentComponent.clear();
        }

        isDelimiter = false;
        currentComponent.push_back(currentChar);
    }

    return results;
}
