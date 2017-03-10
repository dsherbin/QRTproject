//  QueryThreadManager.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the QueryThreadManager class.
//  These are almost entirely copied from Dr. Sobh's ThreadManager example.

#include "QueryThreadManager.h"

QueryThreadManager::QueryThreadManager(map<string, vector<Fact>>* f, map<string, vector<Rule>>* r) {
    qt = vector<QueryThread*>();
    kb = f;
    rb = r;
    pthread_mutex_init(&cout_mtx, NULL);
    pthread_mutex_init(&write_mtx, NULL);
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
void QueryThreadManager::addThread(//map<string, vector<Fact>>* f, map<string, vector<Rule>>* r,
            string q_n, vector<string> q_p, int id) {
    qt.push_back(new QueryThread(kb, rb, q_n, q_p, id, &cout_mtx, &write_mtx));
}

void QueryThreadManager::start() {
    //results = Vector<Fact>(); // Clear results list.
    for(int i = 0; i < qt.size(); i++) qt[i]->start();
}

void QueryThreadManager::barrier() {
    for(int i = 0; i < qt.size(); i++) qt[i]->waitForRunToFinish();
}

QueryThreadManager::~QueryThreadManager() {
    //std::cout << "~querythreadmanager()\n";
    for(int i = 0; i < qt.size(); i++) delete (qt[i]);
}
