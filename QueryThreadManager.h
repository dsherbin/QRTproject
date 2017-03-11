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
        vector<QueryThread*> qt;        // List of threads.
        vector<Fact>* results;          // Shared result vector.
        map<string, vector<Fact>>* kb;  // Pointer to knowledge base.
        map<string, vector<Rule>>* rb;  // Pointer to rule base.
        pthread_mutex_t cout_mtx;       // Mutex for cout writing.
        pthread_mutex_t write_mtx;      // Mutex for result set writing.
    public:
        QueryThreadManager(map<string, vector<Fact>>*, map<string, vector<Rule>>*);
        void setup();
        void addThread(string, vector<string>);
        void start();
        vector<Fact>* barrier();
        ~QueryThreadManager();
};

#endif
