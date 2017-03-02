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


//TODO: Move structs to own .h/.cpp files, split constructors.

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
    string def; // The actual string used to define this fact, for processing efficiency. WIP
    string name; // Stored for result usage
    vector<string> vals;
    Fact(string n, string d, vector<string> v) {
        name = n;
        def = d;
        vals = v;
    };
};


// An RFact is a Fact as defined by a Rule. It has to store a name, and the
// params as defined by the Rule.
struct RFact {
    string name;
    vector<string> params;
    RFact(string n, vector<string> p) {
        name = n;
        params = p;
    }
};


// A rule is declared by the user through the input:
//   RULENAME($P1,$P2...):- AND|OR FACT1($P?,$P?...) FACT2($P?, $P?...) ...
struct Rule {
    string def; // Actual string used to define this rule, for processing efficiency.
    
    vector<string> params; // Params after RULENAME, i.e. [$X, $Y]
    
    // TODO: Make this an enum or something else more intuitive?
    bool andtype; // AND or OR
    vector<RFact> facts;
    
    Rule(string d, bool t, vector<string> p, vector<RFact> f) {
        def = d;
        andtype = t;
        params = p;
        facts = f;
    };
    /*
    Rule(string n, bool t, vector<string> p, vector<RFact> f) {
        def = "RULE " + n + "(";
        for(int i = 0; i < p.size(); i++) {
            def += p;
            if(i < p.size()-1)
                def += ",";
        }
        def += "):- ";
        for
        
    };
    */
};


// Note: it might make more sense in terms of code structure to
// split 'facts' and 'rules', and their associated functions, into KB and RB classes. 
class SRI_Engine {
    private:
      map<string, vector<Fact>> facts;
      map<string, vector<Rule>> rules;
      bool checkFact(const Fact&, const vector<QueryParam>&, int);
    public:
      void addFact(string, string, vector<string>);
      //void addRule(string, string, bool, vector<string>);
      void addRule(string, string, bool, vector<string>, vector<RFact>);
      vector<Fact*> queryFacts(string, vector<string>);
      vector<Fact*> queryRules(string, vector<string>, string);
      vector<Fact*> query(string, vector<string>);
      void print();

};


#endif
