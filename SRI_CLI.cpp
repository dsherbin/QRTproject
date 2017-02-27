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
    //std::vector<string> testparams; testparams.push_back("Marry"); testparams.push_back("George");
    //engine->addFact("FACT Father(Marry,George)", "Father", testparams);
    
    // Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)
    //vector<string> rparams; rparams.push_back("$X"); rparams.push_back("$Y");
    //vector<RFact> rfacts;
    //vector<string> father_params; father_params.push_back("$X"); father_params.push_back("$Y");
    //rfacts.push_back(RFact("Father", father_params));
    //vector<string> mother_params; mother_params.push_back("$X"); mother_params.push_back("$Y");
    //rfacts.push_back(RFact("Mother", mother_params));
    
    //engine->addRule("RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)", "Parent", false, rparams, rfacts);
    parse("FACT Father(Roger,John)");
    parse("FACT Mother(Marry,John)");
    parse("FACT Father(Roger,Albert)");
    parse("FACT Mother(Marry,Albert)");
    parse("RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)");
    
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
        
        
        // TODO: This can be tightened up.
        vector<char> delim = {'(', ',', ')'};
        std::vector<string> fact_params_all = split(words[1], &delim);
        //fact_params_all.erase(fact_params_all.begin());
        
        std::vector<string> fact_params;
        for(int i = 1; i < fact_params_all.size(); i++)
            fact_params.push_back(fact_params_all[i]);
        
        engine->addFact(input, fact_params_all[0], fact_params);
        return;
    }
    else if(words[0] == "RULE") {
        vector<char> delim = {'(', ',', ')'};
        // Example of a valid rule input:
        //   RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)
        //   words[0]: RULE, words[1]: Parent($X,$Y):-, words[2]: OR
        //   words[3]: Father($X,$Y), words[4]: Mother($X,$Y);
        // NYI -- does nothing
        // Define a rule.
        int name_end = words[1].find("(");
        string name = words[1].substr(0, name_end);
        int params_end = words[1].find(")");
        string raw_params = words[1].substr(name_end, (params_end - name_end) + 1);
        //std::cout << "raw_params: " << raw_params << std::endl;
        vector<string> params = split(raw_params, &delim);
        //std::cout << "params: ";
        //for(auto i : params) std::cout << i << ",";
        //std::cout << std::endl;
        
        bool type;
        if(words[2] == "AND")
            type = true;
        if(words[2] == "OR")
            type = false;
        
        vector<RFact> rfacts;
        
        for(int i = 3; i < words.size(); i++) {
            vector<string> rfact_params = split(words[i], &delim);
            string rfact_name = rfact_params[0];
            rfact_params.erase(rfact_params.begin());
            
            rfacts.push_back(RFact(rfact_name, rfact_params));
            //std::cout << "rfact_name for word[" << i << "]: " << rfact_name << std::endl;
            //for(auto j : rfact_params)
            //    std::cout << "rfact_params from word[" << i << "]: " << j << std::endl;
        }
        
        /*
        std::vector<string> rfact_params;
        for(int i = 3; i < words.size(); i++)
            rfact_params.push_back(words[i]);
        */
        
        //engine->addRule(input, name, type, rfacts);
        
        
        engine->addRule(input, name, type, params, rfacts);
        
        return;
    }
    else if(words[0] == "INFERENCE") {
        // Perform a query.
        
        int name_end = words[1].find("(");
        string name = words[1].substr(0, name_end);
        
        vector<char> delim = {'(',')', ','};
        vector<string> query_params = split(words[1].substr(name_end), &delim);
        //for(auto i : query_params) std::cout << i << ",";
        
        //engine->queryFacts(name, query_params);
    
        engine->query(name, query_params);
    
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
