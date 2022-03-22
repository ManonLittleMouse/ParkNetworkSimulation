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

    // Buffers de messages : 
    vector<string> msg ;
    vector<tuple<string, Agent*, float>> msg_flood  ; 
    vector<tuple<string, Agent*, float>> msg_flood_v2  ; 

    // configs : 
    string id ; 
    pos my_pos ; 
    double scope  ;

    // Infos about world 
    vector<Agent*> world ;
    float clock ; 
    vector<Agent*> environ ;
    traces_type traces ; 
    bool is_in_park = true ; // become false when the agent is no more in the parK. Deleted after a new step of simulation.

    // statistics
    vector<string> delivered_msg ; 
    int nb_messages_receive = 0 ; 

    vector<string> delivered_msg_flood ; 
    int nb_messages_receive_flood = 0 ; 

    vector<string> delivered_msg_flood_v2 ; 
    int nb_messages_receive_flood_v2 = 0 ; 

public :


    // *********** INITIALISATION ***************
    Agent(string id_, double scope_ , pos my_pos_) : id(id_),  scope(scope_) , my_pos(my_pos_) {
    } ; 

    // ************ GETTER/SETTER *****************

    //Configs 
    string get_id() {return id ;}
    double get_scope() {return scope ;} 

    // World managements 
    vector<Agent*> get_environ() {return environ ;}
    void set_environ(vector<Agent*> new_environ) {environ = new_environ ;}
    void set_pos(pos t){my_pos = t ;}
    pos get_pos(){return my_pos ;}
    vector<Agent*> get_world() {return world; }
    void set_world(vector<Agent*> w) { world = w ; }
    void set_clock(float c) {clock = c ;}
    float get_clock() {return clock ; }
    traces_type virtual get_traces() {return traces ; } 
    void virtual set_traces(infos t, string j) {}
    void set_is_in_park(bool u) {is_in_park = u ; }
    bool get_is_in_park() {return is_in_park ; }

    //msg managemnts 
    vector<string> get_msg() {return msg ;}
    vector<tuple<string, Agent*, float >> get_msg_flood() {return msg_flood ; }

    // statistics variables
    int get_nb_m_received() {return nb_messages_receive;}
    int get_nb_m_received_flood() {return nb_messages_receive_flood;}
    int get_nb_m_received_flood_v2() {return nb_messages_receive_flood_v2;}
    vector<string> get_delivered_msg() {return delivered_msg ;}

    int virtual get_accurate_path() { return 0 ; }


    //************** MESSAGES MANAGEMENT *************

    void send(string a_msg, Agent* dest, float date) {
        //send_msg(a_msg, dest) ; 
        send_msg_by_flooding(a_msg, dest, date) ; 
        send_msg_by_flooding_v2(a_msg, dest, date) ; 
    }


    // FLOODING V1 ( with terminals ) 
    void send_msg_by_flooding(string a_msg, Agent* dest, float date ) {
        tuple<string, Agent* , float > t(a_msg, dest, date) ; 
        msg_flood.push_back(t) ; 
    }

    int flooding() {   
        for(tuple<string, Agent*, float> t :msg_flood) {
            Agent* dest = get<1>(t) ; 
            for(Agent* ag : get_environ()) {
                if (ag == dest){
                    dest->delivery_msg_flood(get<0>(t)) ; 
                }
                else {
                    if (!(ag->is_here(get<0>(t)))) {
                        cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t flooding v1 :  <<" << get<0>(t) << "," << get<1>(t)->get_id() << " >> from " << get_id() << " to " << ag->get_id()  << "\n" ;
                        ag->msg_flood.push_back(t) ; 
                    }
                }
            }
        }
        return 0 ; 
    }
    bool is_here(string msg) {
        bool res = false ; 
            for (tuple<string, Agent*, float> t2 : msg_flood) {
                string msg2 = get<0>(t2) ;  
                if (msg == msg2) {
                    res = true; 
                }
            }
        return res ;
    }
    void clean_old_msg() {
        vector<tuple<string, Agent*, float>> aux_msgs ; 
        for (tuple<string, Agent*, float> t : msg_flood) {
            float date = get<2>(t) ; 
            //if (date > clock - 86400 ) { // Les messages sont supprimés au bout d'une journée. 
            if (true) {
                aux_msgs.push_back(t) ; 
            }
        }
        msg_flood = aux_msgs ; 
    }
    void delivery_msg_flood(string a_msg) {
        bool already_delireved = false ; 
        for (string m : delivered_msg_flood) {
            if (m == a_msg) {
                already_delireved = true ; 
            }
        }
        if (!already_delireved){
            nb_messages_receive_flood++ ; 
            delivered_msg_flood.push_back(a_msg) ; 
            cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t ------------> MESSAGE <<" << a_msg << ">> IS ARRIVED AT DESTINATION (by flooding w/ terminals): " << get_id() << "\n" ; 
        }  
    }

    // FLOODING v2 (only people) ; 

    void send_msg_by_flooding_v2(string a_msg, Agent* dest, float date ) {
        tuple<string, Agent* , float > t(a_msg, dest, date) ; 
        msg_flood_v2.push_back(t) ; 
    }
    int flooding_v2() { 
        for(tuple<string, Agent*, float> t :msg_flood_v2) {
            Agent* dest = get<1>(t) ; 
            for(Agent* ag : get_environ()) {
                if (ag == dest) {
                    dest->delivery_msg_flood_v2(get<0>(t)) ; 
                }
                if (ag->get_id().substr(0,1) == "3") {
                    if (!(ag->is_here_v2(get<0>(t)))) {
                        cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t flooding v2 :  <<" << get<0>(t) << "," << get<1>(t)->get_id() << " >> from " << get_id() << " to " << ag->get_id()  << "\n" ;
                        ag->msg_flood_v2.push_back(t) ; 
                    }
                }
            }
            
        }
        return 0 ; 
    }
    void clean_old_msg_v2() {
        vector<tuple<string, Agent*, float>> aux_msgs ; 
        for (tuple<string, Agent*, float> t : msg_flood_v2) {
            float date = get<2>(t) ; 
//            if (date > clock - 86400 ) { // Les messages sont supprimés au bout d'une journée. 
            if (true) {
                aux_msgs.push_back(t) ; 
            }
        }
        msg_flood_v2 = aux_msgs ; 
    }
    bool is_here_v2(string msg) {
        bool res = false ; 
            for (tuple<string, Agent*, float> t2 : msg_flood_v2) {
                string msg2 = get<0>(t2) ;  
                if (msg == msg2) {
                    res = true; 
                }
            }
        return res ;
    }
    void delivery_msg_flood_v2(string a_msg) {
        bool already_delireved = false ; 
        for (string m : delivered_msg_flood_v2) {
            if (m == a_msg) {
                already_delireved = true ; 
            }
        }
        if (!already_delireved){
            nb_messages_receive_flood_v2++ ; 
            delivered_msg_flood_v2.push_back(a_msg) ; 
            cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t ------------> MESSAGE <<" << a_msg << ">> IS ARRIVED AT DESTINATION (by flooding v2): " << get_id() << "\n" ; 
        }  
    }
    
    // OUR ALGORITHMS 
    // void virtual send_msg(string a_msg, Agent* dest) {} // The function is called when an Agent (just a People for now) want to send a msg to another people.

  
    void virtual send_msg(string a_msg, Agent* dest) {}
    void receive_msg(string a_msg){
        // Only for communication msg (= algorithm msg) 
        cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t *** Agent " << get_id() << " received <<" << a_msg << ">>\n" ;
        msg.push_back(a_msg) ;
        process_msg() ; // When an agent receive a msg, it is directly processed.

  
    }
    void delivery_msg(string a_msg) {
        bool already_delireved = false ; 
        for (string m : delivered_msg) {
            if (m == a_msg) {
                already_delireved = true ; 
            }
        }
        if (!already_delireved){
            nb_messages_receive++ ; 
            delivered_msg.push_back(a_msg) ; 
            cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t ------------> MESSAGE <<" << a_msg << ">> IS ARRIVED AT DESTINATION: " << get_id() << "\n" ; 
        }           
    }
    void broadcast_msg(string a_msg, Agent* receiver) {
    //Phyiscally send the msg to an agent.
        receiver->receive_msg(a_msg) ;
    }
    void virtual process_msg() {
        cout << "Agent " << get_id() << " has to compute messages : \n process : not overloading function ! \n" ;
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
    
   

    void virtual actualise_environ() {
        vector<Agent*> res ; 
        for (Agent* ag : get_world()) {
            if ((ag->get_id() != get_id()) && near_to(ag->get_pos() , get_pos(), ag->get_scope(), get_scope())){
                res.push_back(ag) ; 
            }
        }
        set_environ(res) ; 
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
        // cout << "TODO : evaluate traces \n" ;
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


    //******************* Simulation *****************

    void virtual simulation(vector<Agent*> world, float clock) {}

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
            cout << "\tAgent " << get_id()<<"( " ;
            print_pos(get_pos()) ; 
            cout << ")" << " has company: " ;
            for (Agent* ag : e) {
                cout << ag->get_id() ;
                //print_pos(ag->get_pos()) ;
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
    
    void print_msg_flood() {
        cout << " msg_flood of " << get_id()  << ": " ; 
        for (auto t : msg_flood) {
            string a = get<0>(t) ; 
            Agent* b = get<1>(t) ; 
            float c = get<2>(t) ; 
            cout << "(" << a << "," << b->get_id() << "," << to_string(c) << ") " ; 
        }
        cout << "\n" ; 
    }

    void print_flood() {
        cout << "msg to flood v1 : \n" ; 
        for (tuple<string, Agent*, float> t :msg_flood){
            string msg = get<0>(t) ; 
            cout << msg << " " ; 
        }
        cout << "\n" ; 
        cout << "msg to flood v2 : \n" ; 
        for (tuple<string, Agent*, float> t :msg_flood_v2){
            string msg = get<0>(t) ; 
            cout << msg << " " ; 
        }
        cout << "\n" ; 
    }

    void print_delivered_flood() {
    cout << "delivered msg flood v1 : \n" ; 
        for (string m : delivered_msg_flood){ 
            cout << m << " " ; 
        }
        cout << "\n" ; 
        cout << "delivered msg flood v2 : \n" ; 
        for (string m :delivered_msg_flood_v2){
            cout << m << " " ; 
        }
        cout << "\n" ; 

    }
};
  
#endif