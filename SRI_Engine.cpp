//  SRI_Engine.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Function definitions for the SRI engine.

#include "SRI_Engine.h"


// Adds a fact to the SRI engine. Searches the KB for duplicate entries, and fails
// with a message to stdout if a duplicate exists.
// Params:
//      name: The name of the fact to be added. Not necessarily unique to other facts. Example: "Father"
//      params: A list of parameters associated with the fact. Example: "[Roger, Fred]"
//
// TODO: Check rule base -- don't allow a new fact to share a name with a rule.
void SRI_Engine::addFact(string def, string name, vector<string> params) {
    // Check for duplicate entries.
    for(auto i: facts[name]) {
        if(i.vals.size() != params.size()) continue; // Not a match.
        bool entries_match = true;
        for(unsigned int j = 0; j < i.vals.size(); j++) {
            //std::cout << "comparing " << params[j] << " and " << i.vals[j] << std::endl;
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
void SRI_Engine::addRule(string def, string name, bool type, vector<string> params, vector<RFact> rfacts) {
    // Duplicate rules are allowed, but rules cannot share names with facts.
    auto it = facts.find(name);
    if(it != facts.end()) {
        std::cout << "Cannot add Rule: a Fact of that name already exists.\n";
        return;
    }
    rules[name].push_back(Rule(def, type, params, rfacts));
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
        //std::cout << "[DEBUG]: Fact doesn't match because the number of parameters don't match the number of values.\n";
        return false; // Check that lengths match.
    }
    
    vector<QueryParam> current_vals(qp); // Make copy of parameters, so we can edit this copy.
    
    // Loop through each value of the fact.
<<<<<<< HEAD
    for(unsigned int i = 0; i < f.vals.size(); i++) {
        std::cout << "[DEBUG]: number of values to check: " << f.vals.size() << std::endl;
=======
    for(int i = 0; i < f.vals.size(); i++) {
        //std::cout << "[DEBUG]: number of values to check: " << f.vals.size() << std::endl;
>>>>>>> origin/master
        // Note that i is the position of the fact's value.
        // I.E. for Father(Roger,George), Roger is in position i = 0, George in i = 1.

        // Find the corresponding QueryParam by looping through
        // the list until we get a position match.
        QueryParam* comp = NULL;
        for(int j = 0; j < current_vals.size() && comp == NULL; j++) {
            for(int k = 0; k < current_vals[j].pos.size(); k++) {
                if(i == current_vals[j].pos[k])
                    comp = &current_vals[j];
            }
        }
        // Given that we already checked the size of the param list, we don't need to test for NULL here.
        
        
        // Wildcard values must match values of the same variable, but can be anything.
        // If we don't already have a value here, just set it to whatever.
        if(comp->wildcard && comp->value == "") {
            //std::cout << "[DEBUG]: comp->value == \"\" in this wildcard, so we're taking the value " << f.vals[i] << std::endl;
            comp->value = f.vals[i];
        }
        else {
            //std::cout << "[DEBUG]: comparing " << comp->value << " to " << f.vals[i] << std::endl;
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
    for(int i = 0; i < params.size(); i++) {
        bool dupe = false;
        // Compare the given string to the values already converted into QueryParams.
        for(int j = 0; j < qParams.size(); j++) {
            if(params[i] == qParams[j].given_name) {
                // If the names match, we have multiple params representing the same value, I.E. Employer($A,$A).
                //std::cout << "[DEBUG]: found a duplicate param\n";
                dupe = true;
                qParams[j].pos.push_back(i);
                //std::cout << "[DEBUG]: new pos list contains: ";
                //for(auto k : qParams[j].pos) {  std::cout << k << ","; }
                //std::cout << std::endl;
                break;
            }
        }
        if(!dupe) qParams.push_back(QueryParam(params[i], i));
    }
    
    //std::cout << "[DEBUG]: qParams contains: " << std::endl;
    //for(auto i : qParams) {
    //    std::cout << "[DEBUG]: " << i.given_name << " in positions ";
    //    for(auto j : i.pos) std::cout << j << ", ";
    //    std::cout << std::endl;
    //}
    
    //std::cout << "[DEBUG]: Checking each Fact of type " << f_name << std::endl;
    //for(auto i : facts[f_name]) {
    for(int i = 0; i < facts[f_name].size(); i++) {
        
        //for(auto x : i.vals) { std::cout << x << ","; }
        //std::cout << std::endl;
    
        if(checkFact(facts[f_name][i], qParams, nParams)) {
            results.push_back(facts[f_name][i]);
            //std::cout << "Matched fact\n";
            //std::cout << "matches: " << results.size() << std::endl;
        }
        //else std::cout << "Couldn't match fact\n";
    }
    //std::cout << "matches: " << results.size() << std::endl;
    
    //std::cout << "results in fact[" << f_name << "]: " << std::endl;
    //for(auto i : results) {
    //for(int i = 0; i < results.size(); i++) {
        //for(int j = 0; j < results[i]->vals.size(); j++)
            //std::cout << results[i]->vals[j] << ",";
        //for(auto j : i->vals) {
        //    std::cout << j << ",";
        //}
        //std::cout << std::endl;
    //}
    
    return results;
}

// Query predefined rules.
vector<Fact> SRI_Engine::queryRules(string r_name, vector<string> params, string res_name = "") {
    // Example usage:
    //  defined with RULE GrandFather($X,$Y):- AND Father($X,$Z) Parent($Z,$Y)
    //  queried with INFERENCE GrandFather($A,$B)
    
    // What needs to happen here:
    //  LOOP THROUGH EACH "GrandFather" RULE: can have multiple definitions.
    //  for each rule:
    //      for the first fact defined in the rule, compare param to rule format saved.
    //
    
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
        for(int i = 0; i < r->params.size(); i++) {
            rule_param_to_calling_param[r->params[i]] = params[i];
            //std::cout << r->params[i] << " = " << params[i] << std::endl;
        }
        
        if(r->andtype) {
            // This is an AND rule.
            
            // First fact parameter format copy for editing
            vector<string> factParams(r->facts[0].params);
            
            for(int j = 0; j < factParams.size(); j++) {
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
            //std::cout << "init_results length: " << init_results.size() << std::endl;
            for(int fr = 0; fr < init_results.size(); fr++) {
            //for(auto fr : init_results) {
            
                vector<string> secondFactParams(r->facts[1].params);
                for(int j = 0; j < secondFactParams.size(); j++) {
                    auto it = rule_param_to_calling_param.find(secondFactParams[j]);
                    if(it != rule_param_to_calling_param.end())
                        secondFactParams[j] = it->second;
                    
                    for(int k = 0; k < factParams.size(); k++)
                        if(secondFactParams[j] == factParams[k])
                            secondFactParams[j] = init_results[fr].vals[k];//fr.vals[k];//init_results[0]->vals[k];
                    }
                    
                //std::cout << "secondFactParams: ";
                //for(auto p : secondFactParams) { std::cout << p << ", "; }
                //std::cout << std::endl;
                
                vector<Fact> second_results = query(r->facts[1].name, secondFactParams);
                
                //std::cout << "second_results: \n";
                //for(auto sr : second_results) {
                for(int sr = 0; sr < second_results.size(); sr++) {
                    //for(auto x : sr.vals) {
                    //    std::cout << x << ",";
                    //}
                    //std::cout << std::endl;
                    
                    map<string,string> final_param_map;
                    for(int j = 0; j < factParams.size(); j++) {
                        final_param_map[factParams[j]] = secondFactParams[j];
                        //std::cout << factParams[j] << " = " << secondFactParams[j] << std::endl;
                    }
                    for(int j = 0; j < secondFactParams.size(); j++) {
                        final_param_map[secondFactParams[j]] = second_results[sr].vals[j];//sr.vals[j];
                        //std::cout << secondFactParams[j] << " = " << sr.vals[j] << std::endl;
                    }
                    
                    
                    vector<string> newvals;
                    for(int j = 0; j < params.size(); j++) {
                        //rule_param_to_calling_param[r->params[i]] = params[i];
                        //std::cout << r->params[i] << " = " << params[i] << std::endl;
                        auto it = final_param_map.find(params[j]);
                        if(it != final_param_map.end())
                            newvals.push_back(it->second);
                    }
                    
                    results.push_back(Fact(res_name, "custom", newvals));
                }
            }
        }
        else {
            // This is an OR rule.
            for(int i = 0; i < r->facts.size(); i++) {
                vector<string> factParams(r->facts[i].params); // Copy parameter format for editing
                
                // Iterate through each parameter and try to match it to a previous parameter.
                for(int j = 0; j < factParams.size(); j++) {
                    auto it = rule_param_to_calling_param.find(factParams[j]);
                    if(it != rule_param_to_calling_param.end())
                        factParams[j] = it->second;
                }
                // factParams now contains the list of calling params where
                // any previously defined param of the rule is replaced with
                // a calling param, if available.
                
                vector<Fact> fact_results = query(r->facts[i].name, factParams);
                results.insert(results.end(), fact_results.begin(), fact_results.end());
            }
        }
    }
    return results;
}


// Perform a query on either a rule or a fact.
// Scans the KB and RB for matches to name, then calls queryFact or queryRule
// depending on where we found the match.
//
// Assumes no duplicate names exist between rules and facts.
// In this situation, it will only return the first entry found (the queryfact results).
vector<Fact> SRI_Engine::query(string name, vector<string> params) {
    vector<Fact> results;
    auto fi = facts.find(name);
    if(fi != facts.end())
        return queryFacts(name, params);
    
    auto ri = rules.find(name);
    if(ri != rules.end())
        return queryRules(name, params);
    
    std::cout << "Couldn't find a match in facts or rules.\n";
    return results; // return empty list.
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
// Rule printing currently wonky while I mess with the rule definition.
void SRI_Engine::print() {
    // Print all facts
    for(auto i : facts) {
        std::cout << "facts[" << i.first << "] = \n";
        for(auto j : i.second) {
            std::cout << j.def << std::endl;
            //for(auto k : j.vals)
            //    std::cout << k << ",";
            //std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    
    // Print all rules
    for(auto i : rules) {
        std::cout << "rules[" << i.first << "] = \n";
        for(auto j : i.second) {
            std::cout << j.def << std::endl;
            //std::cout << "params: ";
            //for(auto k : j.params) std::cout << k << ",";
            //std::cout << std::endl;
            
            //std::cout << "facts: " << std::endl;
            //for(auto k : j.facts) {
            //    std::cout << k.name << "(";
            //    for(auto x : k.params) std::cout << x << ",";
            //    std::cout << ")" << std::endl;
            //}
        }
        std::cout << std::endl;
    }
};
