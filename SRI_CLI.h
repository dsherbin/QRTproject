//  SRI_CLI.h
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Prototypes and definitions for the SRI_CLI class.

#ifndef __sri_cli__
#define __sri_cli__

#include <iostream>
#include <string>
#include <vector>
#include "SRI_Engine.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

class SRI_CLI {
    private:
      //const int N_PARAMS = 2; // Number of parameters per command.
      SRI_Engine* engine;
      std::vector<string> split(string,const vector<char>*);
      bool exit;
    public:
      SRI_CLI();
      void start();
      void parse(string);
      ~SRI_CLI();
};

#endif
