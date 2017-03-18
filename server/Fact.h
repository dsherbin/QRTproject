//  Fact.h
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Prototypes and definitions for the Fact struct.

#ifndef __fact__
#define __fact__

#include <vector>
#include <string>

struct Fact {
    std::string def; // The actual string used to define this fact, for processing efficiency. WIP
    std::string name; // Stored for result usage
    std::vector<std::string> vals;
    
    Fact(std::string, std::string, std::vector<std::string>);
};

#endif
