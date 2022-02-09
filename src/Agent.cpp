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
// (node init, node goal, dist done, dist not done)



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
    traces_type traces ; 
    double clock = 0 ; 
    double dt = 0.1 ; 


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
    traces_type virtual get_traces() {return traces ; } 
    void virtual set_traces(infos t, string j) {}
    double get_dt() {return dt ; }
    double get_clock() {return clock ;}
    void set_clock(double d) {clock = d ;} 

    int virtual get_accurate_path() { return 0 ; }


    //************** MESSAGES MANAGEMENT *************

    void virtual send_msg(string a_msg, Agent* dest) {}
    // The function is called when an Agent (just a People for now) want to send a msg to another people.

    void receive_msg(string a_msg){
        // Only for communication msg (= algorithm msg) 
        cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t *** Agent " << get_id() << " received <<" << a_msg << ">>\n" ;
        msg.push_back(a_msg) ;
        process_msg() ; // When an agent receive a msg, it is directly processed.
    }

    void destination_msg(string a_msg) {
        cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t ------------> MESSAGE <<" << a_msg << ">> IS ARRIVED AT DESTINATION: " << get_id() << "\n" ; 
                    
    }

    void broadcast_msg(string a_msg, Agent* receiver) {
    // Phyiscally send the msg to an agent.
        receiver->receive_msg(a_msg) ;
    }

    void virtual process_msg() {
        cout << "Agent " << get_id() << " has to compute messages : \n Error : not overloading function ! \n" ;
    }

    vector<string> parse_msg(string a_msg, string delimiter) {
        size_t pos = 0;
        std::string token;
        vector<string> res ; 
        while ((pos = a_msg.find(delimiter)) != std::string::npos) {
            token = a_msg.substr(0, pos);
            res.push_back(token) ;
            a_msg.erase(0, pos + delimiter.length());
        }
        res.push_back(a_msg) ; 
        return res ;

    }

    traces_type parse_traces(string infos) {
        cout << "todo : parse_infos" ;
        traces_type res ; 
        return res ; 

    }


    
    //************** ENVIRON MANAGEMENT **************
    
   

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

    Agent*  id_to_agent(string id) {
        Agent* res ;
        for (Agent* ag : get_world()) {
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
        // test if a people a is and a terminal b
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

    void actualise_time() {
        double time = get_clock() + get_dt() ;
        set_clock(time) ; 
    }

    //******************* Simulation *****************

    void virtual simulation(vector<Agent*> world) {}

    // ****************** Prettyprint **************

    void print_msg() {
        for (string a_msg : msg) {
            cout << a_msg  << " . " ; 
        }
        cout << "\n" ;
    }
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

    void print_parse_msg(vector<string> v) {
        cout << "Parsing message : " ;
        for (string s : v) {
            cout << s << " " ;
        }
        cout << "\n" ;
    }
    
};
  
#endif