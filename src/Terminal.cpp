#ifndef TERMINAl_H
#define TERMINAl_H

#include "Agent.cpp"
#include <tuple>
#include <iostream>
#include <string>

using namespace std ; 


class Terminal : public Agent{
    private :

    string id ;
    traces_type traces ;
    int node ;
 

    public :

    Terminal(string id_, double scope, int node_) : Agent(id_,  scope, pos(node_, 0, 0, 0)) , node(node_) {} ;

    
    Terminal*  id_to_terminal(string id, vector<Agent*> world) {
        Terminal* res ;
        for (Agent* ag : world) {
            if (ag->get_id()  == id) {
                res = (Terminal *)ag ; 
            }
        }
        return res ;
    }

    void virtual routine() {} ;

    void simulation(vector<Agent*> w) {
        set_world(w) ; 
        routine() ; 
    }
    

    traces_type get_traces() {
        return traces ;
    }

    void actualise_traces() {
        
    }

    double compute_limite(string m) {
        cout << "TODO : terminal.compute_limite()\n " ;
        return 0 ;
    }

    double compute_min(string m) {
        cout << "TODO : terminal.compute_min()\n" ;
        return 0 ;
    }

    string indications(string j) {
        cout << "TODO : terminal.indication\n" ;
        return "" ;
    }

    void delegate_function(string m, string j) {
        vector<tuple<Agent*,double>> relevance ;
        double limite = compute_limite(m) ; 
        double min = compute_min(m) ;
        double K = limite - min * (get_environ()).size() ;
        for (Agent* xk : get_environ()) {
            double k = evaluate_traces(j, traces, xk->get_id()) ;
            tuple<Agent*, double> tmp(xk,k) ;
            relevance.push_back(tmp) ;
        }
        bool forwarded = false ; 
        for (auto res : relevance) {
            Agent* xk = get<0>(res) ;
            double k = get<1>(res) ; 
            if (k> K) {
                broadcast_msg("forward" + m + "," + j+ ","  + to_string(k)+ ","  + get_id() + "," + indications(j) , xk );
                forwarded = true ; 
            }
        }
        if (not(forwarded)) {
            sleep(2) ;
            delegate_function(m,j) ;
        }
            
    }



};

#endif