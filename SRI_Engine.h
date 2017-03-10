//  SRI_Engine.h
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Prototypes and definitions for the SRI_Engine class.

#ifndef __sri_engine__
#define __sri_engine__

#include <vector>
#include <string>
#include <map>
#include <fstream>
using std::vector;
using std::string;
using std::map;

#include <iostream> // For console debugging

#include "QueryParam.h"
#include "Fact.h"
#include "Rule.h"

class SRI_Engine {
    private:
      map<string, vector<Fact>> facts;
      map<string, vector<Rule>> rules;
      bool checkFact(const Fact&, const vector<QueryParam>&, int);
    public:
      void addFact(string, string, vector<string>);
<<<<<<< HEAD
      void addRule(string, string, bool, vector<string>, vector<RFact>);
      vector<Fact*> queryFacts(string, vector<string>);
      vector<Fact*> queryRules(string, vector<string>);
      vector<Fact*> query(string, vector<string>);
      void dump(string filename);
=======
      void addFact(Fact);
      void addRule(string, string, bool, vector<string>, vector<RFact>);
      vector<Fact> queryFacts(string, vector<string>);
      vector<Fact> queryRules(string, vector<string>, string);
      vector<Fact> query(string, vector<string>);
>>>>>>> origin/master
      void print();

};


#endif
