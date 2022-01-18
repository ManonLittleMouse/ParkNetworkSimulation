#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <cmath>

using namespace std ; 

typedef pair<int,double> proba ;
typedef vector<proba> probas ;
typedef tuple<double, string, probas> info ;
// (date, z_i option, (path, proba) list )
typedef vector<info> infos ; 
typedef map<string, infos> traces_type ;
typedef tuple<int, int, double,double> pos ; 



class Agent
{
private :

    vector<string> msg ;
    string id ;
    vector<Agent*> environ ;
    probas nearest_terminal_path ; 
    pos my_pos ; 
    vector<Agent*> world ; 
    double scope  ;



public :


    // *********** INITIALISATION ***************
    Agent(string id_, double scope_ , pos my_pos_) : id(id_),  scope(scope_) , my_pos(my_pos_) {
    } ; 

    // ************ GETTER/SETTER *****************

    string get_id() {return id ;}
    vector<Agent*> get_environ() {return environ ;}
    void set_environ(vector<Agent*> new_environ) {environ = new_environ ;}
    vector<string> get_msg() {return msg ;}
    void set_pos(pos t){my_pos = t ;}
    pos get_pos(){return my_pos ;}
    vector<Agent*> get_world() {return world; }
    void set_world(vector<Agent*> w) { world = w ; }
    double get_scope() {return scope ;} 

    //************** MESSAGES MANAGEMENT *************

    void receive_msg(string a_msg){
        cout << "Agent " << get_id() << "received <<" << a_msg << ">>\n" ;
        msg.push_back(a_msg) ;
        compute_msg() ;
    }

    void broadcast_msg(string a_msg, Agent* receiver) {
        receiver->receive_msg(a_msg) ;
    }

    void virtual compute_msg() {
        cout << "Agent " << get_id() << " has to compute messages : \n Error : not overloading function ! \n" ;
    }

    auto parse_msg(string a_msg, string delimiter) {
        vector<string> result ;
        size_t start = 0;
        size_t end = 0 ;
        string msg_type ;
        if ((end = a_msg.find(delimiter))!= string::npos) {
                msg_type = a_msg.substr(start,end) ;
        }
        start = end + delimiter.length() ;
        end = a_msg.find(delimiter, start) ;
        vector<string> param ;
        while (end != std::string::npos) {
            param.push_back(a_msg.substr(start,end)) ;
            start = end +delimiter.length() ;
            end = a_msg.find(delimiter, start ) ;
        }

        return (msg_type, param); 
    }

    traces_type parse_traces(string infos) {
        cout << "todo : parse_infos" ;
        traces_type res ; 
        return res ; 

    }


    // Debug print 
    void print_msg() {
        for (string a_msg : msg) {
            cout << a_msg  << " . " ; 
        }
        cout << "\n" ;
    }

    //************** ENVIRON MANAGEMENT **************
    
    void initialisation(vector<Agent*> world) {
        vector<Agent*> terminals ; 
        for (Agent* ag : world) {
            if (ag->get_id().substr(0,1) == "1") {
                terminals.push_back(ag) ;
            }
        }
        Agent* nearest_terminal = terminals[0] ;
        cout << "TODO : terminer initialisation des agents" ;
    }

    void actualise_environ() {
        vector<Agent*> res ; 
        for (Agent* ag : get_world()) {
            if ((ag->get_id() != get_id()) && near_to(ag->get_pos() , get_pos(), ag->get_scope(), get_scope())){
                res.push_back(ag) ; 
            }
        }
        set_environ(res) ; 
        print_environnement(get_environ()) ;
    }

    vector<Agent*> get_terminals() {
        vector<Agent*> terminals ;
        for (Agent* x : get_environ()) {
            if (x->get_id().substr(0,1) == "1" || x->get_id().substr(0,1) == "2"){
                terminals.push_back(x) ;
            }
        }
        return terminals ;
    }

    vector<Agent*> get_peoples(){
        vector<Agent*> peoples ;
        for (Agent* x : get_environ()) {
            if (x->get_id().substr(0,1) == "3"){
                peoples.push_back(x) ;
            }
        }
        return peoples ;
    }

    Agent*  id_to_agent(string id, vector<Agent*> world) {
        Agent* res ;
        for (Agent* ag : world) {
            if (ag->get_id()  == id) {
                res = ag ; 
            }
        }
        return res ;
    }

    

    //**************** TRACE MANAGEMENT ******************

    double virtual evaluate_traces(string j, traces_type infos, string x) {
        cout << "TODO : evaluate traces \n" ;
        return 0 ; 
    }

     bool virtual near_to(pos a, pos b, double s1, double s2) {
        // test if a people a is around a terminal b
        int a1 = get<0>(a);
        int a2 = get<1>(a);
        double a3 = get<2>(a);
        double a4 = get<3>(a) ; 
        int b1 = get<0>(b);
        
        if(a1 == b1)  {
            if (a3 <= s1 + s2) {
                return true ;
            }
            else {
                return false ;
            }
        }
        if(a2 == b1) {
            if (a4 <= s1 + s2){
                return true ;
            }
            else {
                return false ;
            }

        }
        return false ;
    }

    //****************** TIME ********************
    void sleep_for(int i) {
        sleep(i);
    }

    //******************* Simulation *****************

    void virtual simulation(vector<Agent*> world) {}

    // ****************** Prettyprint **************

    void print_pos(pos t) {
        int a = get<0>(t) ;
        int b = get<1>(t) ;
        double c = get<2>(t) ; 
        double d = get<3>(t) ;
        cout << "(" << to_string(a) << ", " << to_string(b) << ", " << to_string(c) << ", " << to_string(d) << ")" ;
        }

    void print_path(vector<int> vec) {
        for (int i : vec) {
            cout << to_string(i) << " " ;
        } 
    }

    void print_path_distance(vector<double> vec) {
        for (double i : vec) {
            cout << (to_string(i)).substr(0,4) << " " ;
        } 
    }

    void print_environnement(vector<Agent*> e) {
        if (e.size() > 0) {
            cout << "\tAgent " << get_id()<<"(node:"<< to_string(get<0>(get_pos()))<< " + " << to_string(get<2>(get_pos())) << ")" << " has company: " ;
            for (Agent* ag : e) {
                cout << ag->get_id() ;
                print_pos(ag->get_pos()) ;
                cout  << " " ;
            }
            cout << "\n" ;
        }
    }
    
};
  
#endif