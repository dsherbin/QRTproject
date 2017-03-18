//  Rule.h
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Prototypes and definitions for the Rule struct, and related RFact struct.

#ifndef __rule__
#define __rule__

#include <vector>
#include <string>
using std::vector;
using std::string;

// An RFact is a Fact as defined by a Rule. It has to store a name, and the
// params as defined by the Rule.
struct RFact {
    string name;
    vector<string> params;
    
    RFact(string, vector<string>);
};


// A rule is declared by the user through the input:
//   RULENAME($P1,$P2...):- AND|OR FACT1($P?,$P?...) FACT2($P?, $P?...) ...
struct Rule {
    string def; // Actual string used to define this rule, for processing efficiency.
    vector<string> params; // Params after RULENAME, i.e. [$X, $Y]
    int operation;
    vector<RFact> facts;

    //constants
    static const int AND = 1;
    static const int OR = 2;
    
    Rule(string, int, vector<string>, vector<RFact>);
};

#endif
