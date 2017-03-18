//  QueryThreadManager.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the QueryThreadManager class, adapted from Dr. Sobh's ThreadManager example.

#include "QueryThreadManager.h"

// Constructor
QueryThreadManager::QueryThreadManager(map<string, vector<Fact>>* f, map<string, vector<Rule>>* r) {
    kb = f;
    rb = r;
    pthread_mutex_init(&cout_mtx, NULL);
    pthread_mutex_init(&write_mtx, NULL);
}

// Prepare the thread manager. Clears previous result set, if any.
void QueryThreadManager::setup() {
    results = new vector<Fact>();
}

// Add a thread to the query manager. Will process the given fact name with the given parameters.
void QueryThreadManager::addThread(string q_n, vector<string> q_p) {
    qt.push_back(new QueryThread(kb, rb, q_n, q_p, qt.size(), &cout_mtx, &write_mtx, results));
}

// Start the threads.
void QueryThreadManager::start() {
    for(unsigned int i = 0; i < qt.size(); i++) qt[i]->start();
}

// Wait for threads to complete, and return the result set.
vector<Fact>* QueryThreadManager::barrier() {
    for(unsigned int i = 0; i < qt.size(); i++) qt[i]->waitForRunToFinish();
    return results;
}

// Destructor
QueryThreadManager::~QueryThreadManager() {
    for(unsigned int i = 0; i < qt.size(); i++) delete (qt[i]);
    delete results;
}
