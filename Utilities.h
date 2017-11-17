//
// Created by stoffel on 11/14/17.
//

#ifndef EXTRACREDIT_UTILITIES_H
#define EXTRACREDIT_UTILITIES_H

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace Utilities {
    vector<string>& split(string& str, char delimiter, vector<string>& results);
};

#endif //EXTRACREDIT_UTILITIES_H
