//  Fact.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the Fact struct.

#include "Fact.h"

// Constructor
Fact::Fact(string n, string d, vector<string> v) {
    name = n;
    def = d;
    vals = v;
};