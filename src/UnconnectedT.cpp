#include "Terminal.cpp"

using namespace std ; 

class UnconnectedT : public Terminal{
    private :

    string id ; 

    public :

    UnconnectedT(string id_, int node) : Terminal("2" + id_, 0.2, node) {} ;

    void routine() {
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
                delegate_function(param[0], param[1]) ;
            }
            else {
                cout << "Error : deal with unknown message\n" ;
                exit(42) ;
            }
        }
    }

    



};