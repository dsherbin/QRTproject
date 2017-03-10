//  main.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Entry point for program.

#include "main.h"

/*
#include "Thread.h"

class TestThread: public Thread {
    virtual void* threadMainBody(void*);
};
void* TestThread::threadMainBody(void* arg) {
    //std::cout << "threadMainBody\n";
    printf("threadMainBody\n");
}
*/


int main(int argc, const char* argv[]) {
    //TestThread tt;
    //tt.start();
    
    
    std::cout << "Program started. Type 'q' to quit.\n";
    SRI_CLI cli;
    cli.start();
    std::cout << "Program ending.\n";
    
    return 0;
}
