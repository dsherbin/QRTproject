//  QueryThreadManager.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the QueryThreadManager class.
//  These are almost entirely copied from Dr. Sobh's ThreadManager example.

#include "QueryThreadManager.h"

QueryThreadManager::QueryThreadManager(map<string, vector<Fact>>* f, map<string, vector<Rule>>* r) {
    //qt = vector<QueryThread*>();
    kb = f;
    rb = r;
    pthread_mutex_init(&cout_mtx, NULL);
    pthread_mutex_init(&write_mtx, NULL);
    //n_threads = 0;
    //results = new vector<Fact>();
    //std::cout << "qt initialized\n";
    //std::cout << "qt size: " << qt.size() << std::endl;
}
/*
void QueryThreadManager::addThread(QueryThread* q_thread) {
    //std::cout << "adding new thread...\n";
    qt.push_back(q_thread);
    //std::cout << "success\n";
}
*/

// Prepare the thread manager. Clears previous result set, if any.
void QueryThreadManager::setup() {
    //if(results) delete results;
    results = new vector<Fact>();
}

void QueryThreadManager::addThread(//map<string, vector<Fact>>* f, map<string, vector<Rule>>* r,
            string q_n, vector<string> q_p) {
    qt.push_back(new QueryThread(kb, rb, q_n, q_p, qt.size(), &cout_mtx, &write_mtx, results));
}

void QueryThreadManager::start() {
    std::cout << "querythreadmanager::start()\n";
    for(int i = 0; i < qt.size(); i++) qt[i]->start();
}

vector<Fact>* QueryThreadManager::barrier() {
    for(int i = 0; i < qt.size(); i++) qt[i]->waitForRunToFinish();
    return results;
}

QueryThreadManager::~QueryThreadManager() {
    //std::cout << "~querythreadmanager() start\n";
    for(int i = 0; i < qt.size(); i++) delete (qt[i]);
    //n_threads = 0;
    //if(results) delete results;
    //std::cout << "~querythreadmanager() end\n";
}
