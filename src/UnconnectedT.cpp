#include "Terminal.cpp"

using namespace std ; 

class UnconnectedT : public Terminal{
    private :

    string id ; 

    public :

    UnconnectedT(string id_, int node) : Terminal("2" + id_, 0.2, node) {} ;



   void process_msg() {
       string delimiter = "," ;
       for (string a_msg : get_msg()) {
           vector<string> msg_parsed = parse_msg(a_msg, delimiter) ;
           string msg_type = msg_parsed[0] ;
           msg_parsed.erase(msg_parsed.begin()) ;
           vector<string> param = msg_parsed ;
           if (msg_type == "delegate") {
               delegate_function(param[0], param[1]) ;
           }
           else {
               cout << "Error : deal with unknown message\n" ;
               exit(42) ;
           }
       }
   }

       void actualise_environ() {
        vector<Agent*> res ; 
        for (Agent* ag : get_world()) {
            if ((ag->get_id() != get_id()) && near_to(ag->get_pos() , get_pos(), ag->get_scope(), get_scope())){
                res.push_back(ag) ; 
            }
        }
        set_environ(res) ; 
        if(get_environ().size() > 0) {print_environnement(get_environ()) ;}
    }

    void routine() {
        actualise_environ() ; 
        flooding() ; 
      }

    



};