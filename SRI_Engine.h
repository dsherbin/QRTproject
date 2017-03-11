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

// For C++11 style threads:
//#include <thread>
//using std::thread;
//#include <mutex>

#include "Thread.h"
#include "QueryThread.h"
#include "QueryThreadManager.h"

class SRI_Engine {
    private:
      map<string, vector<Fact>> facts;
      map<string, vector<Rule>> rules;
      bool checkFact(const Fact&, const vector<QueryParam>&, int);
      enum clauseType { CT_NONE, CT_FACT, CT_RULE };
    public:
      SRI_Engine();
      void addFact(string, string, vector<string>);
      void addRule(string, string, int, vector<string>, vector<RFact>);
      bool containsFact(string &name);
      bool containsRule(string &name);
      void dropRule(string &name);
      void dropFact(string &name);
      vector<Fact> queryFacts(string, vector<string>);
      vector<Fact> queryRules(string, vector<string>);
      int getClauseType(string);
      vector<Fact> query(string, vector<string>);
      void dump(string filename);
      void addFact(Fact);
      void print();
      ~SRI_Engine();
    
      //C++11 thread test
      //vector<Fact> beginQuery(string, vector<string>);
      //void threadedQuery(string, vector<string>, int, vector<Fact>&);//string, vector<string>, int, vector<Fact>&);

};


#endif
