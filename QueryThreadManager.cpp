//  QueryThreadManager.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the QueryThreadManager class.
//  These are almost entirely copied from Dr. Sobh's ThreadManager example.

#include "QueryThreadManager.h"

QueryThreadManager::QueryThreadManager() {
    qt = vector<QueryThread*>();
    //std::cout << "qt initialized\n";
    //std::cout << "qt size: " << qt.size() << std::endl;
}

void QueryThreadManager::addThread(QueryThread* q_thread) {
    //std::cout << "adding new thread...\n";
    qt.push_back(q_thread);
    //std::cout << "success\n";
}

void QueryThreadManager::start() {
    for(int i = 0; i < qt.size(); i++) qt[i]->start();
}

void QueryThreadManager::barrier() {
    for(int i = 0; i < qt.size(); i++) qt[i]->waitForRunToFinish();
}

QueryThreadManager::~QueryThreadManager() {
    //std::cout << "~querythreadmanager()\n";
    for(int i = 0; i < qt.size(); i++) delete (qt[i]);
}
