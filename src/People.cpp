#include <iostream>
#include <vector>
#include <cmath>

#include "Agent.cpp"

using namespace std ; 

class People : public Agent {
    private :
    vector<int> path ; 
    vector<double> path_distances ; 
    int step = 0 ; 
    vector<tuple<string, Agent*>> msg_to_send ; 
    vector<vector<string>> msg_to_forward ; 
    double delta_d = 0.1 ; 

    double my_abs(double a1) {
        if (a1 <= 0) {
            return (-a1)  ;
        }
        else {return a1 ; }
    }
    
    public :

        //***************************** Initialisation ******************************
        People(string id_, vector<int> path_, vector<double> path_d_, double c) : Agent("3" + id_, 0.1 , pos(path_[0], path_[1], 0, path_d_[0])), path(path_), path_distances(path_d_){
            set_clock(c) ; 
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

            if (path_distances.size() != path.size() - 1) {
                cout << "Error in path generation : distances (" 
                << path_distances.size()<< ", " << path.size() << ") are not enought or too much\n" ;
                exit(42) ;
            }
        } ;
        //**********************************************************************
        //******************************* Getter/Setter *************************

        vector<int> get_path() { return path ;}
        int get_accurate_path() {return path[step] ; }

        //***********************************************************************
        //*************************** Simulation *************************
        void simulation(vector<Agent*> w, float c) {
            set_clock(c) ; 
            set_world(w) ; 
            actualise_environ() ; 
            //************ People deplacement ***********************
            pos p = get_pos() ;
            int current_node = get<0>(p);
            int next_node = get<1>(p) ; 
            double new_dist = (get<2>(p)) + delta_d;
            double new_rest = (get<3>(p)) - delta_d; 
            if (my_abs(new_dist - path_distances[step]) <= 0.01) {
                if (new_rest >= 0.01) {
                    cout << "ERROR : People.simulation() : wrong computing of distances\n" ; 
                    exit(42) ; 
                }
                current_node = get<1>(get_pos()) ; 
                new_dist = 0 ; 
                step += 1 ;
                if (step >= path.size() - 1) { // Condition of end of path
                    cout << " ------------- /!  People " << get_id() << "end its path ! \n" ;
                    //set_is_in_park(false) ; 
                    step = 0 ; 
                }
                next_node = path[step+1] ; 
                new_rest = path_distances[step] ;
                cout << "People " << get_id() << " has reach a new step: " << to_string(current_node) << "\n" ;
                
            }

            if (new_rest < 0 || new_dist < 0 ) {
                cout << "Error in computing distances of people ! \n";
                exit(42) ;
            }

            set_pos(pos(current_node, next_node, new_dist, new_rest)) ;
            //*********************************************
            routine() ; // People send/forward messages if they have messages in their buffers. 
        }

        void routine() {
            //print_flood() ; 
            //print_delivered_flood() ; 
            flooding() ; 
            flooding_v2() ; 
            // vector<tuple<string, Agent*>> send_aux = msg_to_send ; 
            // msg_to_send = {} ;
            // for (auto t : send_aux) {
            //     send_msg(get<0>(t), get<1>(t)) ;
            // }

            // vector<vector<string>> forward_aux = msg_to_forward ; 
            // msg_to_forward = {} ;
            // for (auto t : forward_aux) {
            //     forward(t) ; 
            // }
        }
        void actualise_environ() {
            vector<Agent*> res ; 
            for (Agent* ag : get_world()) {
                if ((ag->get_id() != get_id()) && near_to(ag->get_pos() , get_pos(), ag->get_scope(), get_scope())){
                    res.push_back(ag) ; 
                }
            }
            set_environ(res) ;
            print_environnement(res) ; 
        }
    //************************* Messages management (= Algorithm) *************************

        void send_msg(string a_msg, Agent* dest) {
            actualise_environ() ;
            bool sended = false ; 
            for (Agent* a : get_environ()) {
                if (a == dest) {
                    dest->delivery_msg(a_msg) ; 
                    sended = true ; 
                }
            }
            if (sended == false && not(get_terminals().empty())){
                for (Agent* pj : get_terminals()) {
                    cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t --- Agent "<< get_id() << " send <" << "delegate," + a_msg + "," + dest->get_id() << "> to " << pj->get_id() << "\n" ; 
                    broadcast_msg("delegate," + a_msg + "," + dest->get_id() , pj) ; 
                }
            }
            else {
                if (not(sended)) {
                    tuple<string, Agent*> t(a_msg, dest) ; 
                    msg_to_send.push_back(t) ;
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
                msg_to_forward.push_back(param) ; 
            }
            else {
                string a_msg = param[0] ;
                string dest = param[1] ;
                double k = stod(param[2]) ;
                string t = param[3] ;
                string infos = param[4] ;
                bool forwarded = false ;
                for (auto a : get_environ()) {
                    if (a->get_id() == dest) {
                        id_to_agent(dest)->delivery_msg(a_msg) ; 
                        forwarded = true ; 
                    }
                }
                if(not(forwarded) && not(get_terminals().empty()) && not(get_terminals().size() == 1 && ((get_terminals()[0])->get_id() == t))) {
                    for (Agent* x : get_terminals()) {
                        broadcast_msg("delegate," + a_msg + "," + dest , x) ;
                    }
                }
                else{
                    if (not(forwarded)) {
                    for (Agent* x : get_peoples()) {
                        auto k_aux = evaluate_traces(dest, parse_traces(infos), x->get_id()) ;
                        if (k_aux > k) {
                            broadcast_msg("forward," + a_msg + "," + to_string(k_aux) + "," + t + "," + infos , x) ;
                            forwarded = true ;
                        }
                    }
                    if (not(forwarded)) {
                        msg_to_forward.push_back(param) ; 
                    }
                    }
                }
            }
        }

        void process_msg() {
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

        //****************************************************************




    //Calcul des voisin proches : 
    bool near_to(pos autre, pos moi, double s_autre, double s_moi){
        int autre_depart = get<0>(autre);
        int autre_arrivee = get<1>(autre);
        double autre_dist_depart = get<2>(autre);
        double autre_dist_arrivee = get<3>(autre) ;         
        int moi_depart = get<0>(moi);
        int moi_arrivee = get<1>(moi);
        double moi_dist_depart = get<2>(moi);
        double moi_dist_arrivee = get<3>(moi) ; 
        // Case autre = people  (x1 = autre, x2 = moi) 
        // 1 )    [1]---- x1 > ---------- < x2 --- [2] 
        if(autre_depart == moi_arrivee && autre_arrivee == moi_depart) {
            if(my_abs(autre_dist_arrivee - moi_dist_depart) <= s_autre + s_moi) {  return true ; }
            else {return false ;}
        }
        // 2) [1] ---- x1> ---------- x2 >-------- [2] 
        if (autre_depart == moi_depart && autre_arrivee == moi_arrivee) {
            if (my_abs(moi_dist_depart - autre_dist_depart) <= s_autre + s_moi) { return true ; }
            else {return false ;}
        }
        // 3) [3] --- x2> ---- [2] ---- x1> ---- [1]
        if (autre_arrivee == moi_depart && autre_depart != moi_arrivee) {
            if (autre_dist_arrivee + moi_dist_depart < s_autre+s_moi) {return true ;}
            else {return false;}
        }
        // 4) [3] ---- x2> ------ [2] ------ <x1 -------- [1]
        if (autre_arrivee == moi_arrivee && autre_depart != moi_depart) {
            if (autre_dist_arrivee + moi_dist_arrivee <= s_autre + s_moi) { return true ;}
            else {return false ;}
        }
        // 5) [3]------ <x2  -------- [2] --- x1> -------- [1]
        if (autre_depart == moi_depart && autre_arrivee != moi_arrivee) {
            if(autre_dist_depart + moi_dist_depart <= s_autre + s_moi) { return true ;}
            else {return false ; }
        }
        // 6) [3] ----------- <x2 ----- [2] ----<x1--------[1]
        if(autre_depart == moi_arrivee && autre_arrivee != moi_depart) {
            if(autre_dist_depart + moi_dist_arrivee <= s_autre + s_moi) {return true ; }
            else {return false ; }
        }


        // Case terminals 
        if (autre_depart == moi_arrivee){
            if (moi_dist_arrivee <= s_autre + s_moi) {return true ;}
            else{return false ; }
        }
        if (autre_depart == moi_depart){
            if(moi_dist_depart <= s_autre + s_moi) {return true ;}
            else {return false ;}
        }
        return false ;
    }
     
       

};