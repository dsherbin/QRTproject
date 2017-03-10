//  SRI_CLI.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the SRI command line interface.

#include "SRI_CLI.h"

#include <iterator>
#include <sstream>
#include <algorithm>

// Constructor
SRI_CLI::SRI_CLI() {
    engine = new SRI_Engine;
    exit = false;
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
std::vector<string> SRI_CLI::split(string s, const vector<char> * delim = NULL){
    std::istringstream iss(s);
    std::vector<string> words = {   std::istream_iterator<string>{iss},
                                    std::istream_iterator<string>{} };
    return words;
}

void SRI_CLI::load(const string filename){
    std::ifstream in(filename, std::ofstream::in);
    for(std::string line; getline(in, line);){
        parse(line);
    }
    in.close();
}

void SRI_CLI::inference(std::vector<string> &words){
    // Perform a query.
    int name_end = words[1].find("(");
    string name = words[1].substr(0, name_end);
    
    vector<char> delim = {'(',')', ','};
    vector<string> query_params = split(words[1].substr(name_end), &delim);
    
    vector<Fact*> results = engine->query(name, query_params);
    
    // Set output formatting of query params.
    int nqp_total = 0;
    for(unsigned int i = 0; i < query_params.size(); ++i) {
        if(query_params[i][0] == '$') {
            query_params[i] = query_params[i].substr(1);
            nqp_total++;
        }
        else
            query_params[i] = "";
    }
    
    // Print
    for(unsigned int i = 0; i < results.size(); ++i) {
        int nqp = nqp_total;
        for(unsigned int j = 0; j < query_params.size(); ++j) {
            if(query_params[j] != "") {
                cout << query_params[j] << ":" << results[i]->vals[j];
                if(--nqp)
                    cout << ", ";
            }
        }
        cout << endl;
    }
    
    if(words.size() == 3) {
        string set_name = words[2];
        // This result is to be saved with the given name.
        for(unsigned int i = 0; i < results.size(); ++i) {
            results[i]->name = set_name;
            string d = "FACT " + set_name + "(";
            for(unsigned int j = 0; j < results[i]->vals.size(); ++j) {
                d += results[i]->vals[j];
                if(j < results[i]->vals.size() - 1)
                    d += ",";
            }
            d += ")";
            results[i]->def = d;
            engine->addFact(*results[i]);
        }
    }
}

void SRI_CLI::fact(std::vector<string> &words, string &input){
    // Define a fact.
    if(words.size() < 2) {
        std::cout << "Invalid fact." << std::endl;
        return;
    }

    // TODO: This can be tightened up.
    vector<char> delim = {'(', ',', ')'};
    std::vector<string> fact_params_all = split(words[1], &delim);
    
    std::vector<string> fact_params;
    for(auto it = fact_params_all.begin(); it != fact_params_all.end(); ++it){
        fact_params.push_back(*it);
    }
    
    engine->addFact(input, fact_params_all[0], fact_params);
}

void SRI_CLI::rule(std::vector<string> &words, string &input){
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
    vector<string> params = split(raw_params, &delim);

    bool type;
    if(words[2] == "AND")
        type = true;
    if(words[2] == "OR")
        type = false;

    vector<RFact> rfacts;

    for(unsigned int i = 3; i < words.size(); i++) {
        vector<string> rfact_params = split(words[i], &delim);
        string rfact_name = rfact_params[0];
        rfact_params.erase(rfact_params.begin());
        rfacts.push_back(RFact(rfact_name, rfact_params));
    }

    engine->addRule(input, name, type, params, rfacts);
}

// Parse the given input string.
void SRI_CLI::parse(string input) {
    std::vector<string> words = split(input);
    
    if(words.size() < 1) {
        std::cout << "Invalid command." << std::endl;
        return;
    }
    
    // TODO: Split these into functions.
    // Using string literals because these commands are required and static.
    if(words[0] == "LOAD"){
        load(words[1]);
    }else if(words[0] == "DUMP"){
        engine->dump(words[1]);
    }else if(words[0] == "FACT") {
        fact(words, input);
    }else if(words[0] == "RULE") {
        rule(words, input);
    }
    else if(words[0] == "INFERENCE") {
        inference(words);
    }
    else if(words[0] == "DROP")
        // TODO: Delete a rule or fact.
        return;
    else if(words[0] == "PRINT") {
        // This is a debug command
        engine->print();
    }
    else if(words[0] == "q" || words[0] == "quit" || words[0] == "exit") {
        exit = true;
    }else{
        std::cout << "Unrecognized command." << std::endl;
    }
}
