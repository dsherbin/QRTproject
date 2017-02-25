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
using std::vector;
using std::string;
using std::map;

#include <iostream> // For console debugging


//TODO: Move to own files.
struct QueryParam {
    string given_name; // Name as provided by the user.
    vector<int> pos;   // Param positions this value occured at.
    bool wildcard;     // Wildcard values start with a $.
    
    string value;     // The current value of this param.
    
    QueryParam(string gn, int p) {
        given_name = gn;
        pos.push_back(p);
        value = "";
        wildcard = (given_name[0] == '$');
        if(!wildcard) value = given_name;
    }
};


struct Fact {
    // Note: might want to add the name here, even though its stored in the engine as the index.
    vector<string> vals;
    Fact(vector<string> v) { vals = v; };
};
//Fact::Fact(vector<string> v) {
//    vals = v;
//}

struct RFact {
    string name;
    vector<string> params;
};

// A rule is declared by the user through the input:
//   RULENAME($P1,$P2...):- AND|OR FACT1($P?,$P?...) FACT2($P?, $P?...) ...
struct Rule {
    string format; // Format of given parameters
    
    vector<string> params; // Params after RULENAME, i.e. [$X, $Y]
    
    // TODO: Make this an enum or something else more intuitive
    bool andtype; // AND or OR
    vector<RFact> facts;
    
    Rule(string f, bool t, vector<string> facts) {
        format = f;
        andtype = t;
        //fact_names = facts;
    };
};

class SRI_Engine {
    private:
      map<string, vector<Fact>> facts;
      map<string, vector<Rule>> rules;
      bool checkFact(const Fact&, const vector<QueryParam>&, int);
    public:
      void addFact(string, vector<string>);
      void addRule(string, string, bool, vector<string>);
      vector<Fact*> queryFacts(string, vector<string>);
      void print();

};


#endif
