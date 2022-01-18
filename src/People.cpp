#include <iostream>
#include <vector>

#include "Agent.cpp"

using namespace std ; 

class People : public Agent {
    private :
    vector<int> path ; 
    vector<double> path_distances ; 
    int step = 0 ; 
    double delta_d = 0.1 ; 
    
    public :
        People(string id_, vector<int> path_, vector<double> path_d_) : Agent("3" + id_, 0.1 , pos(path_[0], path_[1], 0, path_d_[0])), path(path_), path_distances(path_d_){
            if (path_distances.size() != path.size() - 1) {
                cout << "Error in path generation : distances are not enought or too much\n" ;
                exit(42) ;
            }
            cout << "New People creation : \n" ;
            cout << "|\t local id: " << id_ << "\n" ;
            cout << "|\t global id: " << get_id() << "\n" ;
            cout << "|\t path: [" ;
            print_path(path_) ;
            cout <<  "]\n" ; 
            cout << "|\t path_distance: [" ;
            print_path_distance(path_distances) ;
            cout << "]\n" ;
            cout << "|\t pos: " ;
            print_pos(get_pos()) ;
            cout << "\n" ;
            cout << "|\t scope: "  << to_string(get_scope()) << "\n" ;
        } ;


        

        void send_msg(string a_msg, Agent* dest) {
            cout << "People " << get_id() << " wants to send the message << " << a_msg << " to " << dest->get_id() << " ! \n" ;
            actualise_environ() ;
            if (not(get_terminals().empty())){
                for (Agent* pj : get_terminals()) {
                    broadcast_msg("delegate," + a_msg + "," + dest->get_id() , pj) ; 
                }
            }
            else {
                sleep(2) ; 
                send_msg(a_msg, dest) ;
            }
        }

        void compute_msg() {
            string delimiter = "," ;
            for (string a_msg : get_msg()) {
                string msg_type ;
                vector<string> param ;
                msg_type, param = parse_msg(a_msg, delimiter) ;
                if (msg_type == "forward") {
                    forward(param) ;
                }
                else {
                    cout << "Error : deal with unknown message \n " ;
                    exit(42) ;
                }
                               
            }
        }

        void forward(vector<string> param) {
            if (param.size() != 5) {
                cout << "Error : People.forward(param) : Too many/not enought parameters ! \n" ;
                exit(42) ;
            }
            actualise_environ() ;
            if (get_environ().empty()) {
                sleep_for(2);
                forward(param) ;
            }
            else {
                string a_msg = param[0] ;
                string dest = param[1] ;
                double k = stod(param[2]) ;
                string t = param[3] ;
                string infos = param[4] ;
                if(not(get_terminals().empty()) && not(get_terminals().size() == 1 && ((get_terminals()[0])->get_id() == t))) {
                    for (Agent* x : get_terminals()) {
                        broadcast_msg("delegate," + a_msg + "," + dest , x) ;
                    }
                }
                else{
                    bool forwarded = false ;
                    for (Agent* x : get_peoples()) {
                        auto k_aux = evaluate_traces(dest, parse_traces(infos), x->get_id()) ;
                        if (k_aux > k) {
                            broadcast_msg("forward," + a_msg + "," + to_string(k_aux) + "," + t + "," + infos , x) ;
                            forwarded = true ;
                        }
                    }
                    if (not(forwarded)) {
                        sleep(2) ;
                        forward(param) ;
                    }
                }
            }
        }

        //********************* Simulation *************************

        void simulation(vector<Agent*> w) {
            pos p = get_pos() ;
            int current_node = get<0>(p);
            int next_node = get<1>(p) ; 
            double new_dist = (get<2>(p)) + delta_d ;
            double new_rest = (get<3>(p)) - delta_d ; 
            cout << "People : " << get_id() << " : " << to_string(new_dist) << "," << to_string(new_rest) << "," << to_string(path_distances[step]) << "\n" ;
             if (abs(new_dist - path_distances[step]) <= 0.01) {
                 if (new_rest >= 0.01) {
                     cout << "ERROR : People.simulation() : wrong computing of distances\n" ; 
                     exit(42) ; 
                 }
                current_node = get<1>(get_pos()) ; 
                new_dist = 0 ; 
                step += 1 ;
                if (step >= path.size()) {
                    cout << "ERROR : People.simulation() : People ends its paths." ;
                    exit(42) ;
                }
                next_node = path[step+1] ; 
                new_rest = path_distances[step] ;
                cout << "People " << get_id() << " has reach a new step: " << to_string(current_node) << "\n" ;
                
            }

            set_pos(pos(current_node, next_node, new_dist, new_rest)) ;
            set_world(w) ; 
            actualise_environ() ; 

        }


    bool near_to(pos a, pos b, double s1, double s2) {
        // test if a people a is around a terminal b
        int a1 = get<0>(a);
        int a2 = get<1>(a);
        double a3 = get<2>(a);
        double a4 = get<3>(a) ; 
        int b1 = get<0>(b);
        int b2 = get<1>(b);
        int b3 = get<2>(b);
        int b4 = get<3>(b) ; 

        
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
        //test if 2 peoples are clos 
        else {
            if (a1 == b1 && a2 == b2) { // they are on the same path
                if (abs(a3 - b3) <= s1 + s2) {
                    return true ;
                }
                else {return false ;}
            }
            if (a1 == b2 && a2 == b1) { // They cross over the same path
                if (abs (a4 - b3) <= s1 + s2) {
                    return true ;
                }

                else {return false ;}
            }
            if (a1 == b1) { // They are just near to the same node
                if (a3 + b3 <= s1 + s2) {
                    return true ;
                }
                else {return false ;}
            }
            return false ; // Other cases are wrong
        }
    }

        
       

};