#include "Terminal.cpp"


using namespace std ; 

class ConnectedT : public Terminal {
    private :

    string id ;

    public :

     ConnectedT(string id_, int node) : Terminal("1" + id_, 0.3, node) {} ;

     void routine() {
        actualise_environ() ; 
        flooding() ; 
      }

    void actualise_environ() {
        vector<Agent*> res ; 
        for (Agent* ag : get_world()) {
            if (ag->get_id().substr(0,1) == "1" && (ag->get_id() != get_id())) {
                res.push_back(ag) ; 
            }
            else { 
                if ((ag->get_id() != get_id()) && near_to(ag->get_pos() , get_pos(), ag->get_scope(), get_scope())){
                    res.push_back(ag) ; 
                }
            }

        }
        set_environ(res) ; 
        print_environnement(get_environ()) ;
    }


    void process_msg() {
        string delimiter = "," ;
        for (string a_msg : get_msg()) {
            vector<string> msg_parsed = parse_msg(a_msg, delimiter) ;
            string msg_type = msg_parsed[0] ;
            msg_parsed.erase(msg_parsed.begin()) ;
            vector<string> param = msg_parsed ;
            if (msg_type == "delegate") {
                delegate_function_connected_version(param[0], param[1]) ;
            }
            else {
                cout << "Error : deal with unknown message\n" ;
                exit(42) ;
            }
        }
    }

    void delegate_function_connected_version(string m, string j) {
        traces_mutualise(j) ;
        print_trace(j) ; 
        if ((get_traces()).count(j) >= 1) {
            infos vec = get_traces()[j] ;
            tuple<double, string> last = get_last_traces(vec) ;
            string last_t = get<1>(last) ; 
            int nb_co_term = 0; 
            for (Agent* ag : get_world()) {
                if (ag->get_id().substr(0,1) == "1") {
                    nb_co_term ++ ;
                }
            }
            int is_old = old(get<0>(last)) ;
            if (is_old >= 1) {
                cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t *** Agent " << get_id() << " delegates to : " << last_t << "\n" ; 
                have_to_forward(m,j,last_t) ;
            }
            //TODO : add neighbours coTerminal.
            else {
                have_to_forward(m,j,last_t) ;
            }
        }

    }

    void have_to_forward( string m, string j, string k) {
        Terminal* t = id_to_terminal(k) ;
        t->delegate_function(m,j) ; 
    }

    tuple<double, string> get_last_traces(infos vec) {
        tuple<double, string> res(get<0>(vec[0]) , get<1>(vec[0]));
        for (info i : vec) {
            if (get<0>(i) < get<0>(res)){
                tuple<double,string> aux(get<0>(i) , get<1>(i)) ;
                res = aux ; 
            }
        }
        return res ;
    }

    int old(double l) {
     //   TODO : ici les choix sont totalement arbitraires.
        if (l >= 4800) {
            return 4 ;
        }
        if ( l >= 2400) {
            return 3 ; 
        }
        if ( l >= 1200) {
            return 2 ; 
        }
        return 1 ;
    }
    void traces_mutualise(string j) {
        for (Agent* ag : get_world()) {
            if (ag->get_id().substr(0,1) == "1") {
                infos traces = (ag->get_traces())[j] ;
                infos my_traces = get_traces()[j] ;
                for (auto t : traces) {
                    my_traces.push_back(t) ;
                }
                set_traces(my_traces, j) ;
            }
        }
    }

    


};