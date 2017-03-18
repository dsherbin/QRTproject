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
    //engine = new SRI_Engine;
    exit = false;
    
    /*
    // pre-populate with example commands for testing
    parse("FACT Father(Roger,John)");
    parse("FACT Mother(Marry,John)");
    parse("FACT Father(Roger,Albert)");
    parse("FACT Mother(Marry,Albert)");
    parse("FACT Father(Allen,Margret)");
    parse("FACT Mother(Margret,Robert)");
    parse("FACT Mother(Margret,Bob)");
    
    parse("FACT Apple(Red,Delicious)");
    parse("FACT Apple(Green,Sour)");
    parse("FACT Orange(Orange,Delicious)");
    parse("FACT Orange(Green,Sour)");
    parse("FACT Cherry(Red,Delicious)");
    parse("FACT Cherry(Green,Sour)");
    parse("FACT Banana(Yellow,Delicious)");
    parse("FACT Banana(Green,Sour)");

    parse("RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)");
    parse("RULE QueryTest($X,$Y):- OR Apple($X,$Y) Orange($X,$Y) Cherry($X,$Y) Banana($X,$Y)");
    parse("RULE GrandFather($X,$Y):- AND Father($X,$Z) Parent($Z,$Y)");
    */
}

SRI_CLI::~SRI_CLI() {
    //delete engine;
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
// (using previous function for delimitor list support)
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

void SRI_CLI::load(const string filename){
    std::ifstream in(filename, std::ofstream::in);
    for(std::string line; getline(in, line);){
        parse(line);
    }
    in.close();
    string output = filename + "loaded.\n";
    return output;
}

string SRI_CLI::inference(std::vector<string> &words){
    string output = "";
    // Perform a query.
    int name_end = words[1].find("(");
    string name = words[1].substr(0, name_end);
    
    vector<char> delim = {'(',')', ','};
    vector<string> query_params = split(words[1].substr(name_end), &delim);
    //vector<Fact> results = engine->query(name, query_params);
    vector<Fact> results;
    
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
                output += query_params[j];
                output += ":";
                output += results[i].vals[j];
                if(--nqp)
                    output += ", ";
            }
        }
        output += '\n';
    }
    
    if(words.size() == 3) {
        string set_name = words[2];
        // This result is to be saved with the given name.
        for(unsigned int i = 0; i < results.size(); ++i) {
            results[i].name = set_name;
            string d = "FACT " + set_name + "(";
            for(unsigned int j = 0; j < results[i].vals.size(); ++j) {
                d += results[i].vals[j];
                if(j < results[i].vals.size() - 1)
                    d += ",";
            }
            d += ")";
            results[i].def = d;
            //engine->addFact(results[i]);
        }
    }
    return output;
}

string SRI_CLI::fact(std::vector<string> &words, string &input){
    // Define a fact.
    if(words.size() < 2) {
        std::cout << "Invalid fact." << std::endl;
        return;
    }

    vector<char> delim = {'(', ',', ')'};
    std::vector<string> fact_params_all = split(words[1], &delim);
    
    std::vector<string> fact_params;
    for(int i = 1; i < fact_params_all.size(); i++) {
        fact_params.push_back(fact_params_all[i]);
    }
    
    //engine->addFact(input, fact_params_all[0], fact_params);
    return "Added fact\n";
}

string SRI_CLI::rule(std::vector<string> &words, string &input){
    vector<char> delim = {'(', ',', ')'};
    // Example of a valid rule input:
    //   RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)
    //   words[0]: RULE, words[1]: Parent($X,$Y):-, words[2]: OR
    //   words[3]: Father($X,$Y), words[4]: Mother($X,$Y);
    int name_end = words[1].find("(");
    string name = words[1].substr(0, name_end);
    int params_end = words[1].find(")");
    string raw_params = words[1].substr(name_end, (params_end - name_end) + 1);
    vector<string> params = split(raw_params, &delim);

    int type;
    if(words[2] == "AND")
        type = 1;
    else // OR
        type = 2;

    vector<RFact> rfacts;

    for(unsigned int i = 3; i < words.size(); i++) {
        vector<string> rfact_params = split(words[i], &delim);
        string rfact_name = rfact_params[0];
        rfact_params.erase(rfact_params.begin());
        rfacts.push_back(RFact(rfact_name, rfact_params));
    }

    //engine->addRule(input, name, type, params, rfacts);
    return "Added rule";
}

string SRI_CLI::drop(std::vector<string> &words){
    if(engine->containsRule(words[1])){
        engine->dropRule(words[1]);
    }

    if(engine->containsFact(words[1])){
        engine->dropFact(words[1]);
    }
    return "Dropped\n";
}

// Parse the given input string.
string SRI_CLI::parse(string input) {
    std::vector<string> words = split(input);
    
    if(words.size() < 1) {
        std::cout << "Invalid command." << std::endl;
        return;
    }
    
    // Using string literals because these commands are required and static.
    if(words[0] == "LOAD"){
        return load(words[1]);
    }else if(words[0] == "DUMP"){
        //engine->dump(words[1]);
        return "DUMPED";
    }else if(words[0] == "FACT"){
        return fact(words, input);
    }else if(words[0] == "RULE"){
        return rule(words, input);
    }else if(words[0] == "INFERENCE"){
        return inference(words);
    }else if(words[0] == "DROP"){
        return drop(words);
    }else if(words[0] == "PRINT"){
        //engine->print();
        return "";  //later
    }else{
        return NULL;
    }
}
