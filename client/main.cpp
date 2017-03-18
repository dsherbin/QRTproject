//  main.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Entry point for program.

#include "main.h"

#include "TCPSocket.h"

int main(int argc, const char* argv[]) {
    
    std::cout << "Program started. Type 'q' to quit.\n";
    SRI_CLI cli;
    cli.start();
    std::cout << "Program ending.\n";
    
    TCPSocket socket((char*)"localhost", 9999, 10, 10);
}
