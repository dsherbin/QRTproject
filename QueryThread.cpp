//  QueryThread.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the QueryThread class.

#include "QueryThread.h"

QueryThread::QueryThread(map<string, vector<Fact>>* f,
        map<string, vector<Rule>>* r, string n, vector<string> p, int id) {
    kb = f;
    rb = r;
    name = n;
    params = p;
    tid = id;
}

void* QueryThread::threadMainBody(void* arg) {
    std::cout << "thread " << tid << " started\n";
    
    //vector<Fact> results;
    //auto fi = kb->find(name);
    //if(fi != kb->end())
    //    results = queryFacts(name, params);
        //return queryFacts(name, params);
    
    //auto ri = rb->find(name);
    //if(ri != rb->end())
    //    results = queryRules(name, params);
        //return queryRules(name, params);
    
    //res->insert(res->end(), results.begin(), results.end());
    
    //std::cout << "Couldn't find a match in facts or rules.\n";
    
    std::cout << "thread " << tid << " ending\n";
    
    return nullptr;
}
