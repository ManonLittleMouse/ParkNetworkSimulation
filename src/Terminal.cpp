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
    vector<tuple<string,string>> buffer_delegate = {} ;  
 

    public :

    Terminal(string id_, double scope, int node_) : Agent(id_,  scope, pos(node_, 0, 0, 0)) , node(node_) {} ;


    void routine() {

        actualise_environ() ;
        actualise_traces() ; 
        delegate() ; 
    }

    Terminal*  id_to_terminal(string id) {
        Terminal* res ;
        for (Agent* ag : get_world()) {
            if (ag->get_id()  == id) {
                res = (Terminal *)ag ; 
            }
        }
        return res ;
    }


    void simulation(vector<Agent*> w) {
        set_world(w) ; 
        actualise_time() ;
        routine() ; 
    }
    

    traces_type get_traces() {
        return traces ;
    }

    void set_traces(infos new_traces, string j) {
        traces[j] = new_traces ;
    }

    void actualise_traces() {
        for (Agent* ag : get_environ()) {
            info i(get_clock() , get_id(), compute_probas(ag) ) ;
            traces_type infs = get_traces() ;
            infos res ; 
            if (infs.count(ag->get_id()) >= 1) {
                res = infs[ag->get_id()] ; 
            }
            res.push_back(i) ;
            set_traces(res, ag->get_id()) ;
        }

    }

    probas compute_probas(Agent* ag) {
        //TODO : add incertitudes 
        probas res ;
        proba i(ag->get_accurate_path(), 1) ;
        res.push_back(i) ;
        return res ; 

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

    void virtual process_msg() {}

    void delegate_function(string m, string j) {
        bool sended = false ; 
        Agent* dest = id_to_agent(j) ; 
        for (Agent* ag : get_world()) {
            if (ag == dest) {
                dest->destination_msg(m) ; 
                sended = true ; 
            }
        }
        if (not(sended)) {
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
                tuple<string,string> res(m,j) ; 
                buffer_delegate.push_back(res) ; 
            }
        }
        
    }

    //*******************Prettyprint ***************************
    void print_trace(string j) {
        cout << "traces of " << get_id() << " : " ;
        infos is = get_traces()[j] ; 
        for(info i : is) {
            double a = get<0>(i) ;
            string b = get<1>(i) ; 

            cout << j << "(" << to_string(a) << "," << b << ")\n" ;
        }
        cout << "\n" ;
    }

    void delegate(){
        vector<tuple<string,string>> buf_del_aux = buffer_delegate ; 
        buffer_delegate = {} ; 
        for (auto c : buf_del_aux) {
            string m = get<0>(c) ; 
            string j = get<1>(c) ; 
            delegate_function(m,j) ;
        }
    }


};

#endif