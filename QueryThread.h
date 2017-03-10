//  QueryThread.h
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Prototypes and definitions for the QueryThread class.

#ifndef __querythread__
#define __querythread__

#include <vector>
#include <string>
#include <map>
#include <fstream>
using std::vector;
using std::string;
using std::map;

#include <iostream> // For console debugging
#include "Fact.h"
#include "Rule.h"
#include "Thread.h"

class QueryThread : public Thread {
    private:
      map<string, vector<Fact>>* kb;
      map<string, vector<Rule>>* rb;
      //vector<Fact>* res;
      //bool* finished;
      string name;
      vector<string> params;
      int tid;
    public:
      QueryThread(map<string, vector<Fact>>*, map<string, vector<Rule>>*, string, vector<string>, int);
      virtual void* threadMainBody(void*);
};

class QueryThreadManager {
    private:
        vector<QueryThread*> qt;
        vector<Fact> results;
    public:
        QueryThreadManager();
        void addThread(QueryThread* p_thread);
        void start();
        void barrier();
        ~QueryThreadManager();
};
#endif
