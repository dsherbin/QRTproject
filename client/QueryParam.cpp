//  QueryParam.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the QueryParam struct.

#include "QueryParam.h"

// Constructor
QueryParam::QueryParam(string gn, int p) {
    given_name = gn;
    pos.push_back(p);
    value = "";
    wildcard = (given_name[0] == '$');
    if(!wildcard) value = given_name;
}