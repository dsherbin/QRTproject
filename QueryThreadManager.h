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

using std::vector;

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
