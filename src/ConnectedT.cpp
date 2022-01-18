#include "Terminal.cpp"


using namespace std ; 

class ConnectedT : public Terminal {
    private :

    string id ;

    public :

    ConnectedT(string id_, int node) : Terminal("1" + id_, 0.3, node) {} ;

    void routine(){

        //cout << "Connected terminals " << get_id() << " do its routine ! "<< "\n" ;
        actualise_environ() ;
        //actualise_traces() ;
    }

    void compute_message() {
        string delimiter = "," ;
        for (string a_msg : get_msg()) {
            string msg_type ;
            vector<string> param ;
            msg_type, param = parse_msg(a_msg, delimiter) ;
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
        traces_mutualise() ;
        if ((get_traces()).count(j) == 1) {
            infos vec = get_traces()[j] ;
            tuple<double, string> last = get_last_traces(vec) ;
            for (int k = stoi(j) - old(get<0>(last)) ; k < stoi(j) + old(get<0>(last)) ; k++) {
                have_to_forward(m,j,k) ; 
            }
        }

    }

    void have_to_forward( string m, string j, int k) {
        id_to_terminal(to_string(k), {})->delegate_function(m,j) ; 
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
        cout << "TODO : connectedT.old\n" ;
        return 0 ;
    }
    void traces_mutualise() {
        cout << "TODO : connected_terminals : traces_mutualise()\n" ;
    }

    


};