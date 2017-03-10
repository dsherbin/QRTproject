//  QueryThreadManager.h
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Prototypes and definitions for the QueryThreadManager class.

#ifndef __querythreadmanager__
#define __querythreadmanager__

#include <vector>
#include <iostream> // For console debugging
#include "Thread.h"
#include "QueryThread.h"
#include "Rule.h"
#include "Fact.h"

using std::vector;

class QueryThreadManager {
    private:
        vector<QueryThread*> qt;
        //vector<Fact> results;
        map<string, vector<Fact>>* kb;
        map<string, vector<Rule>>* rb;
        pthread_mutex_t cout_mtx;
        pthread_mutex_t write_mtx;
    public:
        QueryThreadManager(map<string, vector<Fact>>*, map<string, vector<Rule>>*);
        void addThread(string, vector<string>, int);
        void start();
        void barrier();
        ~QueryThreadManager();
};

#endif
