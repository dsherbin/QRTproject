//  SRI_Engine.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the SRI engine.

#include "SRI_Engine.h"

// Default constructor
SRI_Engine::SRI_Engine() : qtm(&facts, &rules) {}

// Destructor
SRI_Engine::~SRI_Engine() {}

// Adds a fact to the SRI engine. Searches the KB for duplicate entries, and fails
// with a message to stdout if a duplicate exists.
// Params:
//      name: The name of the fact to be added. Not necessarily unique to other facts. Example: "Father"
//      params: A list of parameters associated with the fact. Example: "[Roger, Fred]"
//
// TODO: Check rule base -- don't allow a new fact to share a name with a rule.
void SRI_Engine::addFact(std::string def, string name, vector<string> params) {
    // Check for duplicate entries.
    for(auto i: facts[name]) {
        if(i.vals.size() != params.size()) continue; // Not a match.
        bool entries_match = true;
        for(unsigned int j = 0; j < i.vals.size(); j++) {
            if(params[j] != i.vals[j]) {
                entries_match = false; // Not a match.
                break;
            }
        }
        if(entries_match) {
            std::cout << "Cannot add Fact: duplicate entry.\n";
            return;
        }
    }

    // Add to fact list.
    facts[name].push_back(Fact(name, def, params));
}

void SRI_Engine::addFact(Fact f) {
    addFact(f.def, f.name, f.vals);
}

// Adds a rule to the SRI engine.
void SRI_Engine::addRule(string def, string name, int type, vector<string> params, vector<RFact> rfacts) {
    // Duplicate rules are allowed, but rules cannot share names with facts.
    auto it = facts.find(name);
    if(it != facts.end()) {
        std::cout << "Cannot add Rule: a Fact of that name already exists.\n";
        return;
    }
    rules[name].push_back(Rule(def, type, params, rfacts));
}

bool SRI_Engine::containsFact(string &name){
    return (facts.find(name) != facts.end());
}

bool SRI_Engine::containsRule(string &name){
    return (rules.find(name) != rules.end());
}

void SRI_Engine::dropFact(string &name){
    facts.erase(name);
}

void SRI_Engine::dropRule(string &name){
    rules.erase(name);
}


// Return true if the given Fact meets the requirements of the given QueryParam list.
// IMPORTANT: this function treats repeat variable situations, such as ($A,$A) in a specific manner:
//              If ($A,$A) are the original params, $A must equal $A.
//              However, if ($A,$B) are the params, $A may or may not equal $B. ($A,$A) will return a subset of ($A,$B).
//              This behavior can be changed (fixed?) later by running another check through the queryParams.
// Params:
//      f: The specific Fact to check against.
//      qp: A list of QueryParams, as generated by queryFacts().
//      np: The actual number of parameters, after accounting for qp's pos system. (That is, not necessarily equal to qp.size)
// Returns:
//      true if the fact meets the requirements defined by the QueryParams, false otherwise.
bool SRI_Engine::checkFact(const Fact& f, const vector<QueryParam>& qp, int np) {
    if(f.vals.size() != (unsigned int)np) {
        return false; // Check that lengths match.
    }
    
    vector<QueryParam> current_vals(qp); // Make copy of parameters, so we can edit this copy.
    
    // Loop through each value of the fact.

    for(unsigned int i = 0; i < f.vals.size(); i++) {
        // Note that i is the position of the fact's value.
        // I.E. for Father(Roger,George), Roger is in position i = 0, George in i = 1.

        // Find the corresponding QueryParam by looping through
        // the list until we get a position match.
        QueryParam* comp = NULL;
        for(unsigned int j = 0; j < current_vals.size() && comp == NULL; j++) {
            for(unsigned int k = 0; k < current_vals[j].pos.size(); k++) {
                if((int)i == current_vals[j].pos[k])
                    comp = &current_vals[j];
            }
        }
        // Given that we already checked the size of the param list, we don't need to test for NULL here.
        
        
        // Wildcard values must match values of the same variable, but can be anything.
        // If we don't already have a value here, just set it to whatever.
        if(comp->wildcard && comp->value == "") {
            comp->value = f.vals[i];
        }
        else {
            if(comp->value != f.vals[i])
                return false;
                // The value in this position does not match the expected value.
                // This can occur if we're searching for a literal, or if we have
                // repeated parameters.
        }
    }
    return true;
}

// Get a list of matching facts with the given name and parameters.
// Searches through all matching facts found in the given name's category.
//
// For example, calling queryFacts with f_name = Father will iterate through
// all Facts stored in the KB with the name Father, compare them to the given
// list of requirements, and return a list of Facts that satifisy them.
// Params:
//      f_name: The Fact name-type to check.
//      params: A list of string parameters used to call the query.
//              For example, if Father($A,George) is the query, params = ["$A", "George"].
//              Note that the $ is retained for 'wildcard' parameters to denote them as such.
//
vector<Fact> SRI_Engine::queryFacts(string f_name, vector<string> params) {
    int nParams = params.size();
    vector<QueryParam> qParams;
    vector<Fact> results;
    
    // Build the list of query parameters from the raw parameter input.
    // Loop through each given param string.
    for(unsigned int i = 0; i < params.size(); i++) {
        bool dupe = false;
        // Compare the given string to the values already converted into QueryParams.
        for(unsigned int j = 0; j < qParams.size(); j++) {
            if(params[i] == qParams[j].given_name) {
                // If the names match, we have multiple params representing the same value, I.E. Employer($A,$A).
                dupe = true;
                qParams[j].pos.push_back(i);
                break;
            }
        }
        if(!dupe){
            qParams.push_back(QueryParam(params[i], i));
        }
    }
    
    for(unsigned int i = 0; i < facts[f_name].size(); i++) {
        if(checkFact(facts[f_name][i], qParams, nParams)) {
            results.push_back(facts[f_name][i]);
        }
    }
    
    return results;
}

// Query predefined rules.
vector<Fact> SRI_Engine::queryRules(string r_name, vector<string> params) {
    // Example usage:
    //  defined with RULE GrandFather($X,$Y):- AND Father($X,$Z) Parent($Z,$Y)
    //  queried with INFERENCE GrandFather($A,$B)
    
    vector<Fact> results;
    vector<Rule>* rl = &(rules[r_name]);
    
    
    
    for(vector<Rule>::iterator ri = rl->begin(); ri != rl->end(); ri++) {
        Rule* r = &(*ri);
        if(!r) { std::cout << "No rule by that name.\n"; return results; }


        // Build a mapping structure so we know what the "actual" params being used
        // in place of the rule params.
        // For example, if rule R($X,$Y) invoked with ($A,George), this map will contain:
        //  [$X] = $A
        //  [$Y] = George
        map<string,string> rule_param_to_calling_param;
        for(unsigned int i = 0; i < r->params.size(); i++) {
            rule_param_to_calling_param[r->params[i]] = params[i];
        }
        
        if(r->operation == Rule::AND) {
            //std::cout << "AND rule\n";
            // First fact parameter format copy for editing
            vector<string> factParams(r->facts[0].params);
            
            for(unsigned int j = 0; j < factParams.size(); j++) {
                auto it = rule_param_to_calling_param.find(factParams[j]);
                if(it != rule_param_to_calling_param.end())
                    factParams[j] = it->second;
            }
            
            // Fact params is now the list of calling params where
            // any previously defined param of the rule is replaced with a
            // calling param, if available.
            vector<Fact> init_results = query(r->facts[0].name, factParams);
            
            // We want to call the next fact in the list, using results from
            // this list only if applicable.
            for(unsigned int fr = 0; fr < init_results.size(); fr++) {
                vector<string> secondFactParams(r->facts[1].params);
                for(unsigned int j = 0; j < secondFactParams.size(); j++) {
                    auto it = rule_param_to_calling_param.find(secondFactParams[j]);
                    if(it != rule_param_to_calling_param.end())
                        secondFactParams[j] = it->second;
                    
                    for(unsigned int k = 0; k < factParams.size(); k++)
                        if(secondFactParams[j] == factParams[k])
                            secondFactParams[j] = init_results[fr].vals[k];
                }
                
                
                vector<Fact> second_results = query(r->facts[1].name, secondFactParams);
                
                for(unsigned int sr = 0; sr < second_results.size(); sr++) {
                    map<string,string> final_param_map;
                    for(unsigned int j = 0; j < factParams.size(); j++) {
                        final_param_map[factParams[j]] = secondFactParams[j];
                    }
                    for(unsigned int j = 0; j < secondFactParams.size(); j++) {
                        final_param_map[secondFactParams[j]] = second_results[sr].vals[j];
                    }
                    
                    vector<string> newvals;
                    for(unsigned int j = 0; j < params.size(); j++) {
                        auto it = final_param_map.find(params[j]);
                        if(it != final_param_map.end())
                            newvals.push_back(it->second);
                    }
                    
                    results.push_back(Fact("custom", "custom", newvals));
                }
            }
        }else if(r->operation == Rule::OR) {
            
            QueryThreadManager tm(&facts, &rules);
            tm.setup();
        
            for(unsigned int i = 0; i < r->facts.size(); i++) {
                vector<string> factParams(r->facts[i].params); // Copy parameter format for editing
                
                // Iterate through each parameter and try to match it to a previous parameter.
                for(unsigned int j = 0; j < factParams.size(); j++) {
                    auto it = rule_param_to_calling_param.find(factParams[j]);
                    if(it != rule_param_to_calling_param.end())
                        factParams[j] = it->second;
                }
                // factParams now contains the list of calling params where
                // any previously defined param of the rule is replaced with
                // a calling param, if available.
                
                if(getClauseType(r->facts[i].name) == CT_FACT)
                    tm.addThread(r->facts[i].name, factParams);
                else {
                    vector<Fact> fact_results = query(r->facts[i].name, factParams);
                    results.insert(results.end(), fact_results.begin(), fact_results.end());
                }
            }
            
            tm.start();
            vector<Fact>* qt_res = tm.barrier();
            results.insert(results.end(), qt_res->begin(), qt_res->end());
        }
    }
    return results;
}

// Searchs the database for the given name, and returns the type (fact, rule or none).
int SRI_Engine::getClauseType(string name) {
    auto fi = facts.find(name);
    if(fi != facts.end()){
        return CT_FACT;
    }
    
    auto ri = rules.find(name);
    if(ri != rules.end()){
        return CT_RULE;
    }
    
    return CT_NONE;
}

// Perform a query on either a rule or a fact.
// Scans the KB and RB for matches to name, then calls queryFact or queryRule
// depending on where we found the match.
//
// Assumes no duplicate names exist between rules and facts.
// In this situation, it will only return the first entry found (the queryfact results).
vector<Fact> SRI_Engine::query(string name, vector<string> params) {
    int ctype = getClauseType(name);
    switch(ctype) {
        case CT_FACT:
            return queryFacts(name, params);
            break;
        case CT_RULE:
            return queryRules(name, params);
            break;
        default:
            std::cout << "Couldn't find a match in facts or rules.\n";
            return vector<Fact>(); // return empty list.
            break;
    }
}

//dump the contents of the SRI_Engine to the file specified by filename
void SRI_Engine::dump(string filename){
    std::ofstream out(filename, std::ofstream::out);

    //dump facts
    for(auto it = facts.begin(); it != facts.end(); ++it){
        std::pair <const std::basic_string<char>, std::vector<Fact> > val = (*it);
        vector<Fact> outFacts = std::get<1>(val);
        for(auto iter = outFacts.begin(); iter != outFacts.end(); ++iter){
            out << (*iter).def << std::endl;
            std::cout << (*iter).def << std::endl;
        }
    }
    //then rules
    for(auto it = rules.begin(); it != rules.end(); ++it){
        std::pair <const std::basic_string<char>, std::vector<Rule> > val = (*it);
        vector<Rule> outRules = std::get<1>(val);
        for(auto iter = outRules.begin(); iter != outRules.end(); ++iter){
            out << (*iter).def << std::endl;
            std::cout << (*iter).def << std::endl;
        }
    }
    out.close();
}

// This is a debug function that dumps all facts and rules to the console.
void SRI_Engine::print() {
    // Print all facts
    for(auto i : facts) {
        std::cout << "facts[" << i.first << "] = \n";
        for(auto j : i.second) {
            std::cout << j.def << std::endl;
        }
        std::cout << std::endl;
    }
    
    // Print all rules
    for(auto i : rules) {
        std::cout << "rules[" << i.first << "] = \n";
        for(auto j : i.second) {
            std::cout << j.def << std::endl;
            
            std::cout << "operation = " << j.operation << std::endl;
            
            for(auto f : j.facts) {
                std::cout << f.name << ", ";
            }
        }
        std::cout << std::endl;
    }
}
