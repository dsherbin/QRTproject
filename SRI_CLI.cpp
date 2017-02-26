//  SRI_CLI.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the SRI command line interface.

#include "SRI_CLI.h"

// Constructor
SRI_CLI::SRI_CLI() {
    engine = new SRI_Engine;
    exit = false;
    // DEBUG SECTION
    std::vector<string> testparams; testparams.push_back("Marry"); testparams.push_back("George");
    engine->addFact("Father", testparams);
    engine->print();
}

SRI_CLI::~SRI_CLI() {
    delete engine;
}

// Begin a CLI session.
void SRI_CLI::start() {
    string input;
    while(!exit) {
        cout << ">> ";
        getline(std::cin, input);
        parse(input);
    }
}

// Split the given string into a vector of substrings.
// Splits on whitespace by default, or by deliminating character if provided.
std::vector<string> SRI_CLI::split(string s, const vector<char>* delim = NULL) {
    std::vector<string> ws;
    string c = "";
    if(delim) {
        bool skip = false;
        for(auto i : s) {
            for(auto j : *delim)
                if(i == j) {
                    if(c != "") {
                        ws.push_back(c);
                        c = "";
                    }
                    skip = true;
                    break;
                }
            if(skip) {
                skip = false;
                continue;
            }
            c += i;
        }
    }
    else {
        for(auto i : s)
            if(!isspace(i))
                c += i;
            else if(c != "") {
                ws.push_back(c);
                c = "";
            }
    }
    
    if(c != "")
        ws.push_back(c);
    return ws;
}

// Parse the given input string.
void SRI_CLI::parse(string input) {
    std::vector<string> words = split(input);
    
    // DEBUG: Print split word input.
    cout << "[DEBUG] INPUT READ: ";
    for(auto i : words) {
        cout << i << ", ";
    }
    cout << endl;
    
    
    if(words.size() < 1) {
        std::cout << "Invalid command." << std::endl;
        return;
    }
    
    // TODO: Split these into functions.
    // Using string literals because these commands are required and static.
    if(words[0] == "LOAD")
        // TODO: Load an SRI file here.
        return;
    else if(words[0] == "DUMP")
        // TODO: Dump an SRI file here.
        return;
    else if(words[0] == "FACT") {
        // Define a fact.
        if(words.size() < 2) {
            std::cout << "Invalid fact." << std::endl;
            return;
        }
        
        vector<char> delim = {'(', ',', ')'};
        std::vector<string> fact_params_all = split(words[1], &delim);
        //fact_params_all.erase(fact_params_all.begin());
        
        std::vector<string> fact_params;
        for(int i = 1; i < fact_params_all.size(); i++)
            fact_params.push_back(fact_params_all[i]);
        
        engine->addFact(fact_params_all[0], fact_params);
        return;
    }
    else if(words[0] == "RULE") {
        // Define a rule.
        //int p = words[1].find(":-");
        string name = words[1].substr(0, words[1].find("("));//words[1].substr(0, words[1].find(":-"));
        //std::cout << ":- found at position " << p << std::e ndl;
        
        string format = words[1].substr(words[1].find("("), words[1].find(")"));
        
        bool type;
        if(words[2] == "AND")
            type = true;
        if(words[2] == "OR")
            type = false;
        
        std::vector<string> rule_params;
        for(int i = 3; i < words.size(); i++)
            rule_params.push_back(words[i]);
        engine->addRule(name, format, type, rule_params);
        
        return;
    }
    else if(words[0] == "INFERENCE") {
        // Perform a query.
        
        // Only targetting fact query right now
        int name_end = words[1].find("(");
        string name = words[1].substr(0, name_end);
        
        vector<char> delim = {'(',')', ','};
        vector<string> query_params = split(
            words[1].substr(name_end), &delim);
        //for(auto i : query_params) std::cout << i << ",";
        
        engine->queryFacts(name, query_params);
    
        //engine->query(
    
        return;
    }
    else if(words[0] == "DROP")
        // TODO: Delete a rule or fact.
        return;
    else if(words[0] == "PRINT") {
        // This is a debug command
        engine->print();
        return;
    }
    else if(words[0] == "q" || words[0] == "quit" || words[0] == "exit") {
        exit = true;
        return;
    }
    
    std::cout << "Unrecognized command." << std::endl;
    return;
}
