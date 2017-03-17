//  QueryParam.h
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Prototypes and definitions for the QueryParam struct.

#ifndef __queryparam__
#define __queryparam__

#include <vector>
#include <string>
using std::vector;
using std::string;

struct QueryParam {
    string given_name; // Name as provided by the user.
    vector<int> pos;   // Param positions this value occured at.
    bool wildcard;     // Wildcard values start with a $.
    string value;     // The current value of this param.
    
    QueryParam(string, int);
};

#endif
