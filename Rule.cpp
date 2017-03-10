//  Rule.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the Rule struct, and related RFact struct.

#include "Rule.h"

// Constructor
RFact::RFact(string n, vector<string> p) {
    name = n;
    params = p;
}

// Constructor
Rule::Rule(string d, bool t, vector<string> p, vector<RFact> f) {
    def = d;
    andtype = t;
    params = p;
    facts = f;
};